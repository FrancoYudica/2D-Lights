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
        float aspect_ratio;
    
    public:
        RendererConfig(uint32_t width, uint32_t height, uint32_t samples) 
        : width(width), height(height), samples(samples)
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

    public:
        Renderer(Scene* scene, RendererConfig config)
            :   scene(scene),
                config(config),
                img(Image(config.width, config.height))
                {}

        void render();

    private:
        Color<float> _sample(Vec2 uv);
        Nearest _ray_march(Vec2 origin, Vec2 direction);
};


#endif 