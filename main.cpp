#include <iostream>
#include <execution>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "src/renderer.h"
#include "scenes.h"

struct VideoSequenceConfig
{
    RendererConfig frame_config;
    float start_time;
    float time_step;
    uint32_t frame_count;
};

static void render_sequence(const VideoSequenceConfig& config, const std::string& path, signed_distance_function sdf)
{

    // config is per frame configuration
    for (uint32_t i = 0; i < config.frame_count; i++)
    {
        float current_time = config.start_time + config.time_step * i;
        Renderer frame_renderer(config.frame_config, sdf, current_time);
        frame_renderer.debug = false;
        frame_renderer.render();
        std::cout << "Frame " << i << " rendered" << std::endl;

        frame_renderer.img.save(path + std::to_string(i) + ".jpg");
    }
}


int main()
{

    /*
    
    uint32_t width = 512 / 2;
    uint32_t height = 512 / 2;
    uint32_t samples_per_pixel = 1;
    */

    uint32_t width = 512;
    uint32_t height = 512;
    uint32_t samples_per_pixel = 2;
    uint32_t ray_tracing_depth = 6;
    uint32_t ray_marching_iterations = 64;
    RendererConfig frame_config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        static_cast<float>(width) / height // Aspect ratio
    };

    VideoSequenceConfig config = {frame_config, 0.0f, 1.0f / 120.0f, 120};
    render_sequence(config, "../../video/shape_interpolation/seq/", Scenes::shape_interpolation);

   return 0;
}