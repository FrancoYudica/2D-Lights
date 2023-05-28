#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "image.h"
#include "vec2.h"
#include <random>


struct Material
{
    Color<float> emission;
    float emission_intensity;
    float reflectivity;

    Material() : emission(0.0f), emission_intensity(0.0f) {}
    Material(Color<float> emission) : emission(emission), emission_intensity(1.0f) {}
    Material(Color<float> emission, float intensity) : emission(emission), emission_intensity(intensity) {}
    Material(Color<float> emission, float intensity, float reflectivity)
     : emission(emission), emission_intensity(intensity), reflectivity(reflectivity) {}
};

struct Nearest
{
    float distance;
    Material mtl;
    Nearest() : distance(10e5), mtl() {}
};


class RendererConfig
{
    public:
        uint32_t width, height;
        uint32_t samples;
        uint32_t max_recursion_depth;
        float aspect_ratio;
    
    public:
        RendererConfig(uint32_t width, uint32_t height, uint32_t samples, uint32_t recursion_depth) 
        : width(width), height(height), samples(samples), max_recursion_depth(recursion_depth)
        {
            aspect_ratio = static_cast<float>(width) / height;
        }

};

typedef Nearest(*signed_distance_function)(Vec2);

class Renderer
{

    public:
        Image img;
        RendererConfig config;
        std::vector<uint32_t> height_values;
        signed_distance_function sdf;

    public:
        Renderer(RendererConfig config, signed_distance_function sdf)
            :   sdf(sdf),
                config(config),
                img(Image(config.width, config.height))
                {

                height_values.resize(img.height);
                for (uint32_t y = 0; y < img.height; y++)
                    height_values[y] = y;
                }

        void render();

    protected:
        Vec2 normal(Vec2 p);

    private:
        Color<float> _sample(Vec2 uv, uint32_t sample_index);
        Color<float> _ray_march(Vec2 origin, Vec2 direction, uint32_t depth=0);
};


#endif 