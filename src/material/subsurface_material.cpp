#include "bulbit/bssrdfs.h"
#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

SubsurfaceMaterial::SubsurfaceMaterial(
    const Spectrum& reflectance, const Spectrum& mfp, Float eta, Float roughness, const SpectrumTexture* normalmap
)
    : SubsurfaceMaterial(ConstantColorTexture::Create(reflectance), mfp, eta, ConstantFloatTexture::Create(roughness), normalmap)
{
}

SubsurfaceMaterial::SubsurfaceMaterial(
    const SpectrumTexture* reflectance, const Spectrum& mfp, Float eta, Float roughness, const SpectrumTexture* normalmap
)
    : SubsurfaceMaterial(reflectance, mfp, eta, ConstantFloatTexture::Create(roughness), normalmap)
{
}

SubsurfaceMaterial::SubsurfaceMaterial(
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    const FloatTexture* roughness,
    const SpectrumTexture* normalmap
)
    : SubsurfaceMaterial(reflectance, mfp, eta, roughness, roughness, normalmap)
{
}

SubsurfaceMaterial::SubsurfaceMaterial(
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    const FloatTexture* u_roughness,
    const FloatTexture* v_roughness,
    const SpectrumTexture* normalmap
)
    : Material(TypeIndexOf<SubsurfaceMaterial>())
    , normalmap{ normalmap }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , reflectance{ reflectance }
    , l{ mfp }
    , eta{ eta }
{
}

bool SubsurfaceMaterial::TestAlpha(const Point2& uv) const
{
    BulbitNotUsed(uv);
    return true;
}
const SpectrumTexture* SubsurfaceMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum SubsurfaceMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    return Spectrum::black;
}

bool SubsurfaceMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(wo);

    Float alpha_x = u_roughness->Evaluate(isect.uv);
    Float alpha_y = v_roughness->Evaluate(isect.uv);

    Float eta_p = isect.front_face ? eta : 1 / eta;

    *bsdf = BSDF(
        isect.shading.normal, isect.shading.tangent,
        alloc.new_object<DielectricBxDF>(eta_p, TrowbridgeReitzDistribution(alpha_x, alpha_y))
    );

    return true;
}

bool SubsurfaceMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    // Spectrum R = reflectance->Evaluate(isect.uv);
    // Spectrum s = Spectrum(1.9f) - R + 3.5f * Sqr(R - Spectrum(0.8f)); // Eq. 6
    // Spectrum d = l / s;

    // *bssrdf = alloc.new_object<DisneyBSSRDF>(R, d, isect, wo, eta);
    *bssrdf = alloc.new_object<RandomWalkBSSRDF>(Spectrum(0), Spectrum(100), isect, wo, eta);
    return true;
}

} // namespace bulbit
