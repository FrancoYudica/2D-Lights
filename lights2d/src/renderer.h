#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "image.h"
#include "vec2.h"
#include "material.h"
#include <random>
#include <memory>
#include <functional>

namespace Lights2D
{
    struct Nearest
    {
        // SDF Scene function returns Nearest data
        float distance;
        Material mtl;
        Nearest() : distance(10e5), mtl() {}
    };


    struct FrameConfig
    {
        uint32_t width, height;                 // Output image size
        uint32_t samples;                       // For coherent results, samples should be an integer power of 2, 2^n
        uint32_t max_recursion_depth;           // Max depth for ray bounces
        uint32_t ray_march_max_iterations;      // If the images produced have "black edges / artifacts" probably increasing this attribute would give better results 
        float aspect_ratio;                     // Should be manually set width / height
        bool sampling_offset;                   // Recommended if smooth edges are desired
    };

    // Function pointer type definition. This function should be given as an argument to the constructor of the renderer
    typedef std::function<Nearest(Vec2, float)> SignedDistanceFunction;

    class Renderer
    {

        public:
            std::shared_ptr<Image> img;
            FrameConfig config;
            std::vector<uint32_t> height_values;
            SignedDistanceFunction sdf;
            bool debug;

        public:
            Renderer(FrameConfig config, SignedDistanceFunction sdf, float time, std::shared_ptr<Image> image)
                :   sdf(sdf),
                    config(config),
                    img(image),
                    _time(time)
                    {

                    height_values.resize(config.height);
                    for (uint32_t y = 0; y < config.height; y++)
                        height_values[y] = y;
                    }

            void render();

        protected:
            Vec2 gradient(Vec2 p);

        private:
            Color<float> _sample(Vec2 uv, uint32_t sample_index);
            Color<float> _ray_march(Vec2 origin, Vec2 direction, uint32_t depth=0);
        private:
            float _time;
    };
}
#endif 