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
    render_sequence allows the user to render sequences of images. The only propose of the
    function is to modify the time argument passed in the sdf function.
    The configuration is specified by SequenceConfig struct
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