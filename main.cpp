#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "src/renderer.h"
#include "src/sdf_functions.h"


static void _eval(float distance, Material mtl, Nearest& nearest)
{
    if (distance < nearest.distance)
    {
        nearest.distance = distance;
        nearest.mtl = mtl;
    }
}      

static Material mix(const Material& m1, const Material& m2, float t)
{
    return Material(
        mix(m1.emission, m2.emission, t),
        mix(m1.emission_intensity, m2.emission_intensity, t),
        mix(m1.reflectivity, m2.reflectivity, t)
    );
}



Nearest room_sdf(Vec2 pos)
{
    Nearest nearest;
    Material white_mtl(Color<float>(1.0f), 1.0f, 0.0f);
    Material purple_mtl(Color<float>(0.3, 0.0f, .8f, 1.0f), 2.0f, 0.0f);
    Material light_purple_mtl(Color<float>(0.5, 0.2f, 1.0f, 1.0f), 1.0f, 0.0f);
    Material green_mtl(Color<float>(0.7, 1.0f, .2f, 1.0f), 1.5f, 0.0f);
    Material black_mtl(Color<float>(0.0f), 0.0f, 0.5f);

    // Center circle
    _eval(SDF::circle(pos, Vec2(), 0.1f), purple_mtl, nearest);

    // BLACK - REFLECTIVE
    float plane1_dist = SDF::plane(pos, {0, -0.70}, {0, 1});
    float plane2_dist = SDF::plane(pos, {0, 0.70}, {0, -1});
    float box1_dist = SDF::box(pos, {-0.7, 0}, {0.1, 1});
    float box2_dist = SDF::box(pos, {0.7, 0}, {0.1, 1});

    float circle_dist = SDF::circle(pos, Vec2(), 0.8f);
    float planes = SDF::combine_union(plane1_dist, plane2_dist);
    float boxes = SDF::combine_union(box1_dist, box2_dist);
    float subtract = SDF::combine_union(circle_dist, boxes);
    _eval(SDF::combine_subtract(planes, subtract), black_mtl, nearest);

    // LIGHTS
    Vec2 box_size(0.1f, 0.05);
    float box_light1 = SDF::box(pos, {-0.7, -1 + box_size.y}, box_size);
    float box_light2 = SDF::box(pos, {-0.7,  1 - box_size.y}, box_size);
    float box_light3 = SDF::box(pos, {+0.7, -1 + box_size.y}, box_size);
    float box_light4 = SDF::box(pos, {+0.7,  1 - box_size.y}, box_size);
    _eval(box_light1, light_purple_mtl, nearest);
    _eval(box_light2, light_purple_mtl, nearest);
    _eval(box_light3, light_purple_mtl, nearest);
    _eval(box_light4, light_purple_mtl, nearest);
    return nearest;
}

Nearest room2_sdf(Vec2 pos)
{
    Nearest nearest;
    Material white_mtl(Color<float>(1.0f), 1.6f, 0.0f);
    Material purple_mtl(Color<float>(0.3, 0.0f, .8f, 1.0f), 1.3f, 0.9f);
    Material green_mtl(Color<float>(0.7, 1.0f, .2f, 1.0f), 1.5f, 0.0f);

    Material black_mtl(Color<float>(0.0f), 0.0f, 0.7);

    // Central light
    _eval(SDF::circle(pos, Vec2(), 0.1f), white_mtl, nearest);


    float radius = 1.0f;
    float thickness = 0.1f;
    float outer_circle = SDF::circle(pos, Vec2(), radius);
    float inner_circle = SDF::circle(pos, Vec2(), radius - thickness);
    float circular_border = SDF::combine_subtract(outer_circle, inner_circle);

    float line_distances = 1e10;
    for (float i = 0; i < 10; i++)
    {
        float top_angle = PI * (i / 10.0f);
        float bottom_angle = PI + top_angle;

        Vec2 p0(radius * cos(top_angle), radius * sin(top_angle));
        Vec2 p1(radius * cos(bottom_angle), radius * sin(bottom_angle));
        float line_distance = SDF::line(pos, p0, p1, 0.1f);
        line_distances = SDF::combine_union(line_distances, line_distance);

        _eval(SDF::circle(pos, p0, 0.01f), purple_mtl, nearest);
        _eval(SDF::circle(pos, p1, 0.01f), purple_mtl, nearest);
    }

    circular_border = SDF::combine_subtract(circular_border, line_distances);
    _eval(circular_border, black_mtl, nearest);

    return nearest;

}

Nearest rainbow_sdf(Vec2 pos)
{
    Nearest nearest;
    Material black_mtl(Color<float>(0.0f), 0.0f, 0.7);

    Color<float> colors[7];
    colors[0] = Color<float>(148, 0, 211, 255) / 255.0f;
    colors[1] = Color<float>(75, 0, 130, 255) / 255.0f;
    colors[2] = Color<float>(0, 0, 255, 255) / 255.0f;
    colors[3] = Color<float>(0, 255, 0, 255) / 255.0f;
    colors[4] = Color<float>(255, 255, 0, 255) / 255.0f;
    colors[5] = Color<float>(255, 127, 0, 255) / 255.0f;
    colors[6] = Color<float>(255, 0 , 0, 255) / 255.0f;

    float segment_length = 0.7f;
    float box_height = 1.5f;
    float spacing = box_height / 7.0f;
    for (int i = 0; i < 7; i++)
    {
        Color color = colors[i];
        float displacement = i * spacing;
        float y = box_height * 0.5f - displacement;
        Vec2 p0(-0.5f * segment_length, y);
        Vec2 p1(0.5f * segment_length, y);
        float line_distance = SDF::line(pos, p0, p1, 0.1f);
        Material mtl(color, 1.0f, 0.6f);
        _eval(SDF::line(pos, p0, p1, 0.01f), mtl, nearest);
    }
    return nearest;

}


Nearest room3_sdf(Vec2 pos)
{
    Nearest nearest;
    Material white_mtl(Color<float>(1.0f), 1.0f, 0.0f);
    Material purple_mtl(Color<float>(0.3, 0.0f, .8f, 1.0f), 1.3f, 0.9f);
    Material green_mtl(Color<float>(0.7, 1.0f, .2f, 1.0f), 1.5f, 0.9f);
    Material red_mtl(Color<float>(1.0f, 0.0f, 0.3f, 1.0f), 1.5f, 0.9f);

    Material black_mtl(Color<float>(0.0f), 0.0f, 0.7);

    // Central light
    float radius = 0.3f;
    float k_smooth_factor = 0.3f;

    float line = SDF::line(pos, Vec2(-0.5), Vec2(0.0, 0.5f), 0.05f);
    float circle1 = SDF::circle(pos, Vec2(0.3f, 0.0f), radius);
    float objects = SDF::combine_union_s(line, circle1, k_smooth_factor);
    float smooth_lerp = SDF::smooth_t(line, circle1, k_smooth_factor);
    auto interpolated_mtl = mix(purple_mtl, green_mtl, smooth_lerp);
    std::cout << interpolated_mtl.reflectivity;
    
    //interpolated_mtl.reflectivity = 0.0f;
    _eval(objects, interpolated_mtl, nearest);
    return nearest;
}


Nearest scene_sdf(Vec2 pos)
{
    Nearest nearest;
    Material white_mtl(Color<float>(1.0f), 2.0f, 0.0f);
    Material purple_mtl(Color<float>(0.3, 0.0f, .8f, 1.0f), 1.5f, 0.0f);
    Material green_mtl(Color<float>(0.7, 1.0f, .2f, 1.0f), 1.5f, 0.0f);

    Material black_mtl(Color<float>(0.0f), 0.0f, 0.5);
    //_eval(SDF::circle(pos, Vec2(0.3f, 0.6f), 0.2f), white_mtl, nearest);
    //_eval(SDF::box(pos, {-0.4, -0.4}, {0.1, 0.1}), purple_mtl, nearest);
    _eval(SDF::circle(pos, Vec2(), 0.1f), green_mtl, nearest);


    // BLACK - REFLECTIVE
    float plane1_dist = SDF::plane(pos, {0, -0.70}, {0, 1});
    float plane2_dist = SDF::plane(pos, {0, 0.70}, {0, -1});
    float box1_dist = SDF::box(pos, {-0.7, 0}, {0.1, 1});
    float box2_dist = SDF::box(pos, {0.7, 0}, {0.1, 1});

    float circle_dist = SDF::circle(pos, Vec2(), 0.8f);
    float planes = SDF::combine_union(plane1_dist, plane2_dist);
    float boxes = SDF::combine_union(box1_dist, box2_dist);
    float subtract = SDF::combine_union(circle_dist, boxes);
    _eval(SDF::combine_subtract(planes, subtract), black_mtl, nearest);

    // LIGHTS
    Vec2 box_size(0.1, 0.0025);
    float box_light1 = SDF::box(pos, {-0.7, -1 + box_size.y}, box_size);
    float box_light2 = SDF::box(pos, {-0.7,  1 - box_size.y}, box_size);
    float box_light3 = SDF::box(pos, {+0.7, -1 + box_size.y}, box_size);
    float box_light4 = SDF::box(pos, {+0.7,  1 - box_size.y}, box_size);
    _eval(box_light1, white_mtl, nearest);
    _eval(box_light2, white_mtl, nearest);
    _eval(box_light3, white_mtl, nearest);
    _eval(box_light4, white_mtl, nearest);

     // MOON
    //float outer_circle_distance = SDF::circle(pos, {-0.5f, 0.0f}, 0.4);
    //float inner_circle_distance = SDF::circle(pos, {-0.3f, 0.0f}, 0.3);
    //_eval(SDF::combine_subtract(outer_circle_distance, inner_circle_distance), black_mtl, nearest);
    //float line_distance = SDF::line(pos, Vec2(-0.5), Vec2(0.5), 0.07f);
    //_eval(line_distance, white_mtl, nearest);
     //float distance = SDF::triangle(pos, Vec2(-0.3, 0.2), Vec2(0.5, 0.1), Vec2(0.4, 0.5));
    //_eval(distance, white_mtl, nearest);
    //float aperture = PI * 0.35;
    //float distance = SDF::arc(pos - Vec2(0.4, 0.0f), Vec2(sin(aperture), cos(aperture)), 0.3, 0.01);
    //_eval(distance, black_mtl, nearest);
     //float distance = SDF::heart(pos, Vec2(0.0f, -0.5f));
    //_eval(distance, white_mtl, nearest);
     //float distance = SDF::egg(pos, Vec2(0, -0.5), 0.3, 0.01);
    //_eval(distance, white_mtl, nearest);
     //float distance = SDF::pentagon(pos, 0.3f);
    //_eval(distance, white_mtl, nearest);
     return nearest;
}



int main()
{
    //RendererConfig config(512, 512, 1, 4);
    //RendererConfig config(100, 100, 10, 10);
    RendererConfig config(256, 256, 1, 10);
    Renderer renderer(config, room3_sdf);
    renderer.render();
    renderer.img.save("../images/result.jpg");
    return 0;
}