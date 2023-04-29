#pragma once

#include "common.h"

#define COLOR_CHANNELS (3)

namespace spt
{

class Bitmap
{
public:
    Bitmap(int32 width, int32 height);
    ~Bitmap();

    int32 GetWidth() const;
    int32 GetHeight() const;

    void Set(int32 x, int32 y, const Color& color);
    void WriteToFile(char const* filename) const;

private:
    int32 width, height;
    uint8* pixels;
};

inline Bitmap::Bitmap(int32 width, int32 height)
    : width{ width }
    , height{ height }
{
    pixels = new uint8[width * height * COLOR_CHANNELS];
}

inline Bitmap::~Bitmap()
{
    delete[] pixels;
}

inline int32 Bitmap::GetWidth() const
{
    return width;
}

inline int32 Bitmap::GetHeight() const
{
    return height;
}

inline void Bitmap::Set(int32 x, int32 y, const Color& color)
{
    pixels[(x + (height - y - 1) * width) * COLOR_CHANNELS + 0] = static_cast<int32>(Clamp(color.x, 0.0, 0.999) * 256.0);
    pixels[(x + (height - y - 1) * width) * COLOR_CHANNELS + 1] = static_cast<int32>(Clamp(color.y, 0.0, 0.999) * 256.0);
    pixels[(x + (height - y - 1) * width) * COLOR_CHANNELS + 2] = static_cast<int32>(Clamp(color.z, 0.0, 0.999) * 256.0);
}

inline void Bitmap::WriteToFile(char const* filename) const
{
    // stbi_write_png(filename, width, height, COLOR_CHANNELS, pixels, width * COLOR_CHANNELS);
    stbi_write_jpg(filename, width, height, COLOR_CHANNELS, pixels, 100);
}

} // namespace spt