#include "frame_sequence.h"

namespace Lights2D {
void render_sequence(const FrameConfig& frame_config,
    const SequenceConfig& sequence_config,
    SignedDistanceFunction sdf,
    FrameRenderCallback on_render_callback)
{

    std::shared_ptr<Image> image_buffer = std::make_shared<Image>(frame_config.width, frame_config.height);

    float current_time = sequence_config.start_time;
    float delta_time = 1.0f / sequence_config.frames_per_second;
    uint32_t frame_index = 0;

    // Renders frames
    while (current_time <= sequence_config.end_time) {

        Renderer frame_renderer(frame_config, sdf, current_time, image_buffer);
        frame_renderer.debug = false;

        frame_renderer.render();

        on_render_callback(image_buffer, frame_index++);

        // Clears the buffer, since it's used in the next frame
        image_buffer->clear();

        current_time += delta_time;
    }
}

} // namespace Lights2D
