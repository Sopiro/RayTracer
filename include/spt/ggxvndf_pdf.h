#pragma once

#include "microfacet.h"
#include "pdf.h"

namespace spt
{

// GGX visible normal distribution function PDF
class GGXVNDFPDF : public PDF
{
    // https://hal.inria.fr/hal-01024289/file/Heitz2014Slides.pdf
    // https://jcgt.org/published/0003/02/03/
    // https://hal.inria.fr/hal-00996995v1/document
    // https://hal.archives-ouvertes.fr/hal-01509746/document
    // https://schuttejoe.github.io/post/ggximportancesamplingpart2/
    // https://simonstechblog.blogspot.com/2020/01/note-on-sampling-ggx-distribution-of.html
    // https://cdrdv2-public.intel.com/782052/sampling-visible-ggx-normals.pdf

public:
    GGXVNDFPDF(const Vec3& n, const Vec3& wo, f64 roughness, f64 t);

    virtual Vec3 Generate() const override;
    virtual f64 Evaluate(const Vec3& d) const override;

public:
    ONB uvw;
    Vec3 wo;
    f64 alpha;
    f64 t;
};

inline GGXVNDFPDF::GGXVNDFPDF(const Vec3& n, const Vec3& wo, f64 roughness, f64 t)
    : uvw{ n }
    , wo{ wo }
    , alpha{ roughness }
    , t{ t }
{
}

inline Vec3 GGXVNDFPDF::Generate() const
{
    if (Rand() < t)
    {
        // Faster sampling routine
        // https://gist.github.com/jdupuy/4c6e782b62c92b9cb3d13fbb0a5bd7a0
#if 1
        Vec2 u = RandVec2();

        // warp to the hemisphere configuration
        Vec3 woStd = Vec3(wo.x * alpha, wo.y * alpha, wo.z).Normalized();

        // sample the visible hemisphere from a spherical cap

        // sample a spherical cap in (-wo.z, 1]
        f64 phi = 2.0 * pi * u.x;
        f64 z = fma(1.0 - u.y, 1.0 + wo.z, -wo.z);
        f64 sin_thetha = sqrt(Clamp(1.0 - z * z, 0.0, 1.0));
        f64 x = sin_thetha * cos(phi);
        f64 y = sin_thetha * sin(phi);
        Vec3 c = Vec3(x, y, z);

        // compute halfway direction
        Vec3 h = c + wo;

        // warp back to the ellipsoid configuration
        Vec3 wm = Vec3(h.x * alpha, h.y * alpha, h.z).Normalized();

        Vec3 wh = uvw.GetLocal(wm);
        Vec3 wi = Reflect(-wo, wh);

        return wi;
#else
        // Section 3.2: transforming the view direction to the hemisphere configuration
        Vec3 Vh{ alpha * wo.x, alpha * wo.y, wo.z };
        Vh.Normalize();

        // Build an orthonormal basis with v, t1, and t2
        // Section 4.1: orthonormal basis (with special case if cross product is zero)
        Vec3 T1 = (Vh.z < 0.999) ? (Cross(Vh, z_axis)).Normalized() : x_axis;
        Vec3 T2 = Cross(T1, Vh);

        Vec2 u = RandVec2();

        // Section 4.2: parameterization of the projected area
        float64 r = sqrt(u.x);
        float64 phi = two_pi * u.y;
        float64 t1 = r * cos(phi);
        float64 t2 = r * sin(phi);
        float64 s = 0.5 * (1.0 + Vh.z);
        t2 = Lerp(sqrt(1.0 - t1 * t1), t2, s);

        // Section 4.3: reprojection onto hemisphere
        Vec3 Nh = t1 * T1 + t2 * T2 + sqrt(fmax(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;

        // Section 3.4: transforming the normal back to the ellipsoid configuration
        Vec3 h = Vec3(alpha * Nh.x, alpha * Nh.y, fmax(0.0, Nh.z)).Normalized(); // Sampled half vector
        Vec3 wh = uvw.GetLocal(h);
        Vec3 wi = Reflect(-wo, wh);

        return wi;
#endif
    }
    else
    {
        Vec3 random_cosine = RandomCosineDirection();
        return uvw.GetLocal(random_cosine);
    }
}

inline f64 GGXVNDFPDF::Evaluate(const Vec3& d) const
{
    f64 alpha2 = alpha * alpha;

    Vec3 h = (wo + d).Normalized();
    f64 NoH = Dot(uvw.w, h);
    f64 NoV = Dot(uvw.w, d);
    f64 spec_w = D_GGX(NoH, alpha2) * G1_Smith(NoV, alpha2) / Max(4.0 * NoV, epsilon);

    f64 cosine = Dot(d, uvw.w);
    f64 diff_w = cosine * inv_pi;

    return (1.0 - t) * diff_w + t * spec_w;
}

} // namespace spt
