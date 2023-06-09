#include "frame_sequence.h"

namespace Lights2D
{
    void render_sequence(const FrameSequenceConfig& config, const std::string& path, signed_distance_function sdf)
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

}
