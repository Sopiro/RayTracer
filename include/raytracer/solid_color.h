#pragma once

#include "texture.h"

namespace spt
{

class SolidColor : public Texture
{
public:
    static Ref<SolidColor> Create(Color color);
    static Ref<SolidColor> Create(double red, double green, double blue);

    virtual Color Value(const UV& uv, const Point& p) const override;

private:
    SolidColor() = default;
    SolidColor(Color color);

    Color color;
};

inline SolidColor::SolidColor(Color _color)
    : color(_color)
{
}

struct ColorHash
{
    size_t operator()(const Color& v) const
    {
        return std::hash<double>()(v.x) ^ std::hash<double>()(v.y) ^ std::hash<double>()(v.z);
    }
};

static int32 color_count = 0;
static std::unordered_map<Color, Ref<SolidColor>, ColorHash> loaded_colors;

inline Ref<SolidColor> SolidColor::Create(Color color)
{
    auto loaded = loaded_colors.find(color);
    if (loaded != loaded_colors.end())
    {
        return loaded->second;
    }

    Ref<SolidColor> ptr{ new SolidColor(color) };
    loaded_colors.emplace(color, ptr);
    ++color_count;

    return ptr;
}

inline Ref<SolidColor> SolidColor::Create(double red, double green, double blue)
{
    return Create(Color{ red, green, blue });
}

inline Color SolidColor::Value(const UV& uv, const Point& p) const
{
    return color;
}

} // namespace spt
