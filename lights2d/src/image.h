#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <cstring>
#include <stdint.h>
#include "color.h"

namespace Lights2D
{
    class Image
    {
        /*
            Lights2D::Image is a data structure that contains the rendered image buffer.
            Allocates memory in the heap, and has a destructor.
        */
        public:
            uint32_t width, height;
            Color<uint8_t>* buffer;

        public:
            Image(uint32_t width, uint32_t height) : width(width), height(height)
            {
                buffer = new Color<uint8_t>[width * height];
            }

            ~Image()
            {
                delete[] buffer;
            }

            void set_pixel(uint32_t x, uint32_t y, Color<uint8_t> color)
            {
                buffer[x + y * width] = color;
            }

            void clear()
            {
                memset(buffer, 0, width * height * sizeof(Color<uint8_t>));
            }

    };
}
#endif
