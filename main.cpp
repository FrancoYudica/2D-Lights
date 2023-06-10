#include <iostream>
#include <string>

#include "scenes.h"
#include "lights2d/lights2d.h"

// STB specific required definitions and include
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


using namespace Lights2D;    

static void render_frame_callback(std::shared_ptr<Image> image_buffer, uint32_t frame_index)
{
    std::cout << "Frame " << frame_index << " rendered" << std::endl;

    std::string filepath = std::string("../../renders/images/") + std::to_string(frame_index) + std::string(".jpg");
    stbi_write_jpg(
        filepath.c_str(),
        image_buffer->width,
        image_buffer->height,
        4,
        &image_buffer->buffer[0], image_buffer->width * sizeof(int)
    );
}


int main()
{

    // Creates FrameConfiguration
    uint32_t width = 512;
    uint32_t height = 512;
    uint32_t samples_per_pixel = 13;
    uint32_t ray_tracing_depth = 6;
    uint32_t ray_marching_iterations = 128;
    FrameConfig frame_config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        static_cast<float>(width) / height, // Aspect ratio
        false // Enables sampling offset
    };

    SequenceConfig sequence_config = {0.0f, 1.0f, 10.0f};

    render_sequence(frame_config, sequence_config, Scenes::metaballs, render_frame_callback);
    return 0;
}