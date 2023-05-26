#pragma once
#ifndef VEC2_H
#define VEC2_H
#include <math.h>
#include <random>
#define PI 3.141592653589794626433832

struct Vec2
{
    public:
        float x, y;

    public:
        Vec2(float x, float y) : x(x), y(y) {}
        explicit Vec2(float v) : x(v), y(v) {}
        Vec2() : x(0), y(0) {}

        static float length(Vec2 vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y); }
        static float length_squared(Vec2 vec) {return vec.x * vec.x + vec.y * vec.y; }
        static Vec2 normalize(Vec2 vec) {return vec * (1.0f / length(vec)); }
        static Vec2 random()
        {
            float angle = 2.0f * PI * static_cast<float>(std::rand()) / RAND_MAX;
            return {cos(angle), sin(angle)};
        }

        Vec2 dot(Vec2 other) { return *this * other; }

        // Operator overloading
        Vec2 operator +(const Vec2& other)
        {
            return {x + other.x, y + other.y};
        }
        
        void operator +=(const Vec2& other)
        {
            x += other.x;
            y += other.y;
        }

        Vec2 operator -(const Vec2& other)
        {
            return {x - other.x, y - other.y};
        }
        
        void operator -=(const Vec2& other)
        {
            x -= other.x;
            y -= other.y;
        }

        Vec2 operator*(const Vec2& other)
        {
            return {x * other.x, y * other.y};
        }   

        void operator*=(const Vec2& other)
        {
            x *= other.x;
            y *= other.y;
        }


        Vec2 operator+(float value)
        {
            return {x + value, y + value};
        }   

        void operator+=(float value)
        {
            x += value;
            y += value;
        }


        Vec2 operator-(float value)
        {
            return {x - value, y - value};
        }   

        void operator-=(float value)
        {
            x -= value;
            y -= value;
        }


        Vec2 operator*(float value)
        {
            return {x * value, y * value};
        }   

        void operator*=(float value)
        {
            x *= value;
            y *= value;
        }


};

#endif