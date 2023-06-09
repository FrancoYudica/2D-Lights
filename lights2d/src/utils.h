#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <math.h>
#include <random>
#include "vec2.h"

#define PI 3.141592653589794626433832f

namespace Lights2D
{
    static thread_local std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static thread_local std::mt19937 generator;
    namespace Utils
    {


        static void random_seed(uint32_t seed)
        {
            generator.seed(seed);
        }
        static float random()
        {
    		return distribution(generator);
        }

        static Vec2 random_norm_vec2()
        {
            float angle = 2.0f * PI * Utils::random();
            return {cos(angle), sin(angle)};
        }

        static Vec2 reflect(Vec2 incident, Vec2 normal)
        {
            return incident - normal * (2.0f * Vec2::dot(incident, normal));
        }

        static bool refract(const Vec2& incident, const Vec2& normal, float refractionRatio, Vec2& result)
        {
            float i_dot_n = Vec2::dot(incident, normal);
            float k = 1.0f - refractionRatio * refractionRatio * (1.0f - i_dot_n * i_dot_n);
            if (k < 0.0f)
                return false;

            float a = refractionRatio * i_dot_n + sqrtf(k);
            result.x = refractionRatio * incident.x - a * normal.x;
            result.y = refractionRatio * incident.y - a * normal.y;
            return true;
        }

        static float mix(float a, float b, float t)
        {
            // linear interpolation
            return a + (b - a) * t;
        }

        static Vec2 mix(Vec2 a, Vec2 b, float t)
        {
            // linear interpolation
            return a + (b - a) * t;
        }

        static Color<float> mix(Color<float> a, Color<float> b, float t)
        {
            return {
                mix(a.r, b.r, t),
                mix(a.g, b.g, t),
                mix(a.b, b.b, t),
                mix(a.a, b.a, t)
            };
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
}
#endif