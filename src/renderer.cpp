#include <iostream>
#include "renderer.h"
#include "utils.h"
#include "sdf_functions.h"
#include <execution>

#define MARCH_HIT_DIST 1e-4
#define OFFSET 1e-3

void Renderer::render()
{
    uint32_t sample_size = static_cast<uint32_t>(std::sqrt(config.samples));

    std::for_each(
        std::execution::par,
        height_values.begin(),
        height_values.end(),
        [sample_size, this](uint32_t y)
        {
            Utils::random_seed(y);
            for (uint32_t x = 0; x < img.width; x++)
            {
                
                Color<float> accumulated;
                Vec2 uv(
                    static_cast<float>(x) / img.width,
                    1.0f - static_cast<float>(y) / img.height
                );

                // Multi sampling - Samples a grid
                int sample_index = 0;
                for (uint32_t x_sample = 0; x_sample < sample_size; x_sample++)
                {
                    for (uint32_t y_sample = 0; y_sample < sample_size; y_sample++)
                    {
                        Vec2 offset(x_sample, y_sample);
                        offset.x /= sample_size;
                        offset.y /= sample_size;
                        offset = offset - 0.5f;
                        offset.x /= img.width;
                        offset.y /= img.height;
                        offset *= config.sampling_offset;
                        accumulated += _sample(uv + offset, sample_index++);            
                    }
                }

                accumulated /= config.samples;
                Color<uint8_t> byte_color = (Color<uint8_t>)Color<float>::clamp(accumulated, 0, 1.0f);
                img.set_pixel(x, y, byte_color);
            }

            if (debug)
                std::cout << "Calculating: " << 100.0f * y / (img.height - 1.0f) << "%"<< std::endl;

        }
    );  
}

Color<float> Renderer::_ray_march(Vec2 origin, Vec2 direction, uint32_t depth)
{
    for (uint32_t i = 0; i < config.ray_march_max_iterations; i++)
    {
        Nearest nearest = sdf(origin, _time);

        float sign = nearest.distance > 0.0f ? 1.0f : -1.0f;

        Material& material = nearest.mtl;
        if (sign * nearest.distance < MARCH_HIT_DIST)
        {
            // Adds the intensity
            Color color = material.emission * material.emission_intensity;

            // Check for reflection and refraction
            if (depth < config.max_recursion_depth && (material.reflectivity > 0.0f || material.ior > 0.0f))
            {
                Vec2 normal = this->gradient(origin);

                // In case we are inside the object, flipping the normal gives us the correct result
                normal *= sign;

                float reflectivity = material.reflectivity;
                if (material.ior > 0.0f)
                {

                    float ior = sign < 0.0f ? material.ior : 1.0f / material.ior;
                    Vec2 refracted;
                    if (refract(direction, normal, ior, refracted))
                    {
                        // Moves the point to the other medium side
                        Vec2 refracted_origin = origin - normal * OFFSET;
                        color += _ray_march(refracted_origin, refracted, depth + 1) * (1.0f - reflectivity);
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
                    color += _ray_march(reflected_origin, reflected, depth + 1) * reflectivity; 
                }
                
            }
            return color;
        }
        origin += direction * (nearest.distance * sign);
    }
    return Color(0.0f);
  
}

Color<float> Renderer::_sample(Vec2 uv, uint32_t sample_index)
{
    Vec2 origin = (uv - 0.5f) * 2.0f;
    origin.x *= config.aspect_ratio;

    float angle = 2.0f * PI * (sample_index + Utils::random()) / config.samples;
    Vec2 direction(cos(angle), sin(angle));
    
    Color color = _ray_march(origin, direction);
    return color;
}

Vec2 Renderer::gradient(Vec2 p)
{
    constexpr float epsilon = 0.0001f;

    float sdf_source = sdf(p, _time).distance;
    return {
        (sdf({p.x + epsilon, p.y}, _time).distance - sdf_source) / epsilon,
        (sdf({p.x, p.y + epsilon}, _time).distance - sdf_source) / epsilon
    };
}