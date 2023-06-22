#include <iostream>
#include <string>

#include "scenes.h"
#include "lights2d/lights2d.h"

// STB specific required definitions and include
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


using namespace Lights2D;    

static uint32_t current_image_index = 0;

static void render_frame_callback(std::shared_ptr<Image> image_buffer, uint32_t frame_index)
{
    std::cout << "Frame " << frame_index << " rendered" << std::endl;

    std::string filepath = PROJECT_DIRECTORY_PATH;
    filepath += "/renders/";
    filepath += std::to_string(current_image_index++ + frame_index);
    filepath += ".png";
    bool status = stbi_write_png(
        filepath.c_str(),
        image_buffer->width,
        image_buffer->height,
        3,
        &image_buffer->buffer[0], image_buffer->width * 3
    );
}


int main()
{
    // Creates FrameConfiguration
    uint32_t width = 512;
    uint32_t height = 512;
    uint32_t samples_per_pixel = 512;
    uint32_t ray_tracing_depth = 6;
    uint32_t ray_marching_iterations = 64;
    FrameConfig frame_config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        true // Enables sampling offset
    };

    SequenceConfig sequence_config = {0.0f, 0.5f, 1.0f};
    //render_sequence(frame_config, sequence_config, Scenes::convex_lens, render_frame_callback);
    render_sequence(frame_config, sequence_config, Scenes::semicircular_lens, render_frame_callback);
    /*
    render_sequence(frame_config, sequence_config, Scenes::concave_lens, render_frame_callback);
    render_sequence(frame_config, sequence_config, Scenes::circular_lens, render_frame_callback);
    render_sequence(frame_config, sequence_config, Scenes::glass_metaballs, render_frame_callback);
    render_sequence(frame_config, sequence_config, Scenes::metaballs_absorption, render_frame_callback);
    render_sequence(frame_config, sequence_config, Scenes::glass_absorption, render_frame_callback);
    */
    return 0;
}