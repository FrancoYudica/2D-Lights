#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "src/renderer.h"
#include "src/sdf_functions.h"

class TestScene : public Scene
{
    public:
        Nearest sdf(Vec2 pos) override
        {
            Nearest nearest;
            float circle_distance = SDF::circle(pos, Vec2(0.0f), 0.3f);
            nearest.distance = circle_distance;
            nearest.mtl.emission = {1.0f, 1.0f, 0.0f, 1.0f};
            return nearest;
        }
};


int main()
{
    TestScene scene;
    RendererConfig config(1920, 1080, 10);
    Renderer renderer(&scene, config);
    renderer.render();
    renderer.img.save("../images/result.jpg");
    return 0;
}