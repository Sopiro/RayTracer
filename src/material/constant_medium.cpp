#include "spt/constant_medium.h"

namespace spt
{

bool ConstantDensityMedium::Hit(const Ray& ray, float64 t_min, float64 t_max, HitRecord& rec) const
{
    HitRecord rec1;
    HitRecord rec2;

    // Find the closest hit
    if (boundary->Hit(ray, -infinity, infinity, rec1) == false)
    {
        return false;
    }

    // Find the farthest hit
    if (boundary->Hit(ray, rec1.t + ray_offset, infinity, rec2) == false)
    {
        return false;
    }

    if (rec1.t < t_min)
    {
        rec1.t = t_min;
    }

    if (rec2.t > t_max)
    {
        rec2.t = t_max;
    }

    if (rec1.t >= rec2.t)
    {
        return false;
    }

    if (rec1.t < 0.0)
    {
        rec1.t = 0.0;
    }

    float64 ray_length = ray.dir.Length();
    float64 distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    float64 hit_distance = neg_inv_density * log(Rand());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    rec.object = this;
    rec.mat = phase_function.get();
    rec.t = rec1.t + hit_distance / ray_length;
    rec.point = ray.At(rec.t);
    rec.normal = Vec3{ 1.0, 0.0, 0.0 }; // arbitrary
    rec.front_face = true;              // also arbitrary

    return true;
}

} // namespace spt