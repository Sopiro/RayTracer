#include "spt/point_light.h"

namespace spt
{

PointLight::PointLight(const Point3& _position, const Spectrum& _intensity)
    : Light(Light::Type::point_light)
    , position{ _position }
    , intensity{ _intensity }
{
}

Spectrum PointLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const
{
    Vec3 d = position - ref.point;
    Float distance = d.Normalize();

    *wi = d;
    *visibility = distance;
    *pdf = Float(1.0);

    return intensity / (distance * distance);
}

} // namespace spt
