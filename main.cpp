#include <iostream>
#include <execution>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "scenes.h"
#include "lights2d/lights2d.h"

using namespace Lights2D;    


int main()
{
    uint32_t width = 512;
    uint32_t height = 512;
    uint32_t samples_per_pixel = 13;
    uint32_t ray_tracing_depth = 6;
    uint32_t ray_marching_iterations = 128;
    RendererConfig frame_config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        static_cast<float>(width) / height, // Aspect ratio
        false // Enables sampling offset
    };

    FrameSequenceConfig config = {frame_config, 0.0f, 1.0f / 120.0f, 120};
    render_sequence(config, "../../renders/images/", Scenes::concave_lens);

   return 0;
}