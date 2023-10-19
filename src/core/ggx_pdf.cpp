#include "bulbit/ggx_pdf.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Vec3 GGXPDF::Sample() const
{
    if (Rand() < t)
    {
        Vec2 u = RandVec2();
        Vec3 h = Sample_GGX(wo, alpha2, u);

        Vec3 wh = uvw.GetLocal(h);
        Vec3 wi = Reflect(wo, wh);

        return wi;
    }
    else
    {
        Vec3 random_cosine = CosineSampleHemisphere();
        return uvw.GetLocal(random_cosine);
    }
}

Float GGXPDF::Evaluate(const Vec3& wi) const
{
    Vec3 h = Normalize(wo + wi);
    Float NoH = Dot(h, uvw.w);
    Float LoH = Dot(/*L*/ wi, /*H*/ uvw.w);
    Float spec_w = D_GGX(NoH, alpha2) * NoH / std::fmax(4 * LoH, Float(0.0));

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

} // namespace bulbit
