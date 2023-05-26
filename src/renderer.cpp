#include "renderer.h"
#include <iostream>
#include <iostream>

void Renderer::render()
{
    for (uint32_t y = 0; y < img.height; y++)
    {
        for (uint32_t x = 0; x < img.width; x++)
        {
            
            Color<float> accumulated;
            Vec2 uv(static_cast<float>(x) / img.width, 1.0f - static_cast<float>(y) / img.height);

            for (uint32_t sample = 0; sample < config.samples; sample++)
                accumulated += _sample(uv);            

            accumulated *= 1.0f / config.samples;
            
            Color<uint8_t> byte_color = (Color<uint8_t>)Color<float>::clamp(accumulated, 0, 1.0f);

            img.set_pixel(x, y, byte_color);
        }
        std::cout << "Calculating: " << 100.0f * y / (img.height - 1.0f) << "%"<< std::endl;
    }
}

Nearest Renderer::_ray_march(Vec2 origin, Vec2 direction)
{
    for (uint32_t i = 0; i < 10; i++)
    {
        Nearest nearest = scene->sdf(origin);

        if (nearest.distance < 0.01f)
        {
            return nearest;
        }
        origin += direction * nearest.distance;
    }

    return Nearest();
}

Color<float> Renderer::_sample(Vec2 uv)
{
    Vec2 origin = (uv - 0.5f) * 2.0f;
    origin.x *= config.aspect_ratio;

    Vec2 direction = Vec2::normalize(Vec2::random());
    Nearest nearest = _ray_march(origin, direction);
    
    return nearest.mtl.emission;
}