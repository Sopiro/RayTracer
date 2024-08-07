#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

DiffuseLightMaterial::DiffuseLightMaterial(const Spectrum& color, bool two_sided)
    : Material{ TypeIndex<DiffuseLightMaterial>() }
    , emission{ ConstantColorTexture::Create(color) }
    , two_sided{ two_sided }
{
}

DiffuseLightMaterial::DiffuseLightMaterial(const SpectrumTexture* emission, bool two_sided)
    : Material{ TypeIndex<DiffuseLightMaterial>() }
    , emission{ emission }
    , two_sided{ two_sided }
{
}

bool DiffuseLightMaterial::TestAlpha(const Point2& uv) const
{
    return emission->EvaluateAlpha(uv) > epsilon;
}

const SpectrumTexture* DiffuseLightMaterial::GetNormalMap() const
{
    return nullptr;
}

Spectrum DiffuseLightMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    if (!TestAlpha(isect.uv))
    {
        return Spectrum::black;
    }

    if (isect.front_face || two_sided)
    {
        return emission->Evaluate(isect.uv);
    }
    else
    {
        return Spectrum::black;
    }
}

bool DiffuseLightMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent, alloc.new_object<LambertianBxDF>(Spectrum::black));
    return true;
}

bool DiffuseLightMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return false;
}

} // namespace bulbit
