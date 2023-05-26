#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "color.h"
#include "vec2.h"


struct Material
{
    Color<float> emission;
    float emission_intensity;
    float reflectivity;

    Material() : emission(0.0f), emission_intensity(0.0f) {}
    Material(Color<float> emission) : emission(emission), emission_intensity(1.0f) {}
    Material(Color<float> emission, float intensity) : emission(emission), emission_intensity(intensity) {}
    Material(Color<float> emission, float intensity, float reflectivity)
     : emission(emission), emission_intensity(intensity), reflectivity(reflectivity) {}


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
        virtual Nearest sdf(Vec2 p) const { return {}; }
};


#endif