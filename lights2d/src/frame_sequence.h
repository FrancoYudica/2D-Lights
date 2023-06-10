#pragma once
#ifndef FRAME_SEQUENCE_H
#define FRAME_SEQUENCE_H
#include <iostream>
#include "renderer.h"
#include <functional>



namespace Lights2D
{
    typedef std::function<void(std::shared_ptr<Image>, uint32_t)> FrameRenderCallback;

    /*
    Given the folder path, renders a sequence of "continuous" frames, modifying the
    time argument, given in the signed_distance_function. All the configuration is
    done with FrameSequenceConfig struct
    */
    struct SequenceConfig
    {
        float start_time;
        float end_time;
        float frames_per_second;
    };
    void render_sequence(
        const FrameConfig& frame_config,
        const SequenceConfig& sequence_config,
        SignedDistanceFunction sdf,
        FrameRenderCallback on_render_callback
    );
}

#endif  