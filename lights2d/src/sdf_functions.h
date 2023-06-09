#pragma once
#ifndef SDF_FUNCTIONS_H
#define SDF_FUNCTIONS_H
#include "vec2.h"
#include "utils.h"


namespace Lights2D
{
    using namespace Utils;

    namespace SDF
    {

        static float combine_union(float d1, float d2)
        {
            return min(d1, d2);
        }

        static float combine_subtract(float d1, float d2)
        {
            return max(d1, -d2);
        }

        static float smooth_t(float d1, float d2, float k)
        {
            return clamp(0.5f + 0.5f * (d2 - d1) / k, 0.0f, 1.0f);
        }


        static float combine_union_s(float d1, float d2, float k, float h)
        {
            // Smooth union when h is already calculated
            return mix(d2, d1, h) - k * h * (1.0f - h);
        }

        static float combine_union_s(float d1, float d2, float k)
        {
            // Smooth union
            float h = smooth_t(d1, d2, k);
            return mix(d2, d1, h) - k * h * (1.0f - h);
        }

        static float combine_intersect(float d1, float d2)
        {
            return max(d1, d2);
        }

        static float combine_subtract_s(float d1, float d2, float k)
        {
            // Smooth subtract
            float h = smooth_t(d1, d2, k);
            return mix(d2, -d1, h) - k * h * (1.0f - h);
        }

        static float circle(Vec2 pos, Vec2 center, float radius)
        {
            return Vec2::length(pos - center) - radius;
        }

        static float box(Vec2 pos, Vec2 center, Vec2 size)
        {
            Vec2 q = abs(pos - center) - size;
            return Vec2::length(max(q, 0.0f)) + min(max(q.x, q.y), 0.0f);
        }

        static float round_box(Vec2 pos, Vec2 center, Vec2 size, float r)
        {
            Vec2 q = abs(pos - center) - size;
            return Vec2::length(max(q, 0.0f)) + min(max(q.x, q.y), 0.0f) - r;
        }    

        static float line(Vec2 pos, Vec2 a, Vec2 b, float r)
        {
            Vec2 pa = pos - a;
            Vec2 ba = b - a;
            float h = Utils::clamp(Vec2::dot(pa, ba) / Vec2::dot(ba, ba), 0.0f, 1.0f);
            return Vec2::length(pa - ba * h) - r;
        }

        static float plane(Vec2 p, Vec2 p0, Vec2 normal)
        {
            return Vec2::dot(p - p0, normal);
        }
        
        static float arc(Vec2 p, Vec2 sc, float ra, float rb)
        {
            // sc is the vec2(sin, cos) of the arc's aperture
            p.x = std::abs(p.x);
            return ((sc.y*p.x>sc.x*p.y) ? Vec2::length(p-sc*ra) : std::abs(Vec2::length(p)-ra)) - rb;
        }

        static float heart(Vec2 p, Vec2 center)
        {
            p -= center;
            p.x = std::abs(p.x);

            if( p.y+p.x>1.0 )
                return sqrt(Vec2::length_squared(p-Vec2(0.25f,0.75f))) - sqrt(2.0f)/4.0f;
            return sqrt(min(Vec2::length_squared(p-Vec2(0.00f,1.00f)),
                            Vec2::length_squared(p-0.5f*max(p.x+p.y,0.0)))) * sign(p.x-p.y);
        }

        static float egg(Vec2 p, Vec2 center, float ra, float rb)
        {
            p -= center;
            const float k = sqrt(3.0f);
            p.x = std::abs(p.x);
            float r = ra - rb;
            return ((p.y<0.0)       ? Vec2::length(Vec2(p.x,  p.y    )) - r :
                    (k*(p.x+r)<p.y) ? Vec2::length(Vec2(p.x,  p.y-k*r)) :
                                    Vec2::length(Vec2(p.x+r,p.y    )) - 2.0f*r) - rb;
        }

        static float pentagon(Vec2 p, float r)
        {
            const float k0 = 0.809016994f;
            const float k1 = 0.587785252f;
            const float k2 = 0.726542528f;

            p.x = std::abs(p.x);
            p -= Vec2(-k0,k1) * 2.0f*min(Vec2::dot(Vec2(-k0,k1),p),0.0);
            p -= Vec2( k0,k1) * 2.0f*min(Vec2::dot(Vec2( k0,k1),p),0.0);
            p -= Vec2(clamp(p.x,-r*k2,r*k2),r);    
            return Vec2::length(p)*sign(p.y);
        }

    }

}
#endif