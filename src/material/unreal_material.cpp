#include "bulbit/bxdfs.h"
#include "bulbit/material.h"

namespace bulbit
{

UnrealMaterial::UnrealMaterial(const SpectrumTexture* basecolor,
                               const FloatTexture* metallic,
                               const FloatTexture* roughness,
                               const SpectrumTexture* emissive,
                               const SpectrumTexture* normalmap)
    : basecolor{ basecolor }
    , metallic{ metallic }
    , roughness{ roughness }
    , emissive{ emissive }
    , normalmap{ normalmap }
{
}

bool UnrealMaterial::TestAlpha(const Point2& uv) const
{
    return basecolor->EvaluateAlpha(uv) > epsilon;
}

const SpectrumTexture* UnrealMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum UnrealMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return emissive ? emissive->Evaluate(isect.uv) : Spectrum::black;
}

bool UnrealMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Vec3 n = isect.shading.normal;
    if (Dot(n, wo) < 0)
    {
        // Resolve back facing normal by flipping method
        n = Reflect(n, isect.normal);
    }

    Spectrum b = basecolor->Evaluate(isect.uv);
    Float m = metallic->Evaluate(isect.uv);
    Float r = roughness->Evaluate(isect.uv);
    Float alpha = UnrealBxDF::RoughnessToAlpha(r);

    Spectrum f0 = UnrealBxDF::F0(b, m);
    Spectrum F = UnrealBxDF::F_Schlick(f0, Dot(wo, n));
    Float diff_weight = (1 - m);
    Float spec_weight = F.Luminance();
    // Float spec_weight = std::fmax(F.x, std::fmax(F.y, F.z));
    Float t = Clamp(spec_weight / (diff_weight + spec_weight), 0.15f, 0.9f);

    *bsdf = BSDF(n, isect.shading.tangent, alloc.new_object<UnrealBxDF>(b, m, alpha, t));
    return true;
}

} // namespace bulbit
