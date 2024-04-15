#pragma once

#include "light.h"

namespace bulbit
{

struct DirectionalLight : public Light
{
public:
    DirectionalLight(const Vec3& dir, const Spectrum& intensity, Float radius);

    virtual Light* Clone(Allocator* allocator) const override;

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    Vec3 dir;
    Spectrum intensity; // radiance
    Float radius;       // visible radius
};

inline Float DirectionalLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return Float(0.0);
}

} // namespace bulbit
