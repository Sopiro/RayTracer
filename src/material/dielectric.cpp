#include "spt/dielectric.h"

namespace spt
{

bool Dielectric::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    f64 refraction_ratio = is.front_face ? (1.0 / ior) : ior;

    f64 cos_theta = Min(Dot(-wi, is.normal), 1.0);
    f64 sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Check for total internal reflection
    bool refractable = refraction_ratio * sin_theta < 1.0;
    Vec3 direction;

    if (refractable == false || Reflectance(cos_theta, refraction_ratio) > Rand())
    {
        direction = Reflect(wi, is.normal);
    }
    else
    {
        direction = Refract(wi, is.normal, refraction_ratio);
    }

    ir->is_specular = true;
    ir->pdf = nullptr;
    ir->attenuation = Color{ 1.0, 1.0, 1.0 };
    ir->specular_ray = Ray{ is.point, direction };

    return true;
}

} // namespace spt
