#pragma once
#ifndef COLOR_H
#define COLOR_H

template<typename T>
struct Color
{
    public:
        T r, g, b, a;

    public:
        Color() : r(0), g(0), b(0), a(0) {}
        Color(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}
        Color(T val) : r(val), g(val), b(val), a(val) {}

        static Color<T> clamp(Color<T> c, T min, T max)
        {
            Color<T> color = c;
            if (color.r < min)
                color.r = min;
            else if (color.r > max)
                color.r = max;

            if (color.g < min)
                color.g = min;
            else if (color.g > max)
                color.g = max;

            if (color.b < min)
                color.b = min;
            else if (color.b > max)
                color.b = max;

            if (color.a < min)
                color.a = min;
            else if (color.a > max)
                color.a = max;

            return color;
        }

        operator Color<uint8_t>() const
        {
            return {
                static_cast<uint8_t>(255.0f * r),
                static_cast<uint8_t>(255.0f * g),
                static_cast<uint8_t>(255.0f * b),
                static_cast<uint8_t>(255.0f * a)
            };
        }

        Color<T> operator +(const Color& other)
        {
            return {r + other.r, g + other.g, b + other.b, a + other.a};
        }
        
        void operator +=(const Color& other)
        {
            r += other.r;
            g += other.g;
            b += other.b;
            a += other.a;
        }

        Color<T> operator -(const Color& other)
        {
            return {r - other.r, g - other.g, b - other.b, a - other.a};
        }
        
        void operator -=(const Color& other)
        {
            r -= other.r;
            g -= other.g;
            b -= other.b;
            a -= other.a;
        }

        Color<T> operator *(const Color& other)
        {
            return {r * other.r, g * other.g, b * other.b, a * other.a};
        }
        
        void operator *=(const Color& other)
        {
            r *= other.r;
            g *= other.g;
            b *= other.b;
            a *= other.a;
        }

        Color<T> operator*(float value)
        {
            return {value * r, value * g, value * b, value *a};
        }   

        void operator*=(float value)
        {
            r *= value;
            g *= value;
            b *= value;
            a *= value;
        }

        Color<T> operator/(float value)
        {
            return {r / value, g / value, b / value, a / value};
        }   

        void operator/=(float value)
        {
            r /= value;
            g /= value;
            b /= value;
            a /= value;
        }
};

#endif