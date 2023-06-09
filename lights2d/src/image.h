#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <stdint.h>
#include "color.h"
#include "stb_image.h"
#include "stb_image_write.h"


namespace Lights2D
{
    struct Image
    {
        Color<uint8_t>* buffer;
        uint32_t width, height;
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

        void save(const std::string& path)
        {
            stbi_write_jpg(path.c_str(), width, height, 4, &buffer[0], width * sizeof(int));
        }
    };
}
#endif
