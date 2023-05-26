#pragma once
#ifndef SDF_FUNCTIONS_H
#define SDF_FUNCTIONS_H
#include "vec2.h"

namespace SDF
{
    static float circle(Vec2 pos, Vec2 center, float radius)
    {
        return Vec2::length(pos - center) - radius;
    }
}


#endif