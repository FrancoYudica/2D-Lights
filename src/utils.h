#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <math.h>
#include <random>
#include "vec2.h"
#include "scene.h"

#define PI 3.141592653589794626433832

namespace Utils
{

    static float random()
    {
        return static_cast<float>(std::rand()) / RAND_MAX;
    }

    static Vec2 random_norm_vec2()
    {
        float angle = 2.0f * PI * Utils::random();
        return {cos(angle), sin(angle)};
    }

    static Vec2 normal(Vec2 p, const Scene* scene)
    {
        constexpr float epsilon = 0.001f;
        return {
            (scene->sdf({p.x + epsilon, p.y}).distance - scene->sdf({p.x - epsilon, p.y}).distance) * 0.5f / epsilon,
            (scene->sdf({p.x, p.y + epsilon}).distance - scene->sdf({p.x, p.y - epsilon}).distance) * 0.5f / epsilon
        };
    }  

    static Vec2 reflect(Vec2 incident, Vec2 normal)
    {
        return incident - normal * (2.0f * Vec2::dot(incident, normal));
    }

    static float min(float a, float b)
    {
        return a < b ? a : b;
    }

    static float max(float a, float b)
    {
        return a > b ? a : b;
    }

    static Vec2 max(Vec2 a, Vec2 b)
    {
        return {
            max(a.x, b.x),
            max(a.y, b.y)
        };
    }

    static Vec2 max(Vec2 a, float v)
    {
        return {
            max(a.x, v),
            max(a.y, v)
        };
    }

    static Vec2 min(Vec2 a, Vec2 b)
    {
        return {
            min(a.x, b.x),
            min(a.y, b.y)
        };
    }


    static Vec2 abs(Vec2 a)
    {
        return {
            std::abs(a.x),
            std::abs(a.y)
        };
    }

    static float clamp(float val, float min, float max)
    {
        if (val < min)
            return min;

        else if (val > max)
            return max;

        return val;
    }

    static float sign(float a)
    {
        return a > 0.0f ? 1.0f : 0.0f;
    }
}

#endif