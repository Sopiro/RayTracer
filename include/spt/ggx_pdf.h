#pragma once

#include "microfacet.h"
#include "pdf.h"

namespace spt
{

// GGX normal distribution function PDF
class GGXPDF : public PDF
{
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/
    // https://schuttejoe.github.io/post/ggximportancesamplingpart1/

public:
    GGXPDF(const Vec3& n, const Vec3& wo, double roughness, double t);

    virtual Vec3 Generate() const override;
    virtual double Evaluate(const Vec3& d) const override;

public:
    ONB uvw;
    Vec3 wo;
    double alpha2;
    double t;
};

inline GGXPDF::GGXPDF(const Vec3& n, const Vec3& wo, double roughness, double t)
    : uvw{ n }
    , wo{ wo }
    , alpha2{ roughness * roughness }
    , t{ t }
{
}

inline Vec3 GGXPDF::Generate() const
{
    if (Rand() < t)
    {
        double u = Rand();
        double theta = acos(sqrt((1.0 - u) / ((alpha2 - 1.0) * u + 1.0)));
        double phi = two_pi * Rand();

        double sin_thetha = sin(theta);
        double x = cos(phi) * sin_thetha;
        double y = sin(phi) * sin_thetha;
        double z = cos(theta);

        assert(z > 0.0);

        Vec3 h{ x, y, z }; // Sampled half vector
        Vec3 wh = uvw.GetLocal(h);
        Vec3 wi = Reflect(-wo, wh);

        return wi;
    }
    else
    {
        Vec3 random_cosine = RandomCosineDirection();
        return uvw.GetLocal(random_cosine);
    }
}

inline double GGXPDF::Evaluate(const Vec3& d) const
{
    Vec3 h = (wo + d).Normalized();
    double NoH = Dot(uvw.w, h);
    double spec_w = D_GGX(NoH, alpha2) * NoH / fmax(4.0 * Dot(d, h), epsilon);

    double cosine = Dot(d, uvw.w);
    double diff_w = cosine * inv_pi;

    return (1.0 - t) * diff_w + t * spec_w;
}

} // namespace spt
