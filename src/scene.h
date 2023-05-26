#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "color.h"
#include "vec2.h"


struct Material
{
    Color<float> emission;
    float emission_intensity;

    Material() : emission(0.0f), emission_intensity(0.0f) {}

};


struct Nearest
{

    float distance;
    Material mtl;
    Nearest() : distance(10e5), mtl() {}

};


class Scene
{
    public:
        Scene() = default;
        virtual Nearest sdf(Vec2 p) { return {}; }
};


#endif