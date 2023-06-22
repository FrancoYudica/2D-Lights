/*
This file contains a set of scenes. Here, different examples of how the signed_distance_function
should be structured
*/


#pragma once
#ifndef SCENES_H
#define SCENES_H
#include "lights2d/lights2d.h"

using namespace Lights2D;

namespace Scenes
{
        
    static void _eval(float distance, const Material& mtl, Nearest& nearest)
    {
        if (distance < nearest.distance)
        {
            nearest.distance = distance;
            nearest.mtl = mtl;
        }
    }      


    static Nearest test_shapes_sdf(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_mtl = Material::create_light({1.0f}, 1.0f);
        /*
        // CIRCLE
        _eval(SDF::circle(pos, Vec2(0.3f, 0.6f), 0.2f), white_mtl, nearest);
        */

        /*
        // BOX
        _eval(SDF::box(pos, {-0.4, -0.4}, {0.1, 0.1}), white_mtl, nearest);
        */
        /*
        // ROUNDED BOX
        _eval(SDF::box(pos, {-0.4, -0.4}, {0.1, 0.1}), white_mtl, nearest) - 0.05;
        */

        /*
        // MOON
        float outer_circle_distance = SDF::circle(pos, {-0.5f, 0.0f}, 0.4);
        float inner_circle_distance = SDF::circle(pos, {-0.3f, 0.0f}, 0.3);
        _eval(SDF::combine_subtract(outer_circle_distance, inner_circle_distance), white_mtl, nearest);
        */

        /*
        // LINE
        float line_distance = SDF::line(pos, Vec2(-0.5), Vec2(0.5), 0.07f);
        _eval(line_distance, white_mtl, nearest);
        */
        
        /*
        // ARC
        float aperture = PI * 0.35;
        float distance = SDF::arc(pos - Vec2(0.4, 0.0f), Vec2(sin(aperture), cos(aperture)), 0.3, 0.01);
        _eval(distance, white_mtl, nearest);
        */

        /*
        HEART
        float distance = SDF::heart(pos, Vec2(0.0f, -0.5f));
        _eval(distance, white_mtl, nearest);
        */

        /*
        EGG
        float distance = SDF::egg(pos, Vec2(0, -0.5), 0.3, 0.01);
        _eval(distance, white_mtl, nearest);
        */

        /*
        PENTAGON
        float distance = SDF::pentagon(pos, 0.3f);
        //_eval(distance, white_mtl, nearest);
        */

        return nearest;
    }

    static  Nearest smooth_reflections(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light(Color(1.0f), 2.0f);
        Material reflective_walls = Material::create_reflective(1.0f);

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

    static Nearest metaballs_3(Vec2 pos, float time)
    {
        Nearest nearest;
        Material mat0 = Material::create_light(Utils::gamma_exp(Color<float>(56, 192, 242) / 255.0f), 1.0f);
        Material mat1 = Material::create_light(Utils::gamma_exp(Color<float>(58, 10, 200) / 255.0f), 1.0f);
        Material mat2 = Material::create_light(Utils::gamma_exp(Color<float>(255, 10, 255) / 255.0f), 1.0f);
        
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


    static Nearest circle_cut(Vec2 pos, float time)
    {
        // Rotates 2 lines with different angular speed
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 1.0f);

        float angle1 = PI * time;
        float angle2 = 2.0f * PI * time;
        float length = 0.6f;
        float static_circle_wall = SDF::circle(pos, Vec2(0.0f, -0.7f), 0.8f);

        // Completes the range [0.0f, 1.0f, 0.0f] in 2 seconds
        float t = abs(sin(time * PI));
        float moving_circle = SDF::circle(pos, Vec2(0.0f, 0.0f), Utils::mix(0.0f, 1.0f, t));

        float dist = SDF::combine_subtract(static_circle_wall, moving_circle);
        _eval(dist, Material::create_reflective(0.9f), nearest);
        _eval(SDF::circle(pos, Vec2(0.2f, 0.7f), 0.15f), white_light, nearest);
        _eval(SDF::circle(pos, Vec2(-1.6f), 0.15f), white_light, nearest);
        _eval(SDF::circle(pos, Vec2(1.6f, -1.0f), 0.15f), white_light, nearest);
        return nearest;
    }

    static Nearest metaballs(Vec2 pos, float time)
    {
        // Rotates 2 lines with different angular speed
        Nearest nearest;
        Material white_light = Material::create_light({powf(1.0f, 2.2f)}, 1.0f);
        Material mat0 = Material::create_light(Utils::gamma_exp(Color<float>(255, 179, 38) / 255.0f), 1.0f);
        Material mat1 = Material::create_light(Utils::gamma_exp(Color<float>(255, 81, 38) / 255.0f), 1.0f);
        Material mat2 = Material::create_light(Utils::gamma_exp(Color<float>(38, 255, 165) / 255.0f), 1.0f);
        Material mat3 = Material::create_light(Utils::gamma_exp(Color<float>(38, 219, 255) / 255.0f), 1.0f);

        float top_light = SDF::box(pos, Vec2(0.0f, 1.2f), Vec2(0.5f, 0.01f));

        _eval(top_light, white_light, nearest);

        // Loop time
        float t = abs(sin(PI * time));

        // Circle center - static
        float circle_center = SDF::circle(pos, Vec2(0.0f, 0.2f * sin(2.0f * PI * time)), 0.25f);

        // Circle 0
        float min_distance0 = 0.3f;
        float max_distance0 = 0.6f;
        float distance0 = Utils::mix(min_distance0, max_distance0,  t);
        float angle0 = 2.0f * time * PI;
        Vec2 pos0(cos(angle0) * distance0, sin(angle0) * distance0);
        float circle0_distance = SDF::circle(pos, pos0, 0.2f);

        // Circle 1
        float min_distance1 = 0.3f;
        float max_distance1 = 0.55f;
        float distance1 = Utils::mix(min_distance1, max_distance1,  t);
        float angle1 = -2.0f * time * PI;
        Vec2 pos1(0.2f + cos(angle1) * distance0, -0.2f * sin(angle1) * distance0);
        float circle1_distance = SDF::circle(pos, pos1, 0.2f);

        // Circle 2
        float distance2 = Utils::mix(0.5f, 0.7f, t);
        float angle2 = -2.0f * PI * time;
        Vec2 pos2(-0.2f + cos(angle2) * distance2, -0.2f + sin(angle2) * distance2);
        float circle2_distance = SDF::circle(pos, pos2, 0.3f);

        std::pair<float, Material> objects[4];
        objects[0] = {circle_center, mat0};
        objects[1] = {circle0_distance, mat3};
        objects[2] = {circle1_distance, mat2};
        objects[3] = {circle2_distance, mat1};

        float k_smooth_factor = 0.3f;

        Material interpolated_mtl = Material::create_opaque();
        float interpolated_sd = 0;
        for (uint32_t i = 0; i < 4; i++)
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


    static Nearest glass_metaballs(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 1.0f);
        Material yellow_light = Material::create_light(Utils::gamma_exp(Color<float>(252, 241, 177) / 255.0f), 3.0f);
        _eval(SDF::box(pos, Vec2(0.0f, 1.2f), Vec2(1.0f, 0.01f)), white_light, nearest);
        _eval(SDF::circle(pos, Vec2(1.3f, 0.0f), 0.2f), yellow_light, nearest);

        Material refractive_material = Material::create_refractive(0.2f, 1.4f);

        _eval(
            SDF::combine_union_s(
                SDF::circle(pos, Vec2(0.4f * static_cast<float>(cos(time * 2.0f * PI)), -0.3f), 0.4f),
                SDF::box(pos, Vec2(-0.4f * static_cast<float>(cos(time * 2.0f * PI)), 0.3f), Vec2(0.4f)),
                0.3f
            ),
            refractive_material,
            nearest
        );
        return nearest;
    }


    static Nearest circular_lens(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 1.0f);
        _eval(SDF::box(pos, Vec2(0.0f, 1.2f), Vec2(1.0f, 0.01f)), white_light, nearest);

        Vec2 origin(0.0f, 0.0f);
        float offset = 0.55f;
        Material refractive_material = Material::create_refractive(0.2f, 1.4f);

        _eval(
            SDF::circle(pos, {0.0f, 0.0f}, 0.5f),
            refractive_material,
            nearest
        );
        return nearest;
    }


    static Nearest glass_absorption(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 1.0f);

        _eval(SDF::line(pos, Vec2(-.6f, 0.65f), Vec2(0.6f, 0.65f), 0.02f), white_light, nearest);
        Material refractive_material = Material::create_refractive(0.0f, 1.4f, Utils::gamma_exp(Color<float>(1.60f, 1.60f, 1.1f)));
        _eval(SDF::box(pos, Vec2(0, -0.2), Vec2(.3f, 0.3f)) - 0.1f, refractive_material, nearest);
        return nearest;
    }

    
    static Nearest convex_lens(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 20.0f);

        _eval(SDF::circle(pos, Vec2(0.0f, 1.4f), 0.05), white_light, nearest);

        Vec2 origin(0.0f, 0.3f);
        float offset = 0.5f;
        Material refractive_material = Material::create_refractive(0.2f, 1.5f);

        _eval(
            SDF::combine_intersect(
                SDF::circle(pos, {origin.x, origin.y + offset}, 0.6f), 
                SDF::circle(pos, {origin.x, origin.y - offset}, 0.6f)
            ),
            refractive_material,
            nearest
        );
        return nearest;
    }


    static Nearest concave_lens(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 20.0f);

        _eval(SDF::circle(pos, Vec2(0.0f, 1.4f), 0.05), white_light, nearest);

        Vec2 origin(0.0f, 0.0f);
        Material refractive_material = Material::create_refractive(0.2f, 1.5f);
        float radius = 0.6f;
        float lens_thickness = 0.1f;
        _eval(
            SDF::combine_subtract(
                SDF::box(pos, origin, Vec2(0.4f, 0.3f)),
                SDF::combine_union(
                    SDF::circle(pos, {origin.x, origin.y + radius + lens_thickness * 0.5f}, radius), 
                    SDF::circle(pos, {origin.x, origin.y - radius - lens_thickness * 0.5f}, radius)
                )
            ),
            refractive_material,
            nearest
        );
        return nearest;
    }

    static Nearest semicircular_lens(Vec2 pos, float time)
    {
        Nearest nearest;
        Material white_light = Material::create_light({1.0f}, 20.0f);

        _eval(SDF::circle(pos, Vec2(0.0f, 1.4f), 0.05), white_light, nearest);

        Vec2 origin(0.0f, 0.3f);
        Material refractive_material = Material::create_refractive(0.2f, 1.5f);
        float radius = 0.9f;
        float lens_thickness = 0.1f;
        _eval(
            SDF::combine_subtract(
                SDF::circle(pos, origin, radius * 0.5f),
                SDF::plane(pos, origin, Vec2(0, -1))
            ),
            refractive_material,
            nearest
        );
        return nearest;
    }

    static Nearest sample_scene(Vec2 pos, float time)
    {
        Nearest nearest;

        Material purple_mtl = Material::create_light({0.9f, 0.4f, 1.0f}, 1.5f);
        Material pink_mtl = Material::create_light({0.8f, 0.0f, 0.21f}, 1.0f);
        Material opaque_mtl = Material::create_opaque();
        Material reflective_mtl = Material::create_reflective(0.6f);
        
        // Lights
        {
            Material lights_material = Material::create_light({1.0f}, 1.0f);
            // Top light    
            _eval(SDF::box(pos, Vec2(0.0, 1.0f - 0.1f), Vec2(0.5f, 0.01f)), lights_material, nearest);

            float x_light_distance = 2.0f;
            _eval(SDF::line(pos, Vec2(-x_light_distance, -1.0f), Vec2(-x_light_distance, 1.0f), 0.05f), lights_material, nearest);
            _eval(SDF::line(pos, Vec2(x_light_distance, -1.0f), Vec2(x_light_distance, 1.0f), 0.05f), lights_material, nearest);
        }

        // Smooth shape
        {
            float box_sd = SDF::box(pos, Vec2(0.0, 0.4f), Vec2(0.3f, 0.05f));

            float circles_sd;
            {
                float circle_sd_0 = SDF::circle(pos, Vec2(0.3f, 0.4f), 0.1f);
                float circle_sd_1 = SDF::circle(pos, Vec2(-0.25f, 0.3f), 0.15f);
                circles_sd = SDF::combine_union(circle_sd_0, circle_sd_1);
            }

            float k = 0.08f;
            float h = SDF::smooth_t(box_sd, circles_sd, k);

            float d = SDF::combine_union_s(box_sd, circles_sd, k, h);
            Material mix_mtl = Material::mix(purple_mtl, pink_mtl, h);
            _eval(d, mix_mtl, nearest);
        }

        // Reflective edges
        {
            float box_sd_0 = SDF::line(pos, Vec2(-0.3f, -0.2), Vec2(0.3f, -0.2f), 0.15f);
            float box_sd_1 = SDF::line(pos, Vec2(-0.65, 0.7), Vec2(-0.65, 0.1), 0.05f);
            float box_sd_2 = SDF::line(pos, Vec2(0.65, 0.7), Vec2(0.65, 0.1), 0.05f);
            
            float boxes = SDF::combine_union(SDF::combine_union(box_sd_0, box_sd_1), box_sd_2);           
            float circle_sd = SDF::circle(pos, Vec2(), 0.2f);
            float line_sd = SDF::line(pos, Vec2(), Vec2(0.0f, -0.5f), 0.1f);

            float subtract_sd = SDF::combine_union_s(circle_sd, line_sd, 0.1f);

            _eval(SDF::combine_subtract(boxes, subtract_sd), reflective_mtl, nearest);
        }
        {
            Vec2 origin(0.0f, -0.7f);
            float offset = 0.5f;
            Material refractive_material = Material::create_refractive(0.2f, 1.5f);

            _eval(
                SDF::combine_intersect(
                    SDF::circle(pos, {origin.x, origin.y + offset}, 0.6f), 
                    SDF::circle(pos, {origin.x, origin.y - offset}, 0.6f)
                ),
                refractive_material,
                nearest
            );
        }
        return nearest;
    }


    static Nearest metaballs_absorption(Vec2 pos, float time)
    {
        Nearest nearest;

        // LIGHTS
        Material white_light = Material::create_light({1.0f}, 1.0f);
        Material yellow_light = Material::create_light(Color<float>(252, 241, 177) / 255.0f, 3.0f);
        _eval(SDF::box(pos, Vec2(0.0f, 1.2f), Vec2(1.0f, 0.01f)), white_light, nearest);
        _eval(SDF::circle(pos, Vec2(1.3f, 0.0f), 0.2f), yellow_light, nearest);

        // Metaballs
        Material refractive_material1 = Material::create_refractive(0.0f, 1.4f, {1.2f, 1.7f, 2.2f});
        Material refractive_material2 = Material::create_refractive(0.0f, 1.4f, {1.1f, 1.3f, 2.5f});

        float box = SDF::box(pos, Vec2(-0.4f * static_cast<float>(cos(time * 2.0f * PI)), 0.3f), Vec2(0.4f)) - 0.08f;
        float circle =  SDF::circle(pos, Vec2(0.4f * static_cast<float>(cos(time * 2.0f * PI)), -0.3f), 0.4f);
        float k = 0.3f;

        float h = SDF::smooth_t(box, circle, k);
        float distance = SDF::combine_union_s(box, circle, k, h);
        Material mtl = Material::mix(refractive_material1, refractive_material2, h);
        _eval(distance, mtl, nearest);
        return nearest;
    }

    static Nearest caustics(Vec2 pos, float time)
    {
        Nearest nearest;

        // LIGHTS
        Material white_light = Material::create_light({1.0f}, 1.0f);
        _eval(SDF::circle(pos, Vec2(.3f, 0.6f), 0.2f), white_light, nearest);

        _eval(
            SDF::combine_subtract(
                SDF::plane(pos, Vec2(0.0f), Vec2(0.0f, 1.0f)),
                SDF::circle(pos, Vec2(0.0f), 0.8f)
            ),
            Material::create_reflective(0.9f),
            nearest
        );
        return nearest;
    }


    static Nearest room_sdf(Vec2 pos, float time)
    {
        Nearest nearest;

        Material white_mtl = Material::create_light({1.0f}, 1.0f);
        Material black_mtl = Material::create_reflective(0.5f);
        Material purple_mtl = Material::create_light(Color(0.3f, 0.0f, .8f), 1.3f);
        Material light_purple_mtl = Material::create_light(Color(0.5f, 0.2f, 1.0f), 1.3f);
        Material green_mtl = Material::create_light(Color(0.7f, 1.0f, .2f), 1.3f);

        // Center circle
        _eval(SDF::circle(pos, Vec2(), 0.2f), purple_mtl, nearest);

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
        Vec2 box_size(0.1f, 0.05f);
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


    static Nearest rainbow_sdf(Vec2 pos, float time)
    {
        Nearest nearest;
        Color<float> colors[7];
        colors[0] = Utils::gamma_exp(Color<float>(148, 0, 211) / 255.0f);
        colors[1] = Utils::gamma_exp(Color<float>(75, 0, 130) / 255.0f);
        colors[2] = Utils::gamma_exp(Color<float>(0, 0, 255) / 255.0f);
        colors[3] = Utils::gamma_exp(Color<float>(0, 255, 0) / 255.0f);
        colors[4] = Utils::gamma_exp(Color<float>(255, 255, 0) / 255.0f);
        colors[5] = Utils::gamma_exp(Color<float>(255, 127, 0) / 255.0f);
        colors[6] = Utils::gamma_exp(Color<float>(255, 0 , 0) / 255.0f);

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

    static Nearest color_interpolation(Vec2 pos, float time)
    {
        Nearest nearest;
        Material purple_light = Material::create_light(Utils::gamma_exp(Color(0.3f, 0.0f, .8f)), 1.0f);
        Material red_light = Material::create_light(Utils::gamma_exp(Color(1.0f, 0.0f, 0.443f)), 1.0f);

        float k_smooth_factor = 0.3f;

        float line_x = SDF::line(pos, Vec2(-0.5, 0.0f), Vec2(0.5f, 0.0f), 0.05f);
        float line_y = SDF::line(pos, Vec2(0.0f, -0.5f), Vec2(0.0f, 0.5f), 0.05f);

        float h = SDF::smooth_t(line_x, line_y, k_smooth_factor);
        float objects = SDF::combine_union_s(line_x, line_y, k_smooth_factor, h);
        auto interpolated_mtl = Material::mix(purple_light, red_light, h);

        _eval(objects, interpolated_mtl, nearest);

        return nearest;
    }

    static Nearest intensity_interpolation(Vec2 pos, float time)
    {
        Nearest nearest;
        Material purple_light = Material::create_light(Utils::gamma_exp(Color(0.3f, 0.0f, .8f)), 2.0f);
        Material purple_light_darker = Material::create_light(Utils::gamma_exp(Color(0.3f, 0.0f, .8f)), 0.7f);
        Material green_light = Material::create_light(Utils::gamma_exp(Color(0.7f, 1.0f, .2f)), 1.3f);

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
}

#endif