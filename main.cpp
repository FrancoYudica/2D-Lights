#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "src/renderer.h"
#include "src/sdf_functions.h"

class TestScene : public Scene
{
    public:
        Nearest sdf(Vec2 pos) const override
        {
            Nearest nearest;

            Material white_mtl(Color<float>(1.0f), 2.0f, 0.0f);
            Material black_mtl(Color<float>(0.0f), 0.0f, 0.9);

            _eval(SDF::circle(pos, Vec2(0.3f, 0.6f), 0.2f), white_mtl, nearest);
            //_eval(SDF::box(pos, {-0.4, 0.4}, {0.1, 0.1}), black_mtl, nearest);
            //_eval(SDF::box(pos, {0.8f, 0.5f}, {0.1, 0.1}), black_mtl, nearest);

            float plane_dist = SDF::plane(pos, {0, -0.1}, {0, 1});
            float circle_dist = SDF::circle(pos, {0, -0.1}, 0.8f);
            _eval(SDF::combine_subtract(plane_dist, circle_dist), black_mtl, nearest);

            // MOON
            //float outer_circle_distance = SDF::circle(pos, {-0.5f, 0.0f}, 0.4);
            //float inner_circle_distance = SDF::circle(pos, {-0.3f, 0.0f}, 0.3);
            //_eval(SDF::combine_subtract(outer_circle_distance, inner_circle_distance), black_mtl, nearest);
            //float line_distance = SDF::line(pos, Vec2(-0.5), Vec2(0.5), 0.07f);
            //_eval(line_distance, white_mtl, nearest);

            //float distance = SDF::triangle(pos, Vec2(-0.3, 0.2), Vec2(0.5, 0.1), Vec2(0.4, 0.5));
            //_eval(distance, white_mtl, nearest);

            //float aperture = PI * 0.75;
            //float distance = SDF::arc(pos, Vec2(sin(aperture), cos(aperture)), 0.3, 0.01);
            //_eval(distance, white_mtl, nearest);

            //float distance = SDF::heart(pos, Vec2(0.0f, -0.5f));
            //_eval(distance, white_mtl, nearest);

            //float distance = SDF::egg(pos, Vec2(0, -0.5), 0.3, 0.01);
            //_eval(distance, white_mtl, nearest);

            //float distance = SDF::pentagon(pos, 0.3f);
            //_eval(distance, white_mtl, nearest);

            return nearest;
        }

    protected:
        void _eval(float distance, Material mtl, Nearest& nearest) const
        {
            if (distance < nearest.distance)
            {
                nearest.distance = distance;
                nearest.mtl = mtl;
            }
        }        
};

int main()
{
    TestScene scene;
    RendererConfig config(512, 512, 128, 10);
    Renderer renderer(&scene, config);
    renderer.render();
    renderer.img.save("../images/result.jpg");
    return 0;
}