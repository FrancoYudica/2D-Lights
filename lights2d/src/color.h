#pragma once
#ifndef COLOR_H
#define COLOR_H

namespace Lights2D
{

    template<typename T>
    struct Color
    {
        public:
            T r, g, b;

        public:
            Color() : r(0), g(0), b(0) {}
            Color(T r, T g, T b) : r(r), g(g), b(b) {}
            Color(T val) : r(val), g(val), b(val) {}

            static Color<T> clamp(const Color<T>& c, T min, T max)
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

                return color;
            }

            operator Color<uint8_t>() const
            {
                return {
                    static_cast<uint8_t>(255.0f * r),
                    static_cast<uint8_t>(255.0f * g),
                    static_cast<uint8_t>(255.0f * b)
                };
            }

            Color<T> operator +(const Color& other) const
            {
                return {r + other.r, g + other.g, b + other.b};
            }

            void operator +=(const Color& other)
            {
                r += other.r;
                g += other.g;
                b += other.b;
            }

            Color<T> operator -(const Color& other) const
            {
                return {r - other.r, g - other.g, b - other.b};
            }

            void operator -=(const Color& other)
            {
                r -= other.r;
                g -= other.g;
                b -= other.b;
            }

            Color<T> operator *(const Color& other) const
            {
                return {r * other.r, g * other.g, b * other.b};
            }

            void operator *=(const Color& other)
            {
                r *= other.r;
                g *= other.g;
                b *= other.b;
            }

            Color<T> operator*(const float value) const
            {
                return {value * r, value * g, value * b};
            }   

            void operator*=(const float value)
            {
                r *= value;
                g *= value;
                b *= value;
            }

            Color<T> operator/(const float value) const
            {
                return {r / value, g / value, b / value};
            }   

            void operator/=(const float value)
            {
                r /= value;
                g /= value;
                b /= value;
            }
    };
}
#endif