#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"
#include "utils.h"

struct Material
{
    /*
    Material defines the properties of the object and the light interactions
    A light should never be a reflective object, otherwise weird artifacts happen,
    that's the main reason behind the creation of Material
    */
    Color<float> emission;
    float emission_intensity;
    float reflectivity;
    Material() : Material(Color(0.0f), 0.0f, 0.0f){}
    Material(const Material& other)
    : emission(other.emission), emission_intensity(other.emission_intensity), reflectivity(other.reflectivity) {}

    static Material create_opaque() { 
        // Black material with no reflections
        return Material(Color(0.0f), 0.0f, 0.0f);
    }

    static Material create_reflective(float reflectivity)
    {
        return Material(Color(0.0f), 0.0f, reflectivity);
    }

    static Material create_light(Color<float> color, float intensity)
    {
        return Material(color, intensity, 0.0f);
    }

    static Material mix(const Material& m1, const Material& m2, float t)
    {
        return Material(
            Utils::mix(m1.emission, m2.emission, t),
            Utils::mix(m1.emission_intensity, m2.emission_intensity, t),
            Utils::mix(m1.reflectivity, m2.reflectivity, t)
        );
    }
    private:
        // Black material
        Material(Color<float> e, float intensity, float ref)
        : emission(e), emission_intensity(intensity), reflectivity(ref) {}



};

#endif