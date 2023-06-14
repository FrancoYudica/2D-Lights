#include <iostream>
#include "renderer.h"
#include "utils.h"
#include "sdf_functions.h"
#include <execution>

#define MARCH_HIT_DIST 1e-4f
#define OFFSET 1e-3f

namespace Lights2D
{
    void Renderer::render()
    {
        uint32_t sample_size = static_cast<uint32_t>(std::sqrt(config.samples));

        std::for_each(
            std::execution::par_unseq,
            height_values.begin(),
            height_values.end(),
            [sample_size, this](uint32_t y)
            {
                Utils::random_seed(y);
                for (uint32_t x = 0; x < config.width; x++)
                {

                    Color<float> accumulated;
                    Vec2 uv(
                        static_cast<float>(x) / config.width,
                        1.0f - static_cast<float>(y) / config.height
                    );

                    // Multi sampling - Samples a grid
                    //for (uint32_t sample = 0; sample < config.samples; sample++)
                    //    accumulated += _sample(uv, sample);            
                    uint32_t sample_index = 0;
                    for (uint32_t x_sample = 0; x_sample < sample_size; x_sample++)
                    {
                        for (uint32_t y_sample = 0; y_sample < sample_size; y_sample++)
                        {
                            // Adds offset, this way samples from the sample pixel are not in the same position inside the pixel
                            Vec2 offset(
                                static_cast<float>(x_sample),
                                static_cast<float>(y_sample)
                                );
                            offset.x /= sample_size;
                            offset.y /= sample_size;
                            offset = offset - 0.5f;
                            offset.x /= config.width;
                            offset.y /= config.height;

                            // If false, disables offset
                            offset *= config.antialias;
                            accumulated += _sample(uv + offset, sample_index++);            
                        }
                    }
                    accumulated /= static_cast<float>(config.samples);

                    // Color<float> -> Color<uint8_t> is overwritten, and values are mapped [0, 1] to [0, 255] automatically
                    Color<uint8_t> byte_color = (Color<uint8_t>)Color<float>::clamp(accumulated, 0, 1.0f);
                    img->set_pixel(x, y, byte_color);
                }

                if (debug)
                    std::cout << "Calculating: " << 100.0f * y / (config.height - 1.0f) << "%"<< std::endl;

            }
        );  
    }

    Color<float> Renderer::_ray_march(Vec2 origin, Vec2 direction, uint32_t depth)
    {
        float t = 0.0f;
        for (uint32_t i = 0; i < config.ray_march_max_iterations; i++)
        {
            Vec2 point = origin + direction * t;
            Nearest nearest = sdf(point, _time);

            float sign = nearest.distance > 0.0f ? 1.0f : -1.0f;
            float unsigned_distance = sign * nearest.distance;
            Material& material = nearest.mtl;
            if (unsigned_distance < MARCH_HIT_DIST)
            {
                // Adds the intensity
                Color color = material.emission * material.emission_intensity;
                Color<float> accumulated_color = color;

                // Check for reflection and refraction
                if (depth < config.max_recursion_depth && (material.reflectivity > 0.0f || material.ior > 0.0f))
                {

                    Vec2 normal = this->gradient(point);
                    // In case we are inside the object, flipping the normal gives us the correct result
                    normal *= sign;

                    float reflectivity = material.reflectivity;
                    if (material.ior > 0.0f)
                    {

                        float ior = sign < 0.0f ? material.ior : 1.0f / material.ior;
                        Vec2 refracted;
                        bool can_refract = Utils::refract(Vec2::normalize(direction), Vec2::normalize(normal), ior, refracted);
                        if (can_refract)
                        {
                            // Moves the point to the other medium side
                            Vec2 refracted_origin = point - normal * OFFSET;
                            Color refracted_color = _ray_march(refracted_origin, Vec2::normalize(refracted), depth + 1) * (1.0f - reflectivity);
                            accumulated_color = refracted_color;
                        }
                        else
                        {
                            // Total internal reflection
                            reflectivity = 1.0f;
                        }
                    }
                    if (reflectivity > 0.0f)
                    {
                        Vec2 reflected = Utils::reflect(direction, normal);
                        Vec2 reflected_origin = origin + normal * OFFSET;
                        accumulated_color += _ray_march(reflected_origin, reflected, depth + 1) * reflectivity; 
                    }

                }
                // TODO - I think beer - lambert should be used once, and it is when the light leaves the object
                // If we casted the ray inside the object
                if (nearest.distance < 0.0f)
                    return accumulated_color * Utils::beer_lambert(material.absorption, t);

                return accumulated_color;
            }
            t += unsigned_distance;
        }
        return Color(0.0f);
    
    }

    Color<float> Renderer::_sample(Vec2 uv, uint32_t sample_index)
    {
        Vec2 origin = (uv - 0.5f) * 2.0f;
        origin.x *= config.aspect_ratio;

        float angle = 2.0f * PI * (sample_index + Utils::random()) / config.samples;
        //float angle = 2.0f * PI * sample_index / config.samples;
        //float angle = 2.0f * PI * (Utils::random()  + (sample_index) / config.samples);
        //float angle = 2.0f * PI * Utils::random();
        Vec2 direction(cos(angle), sin(angle));

        Color color = _ray_march(origin, direction);
        return color;
    }

    Vec2 Renderer::gradient(Vec2 p)
    {
        /*
        Since sdf function is a scalar field, we know that the gradient vector
        of sdf is perpendicular to each of the level curves. The normal of the "sdf"
        is the gradient vector at the level curve 0
        */
        
        constexpr float epsilon = 0.0001f;

        float sdf_source = sdf(p, _time).distance;
        return {
            (sdf({p.x + epsilon, p.y}, _time).distance - sdf_source) / epsilon,
            (sdf({p.x, p.y + epsilon}, _time).distance - sdf_source) / epsilon
        };
    }
}