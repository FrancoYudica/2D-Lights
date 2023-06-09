#pragma once
#ifndef FRAME_SEQUENCE_H
#define FRAME_SEQUENCE_H
#include <iostream>
#include "renderer.h"

namespace Lights2D
{
    /*
    Given the folder path, renders a sequence of "continuous" frames, modifying the
    time argument, given in the signed_distance_function. All the configuration is
    done with FrameSequenceConfig struct
    */
    struct FrameSequenceConfig
    {
        RendererConfig frame_config;
        float start_time;
        float time_step;
        uint32_t frame_count;
    };
    void render_sequence(const FrameSequenceConfig& config, const std::string& path, signed_distance_function sdf);
}

#endif  