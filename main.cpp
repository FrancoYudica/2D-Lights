#include <iostream>
#include <execution>

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

Nearest room_sdf(Vec2 pos, float time)
{
    Nearest nearest;

    Material white_mtl = Material::create_light({1.0f}, 2.0f);
    Material black_mtl = Material::create_reflective(0.5f);
    Material purple_mtl = Material::create_light(Color(0.3f, 0.0f, .8f, 1.0f), 1.3f);
    Material light_purple_mtl = Material::create_light(Color(0.5f, 0.2f, 1.0f, 1.0f), 1.3f);
    Material green_mtl = Material::create_light(Color(0.7f, 1.0f, .2f, 1.0f), 1.3f);

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

Nearest room2_sdf(Vec2 pos, float time)
{
    Nearest nearest;
    Material white_mtl = Material::create_light({1.0f}, 2.0f);
    Material black_mtl = Material::create_reflective(0.5f);
    Material purple_mtl = Material::create_light(Color(0.3f, 0.0f, .8f, 1.0f), 1.3f);
    Material green_mtl = Material::create_light(Color(0.7f, 1.0f, .2f, 1.0f), 1.3f);

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

Nearest rainbow_sdf(Vec2 pos, float time)
{
    Nearest nearest;
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
        Material mtl = Material::create_light(color, 1.0f);
        _eval(SDF::line(pos, p0, p1, 0.01f), mtl, nearest);
    }
    return nearest;

}


Nearest smooth_intensity_interpolation(Vec2 pos, float time)
{
    Nearest nearest;
    Material purple_light = Material::create_light(Color(0.3f, 0.0f, .8f, 1.0f), 2.0f);
    Material purple_light_darker = Material::create_light(Color(0.3f, 0.0f, .8f, 1.0f), 0.7f);
    Material green_light = Material::create_light(Color(0.7f, 1.0f, .2f, 1.0f), 1.3f);

    // Central light
    float radius = 0.3f;
    float k_smooth_factor = 0.3f;

    float line = SDF::line(pos, Vec2(-0.5), Vec2(0.0, 0.5f), 0.05f);
    float circle1 = SDF::circle(pos, Vec2(0.3f, 0.0f), radius);

    float h = SDF::smooth_t(line, circle1, k_smooth_factor);
    float objects = SDF::combine_union_s(line, circle1, k_smooth_factor, h);
    auto interpolated_mtl = Material::mix(purple_light, purple_light_darker, h);

    _eval(objects, interpolated_mtl, nearest);

    return nearest;
}


Nearest smooth_color_interpolation(Vec2 pos, float time)
{
    Nearest nearest;
    Material purple_light = Material::create_light(Color(0.3f, 0.0f, .8f, 1.0f), 1.2f);
    Material red_light = Material::create_light(Color(1.0f, 0.0f, 0.443f, 1.0f), 1.2f);

    float k_smooth_factor = 0.3f;

    float line_x = SDF::line(pos, Vec2(-0.5, 0.0f), Vec2(0.5f, 0.0f), 0.05f);
    float line_y = SDF::line(pos, Vec2(0.0f, -0.5f), Vec2(0.0f, 0.5f), 0.05f);

    float h = SDF::smooth_t(line_x, line_y, k_smooth_factor);
    float objects = SDF::combine_union_s(line_x, line_y, k_smooth_factor, h);
    auto interpolated_mtl = Material::mix(purple_light, red_light, h);

    _eval(objects, interpolated_mtl, nearest);

    return nearest;
}

Nearest smooth_reflections(Vec2 pos, float time)
{
    Nearest nearest;
    Material white_light = Material::create_light(Color(1.0f), 2.0f);
    Material reflective_walls = Material::create_reflective(0.9f);

    // Central light
    _eval(SDF::circle(pos, Vec2(0.3f), 0.1f), white_light, nearest);
    _eval(SDF::circle(pos, Vec2(-0.9f), 0.05f), white_light, nearest);

    float k_smooth_factor = 0.4f;

    // Calculates the distances
    float box = SDF::box(pos, Vec2(-0.5f, 0.3f), Vec2(0.15f));
    float line_x = SDF::line(pos, Vec2(-0.5f), Vec2(0.5f, -0.5f), 0.05f);
    float line_y = SDF::line(pos, Vec2(-0.5f, -0.5f), Vec2(-0.5f, 0.5f), 0.05f);

    // Interpolates just the distances, since the material is the same
    float lines = SDF::combine_union_s(line_x, line_y, k_smooth_factor);
    float objects = SDF::combine_union_s(lines, box, k_smooth_factor);

    _eval(objects, reflective_walls, nearest);
    return nearest;
}



Nearest multiple_interpolations_0(Vec2 pos, float time)
{
    Nearest nearest;
    Material mat0 = Material::create_light(Color<float>(56, 192, 242, 255) / 255.0f, 1.0f);
    Material mat1 = Material::create_light(Color<float>(58, 10, 200, 255) / 255.0f, 1.0f);
    Material mat2 = Material::create_light(Color<float>(255, 10, 255, 255) / 255.0f, 1.0f);
    
    // Central light
    float obj0 = SDF::circle(pos, Vec2(0.2f), 0.2f);
    float obj1 = SDF::circle(pos, Vec2(-0.2f), 0.2f);
    float obj2 = SDF::circle(pos, Vec2(0.1f, -0.2), 0.15f);

    float k_smooth_factor = 0.2f;

    float h;
    float dist;
    Material mtl;

    h = SDF::smooth_t(obj0, obj1, k_smooth_factor);
    mtl = Material::mix(mat1, mat0, h);
    dist = SDF::combine_union_s(obj0, obj1, k_smooth_factor, h);


    h = SDF::smooth_t(dist, obj2, k_smooth_factor);
    mtl = Material::mix(mat2, mtl, h);
    dist = SDF::combine_union_s(dist, obj2, k_smooth_factor, h);

    _eval(dist, mtl, nearest);
    return nearest;
}


Nearest multiple_interpolations(Vec2 pos, float time)
{
    Nearest nearest;
    Material mat0 = Material::create_light(Color<float>(56, 192, 242, 95) / 255.0f, 1.0f);
    Material mat1 = Material::create_light(Color<float>(58, 252, 122, 99) / 255.0f, 1.0f);
    Material mat2 = Material::create_light(Color<float>(194, 230, 64, 90) / 255.0f, 1.0f);
    Material mat3 = Material::create_light(Color<float>(252, 198, 58, 99) / 255.0f, 1.0f);
    Material mat4 = Material::create_light(Color<float>(245, 115, 64, 96) / 255.0f, 1.0f);
    
    // Central light
    float obj0 = SDF::circle(pos, Vec2(0.0f, 0.6f), 0.2f);
    float obj1 = SDF::circle(pos, Vec2(-0.4f, -0.6f), 0.2f);
    float obj2 = SDF::circle(pos, Vec2(0.2f, -0.1), 0.2f);
    float obj3 = SDF::box(pos, Vec2(-0.4, 0.3), Vec2(0.1f));
    float obj4 = SDF::circle(pos, Vec2(-0.3f, -0.2), 0.1f);

    std::pair<float, Material> objects[5];
    objects[0] = {obj0, mat0};
    objects[1] = {obj1, mat1};
    objects[2] = {obj2, mat2};
    objects[3] = {obj3, mat3};
    objects[4] = {obj4, mat4};

    float k_smooth_factor = 0.4f;

    Material interpolated_mtl = Material::create_opaque();
    float interpolated_sd = 0;
    for (uint32_t i = 0; i < 5; i++)
    {
        auto pair = objects[i];
        if (i == 0)
        {
            interpolated_mtl = pair.second;
            interpolated_sd = pair.first;
        }
        else
        {
            // Interpolation factor
            float h = SDF::smooth_t(interpolated_sd, pair.first, k_smooth_factor);

            // Interpolated distance
            interpolated_sd = SDF::combine_union_s(interpolated_sd, pair.first, k_smooth_factor, h);

            // Interpolated color
            interpolated_mtl = Material::mix(pair.second, interpolated_mtl, h);
        }
    }
    _eval(interpolated_sd, interpolated_mtl, nearest);
    return nearest;
}


Nearest cup_scene(Vec2 pos, float time)
{
    Nearest nearest;
    Material white_light = Material::create_light({1.0f}, 1.0f);
    Material mat0 = Material::create_light(Color<float>(250, 9, 71, 95) / 255.0f, 0.6f);
    Material mat1 = Material::create_light(Color<float>(255, 0, 247, 99) / 255.0f, 1.2f);
    Material mat4 = Material::create_light(Color<float>(245, 115, 64, 96) / 255.0f, 1.2f);
    
    // Central light
    float moon = SDF::combine_subtract(
        SDF::circle(pos, Vec2(0.0f, 0.4f), 0.6f),
        SDF::circle(pos, Vec2(0.0f, 0.8f), 0.5f)
    );
    float body = SDF::box(pos, Vec2(0.0f), Vec2(0.25f, 0.3f));
    float mouth = SDF::box(pos, Vec2(0.0f, -0.5f), Vec2(0.25f, 0.07f));

    float eye1 = SDF::circle(pos, Vec2(-0.15f, 0.0f), 0.1f);
    float eye2 = SDF::circle(pos, Vec2(0.15f, 0.0f), 0.1f);
    float eyes = SDF::combine_union_s(eye1, eye2, 0.26f);
    
    _eval(eyes, white_light, nearest);
    float skull = SDF::combine_union_s(moon, SDF::combine_union(body, mouth), 0.1f);
    _eval(SDF::combine_subtract(skull, eyes), mat0, nearest);
    return nearest;
}


Nearest test_sequence_sdf(Vec2 pos, float time)
{
    Nearest nearest;
    Material white_light = Material::create_light({1.0f}, 1.0f);
    Material mat0 = Material::create_light(Color<float>(250, 9, 71, 95) / 255.0f, 1.2f);
    Material mat1 = Material::create_light(Color<float>(100, 0, 247, 99) / 255.0f, 1.2f);
    
    float angle1 = PI * time;
    float angle2 = 2.0f * PI * time;
    float length = 0.6f;
    Vec2 end1(length * cos(angle1), length * sin(angle1));
    Vec2 end2(length * cos(angle2), length * sin(angle2));

    float line1 = SDF::line(pos, end1, Vec2::flip(end1), 0.06f);
    float line2 = SDF::line(pos, end2, Vec2::flip(end2), 0.06f);

    float k = 0.2f;
    float h = SDF::smooth_t(line1, line2, k);
    float lines = SDF::combine_union_s(line1, line2, k, h);
    Material mtl = Material::mix(mat1, mat0, h);
    _eval(lines, mtl, nearest);
    return nearest;
}



Nearest scene_sdf(Vec2 pos, float time)
{
    Nearest nearest;
    Material white_mtl = Material::create_light({1.0f}, 2.0f);
    Material black_mtl = Material::create_reflective(0.5f);
    Material purple_mtl = Material::create_light(Color(0.3f, 0.0f, .8f, 1.0f), 1.3f);
    Material green_mtl = Material::create_light(Color(0.7f, 1.0f, .2f, 1.0f), 1.3f);
    
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


struct VideoSequenceConfig
{
    RendererConfig frame_config;
    float start_time;
    float time_step;
    uint32_t frame_count;
};

static void render_sequence(const VideoSequenceConfig& config, const std::string& path, signed_distance_function sdf)
{
#if 1
    std::vector<uint32_t> renderers;
    for (uint32_t i = 0; i < config.frame_count; i++)
        renderers.emplace_back(i);
    
    std::for_each(
        std::execution::par_unseq,
        renderers.begin(),
        renderers.end(),
        [config, path, sdf](uint32_t i)
        {
                    
            float current_time = config.start_time + config.time_step * i;
            Renderer frame_renderer(config.frame_config, sdf, current_time);
            frame_renderer.debug = false;
            frame_renderer.render();
            std::cout << "Frame " << i << " rendered" << std::endl;
            frame_renderer.img.save(path + std::to_string(i) + ".jpg");
        }
    );  
#else
    // config is per frame configuration
    for (uint32_t i = 0; i < config.frame_count; i++)
    {
        float current_time = config.start_time + config.time_step * i;
        Renderer frame_renderer(config.frame_config, sdf, current_time);
        frame_renderer.render();
        std::cout << "Frame " << i << " rendered" << std::endl;

        frame_renderer.img.save(path + std::to_string(i) + ".jpg");
    }
#endif
}


int main()
{
    /*
    uint32_t width = 256;
    uint32_t height = 256;
    uint32_t samples_per_pixel = 4;
    uint32_t ray_tracing_depth = 10;
    uint32_t ray_marching_iterations = 64;
    RendererConfig config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        static_cast<float>(width) / height // Aspect ratio
    };
    Renderer renderer(config, cup_scene, 0.0f);
    renderer.render();
    renderer.img.save("../images/result.jpg");
    return 0;
    */
    uint32_t width = 256;
    uint32_t height = 256;
    uint32_t samples_per_pixel = 1;
    uint32_t ray_tracing_depth = 10;
    uint32_t ray_marching_iterations = 64;
    RendererConfig frame_config = {
        width,
        height,
        samples_per_pixel,
        ray_tracing_depth,
        ray_marching_iterations,
        static_cast<float>(width) / height // Aspect ratio
    };

    VideoSequenceConfig config = {frame_config, 0.0f, 1.0f / 30.0f, 30};
    render_sequence(config, "../images/video_seq_speed_test/", test_sequence_sdf);

   return 0;
}