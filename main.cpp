#include "lights2d/lights2d.h"
#include "scenes.h"
#include <filesystem>
#include <iostream>
#include <string>

// STB specific required definitions and include
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace Lights2D;

static uint32_t current_image_index = 0;

static void render_frame_callback(std::shared_ptr<Image> image_buffer,
    uint32_t frame_index)
{
    std::cout << "Frame " << frame_index << " rendered" << std::endl;

    std::string filepath = PROJECT_DIRECTORY_PATH;
    filepath += "/renders/";

    if (!std::filesystem::exists(filepath)) {
        std::filesystem::create_directory(filepath);
    }

    filepath += std::to_string(current_image_index++ + frame_index);
    filepath += ".png";

    bool status = stbi_write_png(
        filepath.c_str(),
        image_buffer->width,
        image_buffer->height, 3,
        &image_buffer->buffer[0],
        image_buffer->width * 3);
}

void parse_arguments(int argc, char* argv[],
    uint32_t& width,
    uint32_t& height,
    uint32_t& samples_per_pixel,
    uint32_t& ray_tracing_depth,
    uint32_t& ray_marching_iterations)
{
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        auto getValue = [&](uint32_t& out) {
            if (i + 1 < argc) {
                out = static_cast<uint32_t>(std::stoi(argv[++i]));
            } else {
                std::cerr << "Missing value after " << arg << std::endl;
                std::exit(EXIT_FAILURE);
            }
        };

        if (arg == "--width")
            getValue(width);
        else if (arg == "--height")
            getValue(height);
        else if (arg == "--samples")
            getValue(samples_per_pixel);
        else if (arg == "--depth")
            getValue(ray_tracing_depth);
        else if (arg == "--iterations")
            getValue(ray_marching_iterations);
        else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char** argv)
{
    // Creates FrameConfiguration
    uint32_t width = 128;
    uint32_t height = 128;
    uint32_t samples_per_pixel = 128;
    uint32_t ray_tracing_depth = 6;
    uint32_t ray_marching_iterations = 64;

    // Parse command-line arguments
    parse_arguments(
        argc,
        argv,
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations);

    // Print final configuration
    std::cout << "Configuration:\n"
              << "Width: " << width << "\n"
              << "Height: " << height << "\n"
              << "Samples per pixel: " << samples_per_pixel << "\n"
              << "Ray tracing depth: " << ray_tracing_depth << "\n"
              << "Ray marching iterations: " << ray_marching_iterations << "\n";

    FrameConfig frame_config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        true // Enables sampling offset
    };
    SequenceConfig sequence_config = { 0.0f, 0.5f, 1.0f };
    render_sequence(frame_config, sequence_config, Scenes::rainbow_sdf, render_frame_callback);

    return 0;
}