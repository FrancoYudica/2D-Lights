#include <iostream>
#include "renderer.h"
#include "utils.h"
#include "sdf_functions.h"
#include <execution>

void Renderer::render()
{

#if 0
    std::for_each(
        std::execution::par,
        height_values.begin(),
        height_values.end(),
        [this](uint32_t y)
        {
            for (uint32_t x = 0; x < img.width; x++)
            {
                
                Color<float> accumulated;
                Vec2 uv(
                    static_cast<float>(x) / img.width,
                    1.0f - static_cast<float>(y) / img.height
                );

                // Multi sampling
                for (uint32_t sample = 0; sample < config.samples; sample++)
                {
                    
                    Vec2 offset = Utils::random_norm_vec2();
                    offset.x *= 0.25f / img.width;
                    offset.y *= 0.25f / img.height;
                    accumulated += _sample(uv + offset, sample);            
                }
                accumulated /= config.samples;
                
                Color<uint8_t> byte_color = (Color<uint8_t>)Color<float>::clamp(accumulated, 0, 1.0f);
                img.set_pixel(x, y, byte_color);
            }
        }
    );  

#else
    for (uint32_t y = 0; y < img.height; y++)
    {
        for (uint32_t x = 0; x < img.width; x++)
        {
            
            Color<float> accumulated;
            Vec2 uv(
                static_cast<float>(x) / img.width,
                1.0f - static_cast<float>(y) / img.height
            );

            // Multi sampling
            for (uint32_t sample = 0; sample < config.samples; sample++)
            {
                
                Vec2 offset = Utils::random_norm_vec2();
                offset.x *= 0.25f / img.width;
                offset.y *= 0.25f / img.height;
                accumulated += _sample(uv + offset, sample);            
            }
            accumulated /= config.samples;
            
            Color<uint8_t> byte_color = (Color<uint8_t>)Color<float>::clamp(accumulated, 0, 1.0f);
            img.set_pixel(x, y, byte_color);
        }
        std::cout << "Calculating: " << 100.0f * y / (img.height - 1.0f) << "%"<< std::endl;
    }
#endif
}

Color<float> Renderer::_ray_march(Vec2 origin, Vec2 direction, uint32_t depth)
{

    for (uint32_t i = 0; i < 64; i++)
    {
        Nearest nearest = sdf(origin);
        Material& material = nearest.mtl;
        if (nearest.distance < 1e-4f)
        {
            // Adds the intensity
            Color color = material.emission * material.emission_intensity;

            // If the material is reflective
            if (depth < config.max_recursion_depth && material.reflectivity > 0.0f)
            {

                if (nearest.distance < 0.0f)
                    return color;

                constexpr float OFFSET = 1e-3f;
                Vec2 normal = this->normal(origin);
                Vec2 reflected = Utils::reflect(direction, normal);
                Vec2 reflected_origin = reflected * OFFSET + origin;

                Color reflected_color = _ray_march(reflected_origin, reflected, depth + 1);
                color += reflected_color * material.reflectivity; 
            }
            return color;
        }
        origin += direction * nearest.distance;
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

Vec2 Renderer::normal(Vec2 p)
{
    constexpr float epsilon = 0.001f;
    return {
        (sdf({p.x + epsilon, p.y}).distance - sdf({p.x - epsilon, p.y}).distance) * 0.5f / epsilon,
        (sdf({p.x, p.y + epsilon}).distance - sdf({p.x, p.y - epsilon}).distance) * 0.5f / epsilon
    };
}