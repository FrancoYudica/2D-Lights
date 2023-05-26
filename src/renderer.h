#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "image.h"
#include "scene.h"
#include "vec2.h"
#include <random>


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

class Renderer
{

    public:
        Scene* scene;
        Image img;
        RendererConfig config;
        std::vector<uint32_t> height_values;

    public:
        Renderer(Scene* scene, RendererConfig config)
            :   scene(scene),
                config(config),
                img(Image(config.width, config.height))
                {

                height_values.reserve(img.height);
                for (uint32_t y = 0; y < img.height; y++)
                    height_values.push_back(y);
                }

        void render();

    private:
        Color<float> _sample(Vec2 uv, uint32_t sample_index);
        Color<float> _ray_march(Vec2 origin, Vec2 direction, uint32_t depth=0);
};


#endif 