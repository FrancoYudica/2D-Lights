#pragma once
#ifndef SDF_FUNCTIONS_H
#define SDF_FUNCTIONS_H
#include "vec2.h"
#include "utils.h"

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

    static float combine_union_s(float d1, float d2, float k)
    {
        // Smooth union
        float h = smooth_t(d1, d2, k);
        return mix(d2, d1, h) - k * h * (1.0f - h);
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
    static float equilateral_triangle(Vec2 pos, Vec2 center, float r)
    {
        Vec2 p = pos;
        const float k = sqrt(3.0);
        p.x = abs(p.x) - r;
        p.y = p.y + r/k;
        
        if(p.x + k * p.y > 0.0) 
            p = Vec2(p.x - k * p.y, -k * p.x - p.y) * 0.5f;
        p.x -= clamp(p.x, -2.0 * r, 0.0);
        return -Vec2::length(p) * sign(p.y); 
    }
    
    static float triangle(Vec2 p, Vec2 p0, Vec2 p1, Vec2 p2)
    {
        Vec2 e0 = p1-p0, e1 = p2-p1, e2 = p0-p2;
        Vec2 v0 = p -p0, v1 = p -p1, v2 = p -p2;
        Vec2 pq0 = v0 - e0*clamp(Vec2::dot(v0,e0) / Vec2::dot(e0,e0), 0.0, 1.0 );
        Vec2 pq1 = v1 - e1*clamp(Vec2::dot(v1,e1) / Vec2::dot(e1,e1), 0.0, 1.0 );
        Vec2 pq2 = v2 - e2*clamp(Vec2::dot(v2,e2) / Vec2::dot(e2,e2), 0.0, 1.0 );
        float s = sign( e0.x*e2.y - e0.y*e2.x );
        Vec2 d = min(min(Vec2(Vec2::dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
                         Vec2(Vec2::dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
                         Vec2(Vec2::dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));

        return -sqrt(d.x)*sign(d.y);
    }

    static float arc(Vec2 p, Vec2 sc, float ra, float rb)
    {
        // sc is the vec2(sin, cos) of the arc's aperture
        p.x = abs(p.x);
        return ((sc.y*p.x>sc.x*p.y) ? Vec2::length(p-sc*ra) : abs(Vec2::length(p)-ra)) - rb;
    }

    static float heart(Vec2 p, Vec2 center)
    {
        p -= center;
        p.x = abs(p.x);

        if( p.y+p.x>1.0 )
            return sqrt(Vec2::length_squared(p-Vec2(0.25,0.75))) - sqrt(2.0)/4.0;
        return sqrt(min(Vec2::length_squared(p-Vec2(0.00,1.00)),
                        Vec2::length_squared(p-0.5*max(p.x+p.y,0.0)))) * sign(p.x-p.y);
    }

    static float egg(Vec2 p, Vec2 center, float ra, float rb)
    {
        p -= center;
        const float k = sqrt(3.0);
        p.x = abs(p.x);
        float r = ra - rb;
        return ((p.y<0.0)       ? Vec2::length(Vec2(p.x,  p.y    )) - r :
                (k*(p.x+r)<p.y) ? Vec2::length(Vec2(p.x,  p.y-k*r)) :
                                Vec2::length(Vec2(p.x+r,p.y    )) - 2.0*r) - rb;
    }

    static float pentagon(Vec2 p, float r)
    {
        const float k0 = 0.809016994;
        const float k1 = 0.587785252;
        const float k2 = 0.726542528;
        
        p.x = abs(p.x);
        p -= Vec2(-k0,k1) * 2.0f*min(Vec2::dot(Vec2(-k0,k1),p),0.0);
        p -= Vec2( k0,k1) * 2.0f*min(Vec2::dot(Vec2( k0,k1),p),0.0);
        p -= Vec2(clamp(p.x,-r*k2,r*k2),r);    
        return Vec2::length(p)*sign(p.y);
    }

}


#endif