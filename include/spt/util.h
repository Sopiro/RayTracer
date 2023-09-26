#pragma once

#include "mesh.h"
#include "solid_color.h"
#include "transform.h"

namespace spt
{

struct MaterialTextures
{
    Ref<Texture> basecolor;
    Ref<Texture> metallic;
    Ref<Texture> roughness;
    Ref<Texture> emissive;
    Ref<Texture> normal_map;
};

struct MaterialColors
{
    Color diffuse;
    Color specular;
    Color emissive;
};

// Create microfacet material with given textures and colors
Ref<Material> CreateMaterial(const MaterialTextures& textures, const MaterialColors& colors);
Ref<Microfacet> RandomMicrofacetMaterial();

Ref<Mesh> CreateRectXY(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV(1.0, 1.0));
Ref<Mesh> CreateRectXZ(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV(1.0, 1.0));
Ref<Mesh> CreateRectYZ(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV(1.0, 1.0));
Ref<Mesh> CreateBox(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV(1.0, 1.0));

inline bool IsNullish(f64 v)
{
    return std::isnan(v) || std::isinf(v);
}

template <typename T>
inline bool IsNullish(const T& v)
{
    return v.IsNullish();
}

inline bool IsBlack(Color color)
{
    return std::fabs(color.x) == 0.0 && std::fabs(color.y) == 0.0 && std::fabs(color.z) == 0.0;
}

// https://en.wikipedia.org/wiki/Luma_(video)
inline f64 Luma(Vec3 srgb)
{
    return Dot(srgb, Vec3(0.2126, 0.7152, 0.0722));
    // return Dot(srgb, Vec3(0.299, 0.587, 0.114));
}

#define checkNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
    }

} // namespace spt
