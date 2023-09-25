#include "spt/sphere.h"

namespace spt
{

bool Sphere::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    Vec3 oc = ray.o - center;
    f64 a = ray.d.Length2();
    f64 half_b = Dot(oc, ray.d);
    f64 c = oc.Length2() - radius * radius;

    f64 discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0)
    {
        return false;
    }
    f64 sqrt_d = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    f64 root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    // Found intersection
    is->object = this;
    is->material = GetMaterial();
    is->t = root;
    is->point = ray.At(root);
    Vec3 outward_normal = (is->point - center) / radius;

    Vec3 t = (std::fabs(outward_normal.y) > 0.999) ? x_axis : y_axis;
    Vec3 outward_tangent = Normalize(Cross(t, outward_normal));

    SetFaceNormal(is, ray.d, outward_normal, outward_normal, outward_tangent);
    is->uv = ComputeSphereUV(outward_normal);

    return true;
}

bool Sphere::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
    Vec3 oc = ray.o - center;
    f64 a = ray.d.Length2();
    f64 half_b = Dot(oc, ray.d);
    f64 c = oc.Length2() - radius * radius;

    f64 discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0)
    {
        return false;
    }
    f64 sqrt_d = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    f64 root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    return true;
}

void Sphere::Sample(Intersection* sample, f64* pdf) const
{
    f64 area = 4.0 * pi * radius * radius;
    sample->normal = UniformSampleSphere();
    sample->point = center + sample->normal * radius;
    sample->uv = ComputeSphereUV(sample->normal);
    *pdf = 1.0 / area;
}

void Sphere::Sample(Intersection* sample, f64* pdf, Vec3* ref2p, const Point3& ref) const
{
    Vec3 direction = center - ref;
    Real distance = direction.Normalize();
    Real distance_squared = distance * distance;

#if 0
    if (distance * distance <= radius * radius)
    {
        return Sample();
    }
#endif

    Real u1 = Rand();
    Real u2 = Rand();

    Real cos_theta_max = std::sqrt(1.0 - radius * radius / distance_squared);
    Real z = Real(1.0) + u2 * (cos_theta_max - Real(1.0));

    Real phi = two_pi * u1;

    Real sin_theta = std::sqrt(Real(1.0) - z * z);
    Real x = std::cos(phi) * sin_theta;
    Real y = std::sin(phi) * sin_theta;

    Vec3 d{ x, y, z };

    Real s = distance * z - std::sqrt(radius * radius - distance_squared * sin_theta * sin_theta);

    // Real cosAlpha = (distance * distance + radius * radius - s * s) / (2 * distance * radius);
    // Real sinAlpha = std::std::sqrt(std::fmax((Real)0, 1 - cosAlpha * cosAlpha));

    // Real xx = std::cos(phi) * sinAlpha;
    // Real yy = std::sin(phi) * sinAlpha;
    // Real zz = cosAlpha;

    // Vec3 dd{ -xx, -yy, -zz };
    ONB uvw{ direction };

    *ref2p = uvw.GetLocal(d) * s;

    f64 solid_angle = two_pi * (1.0 - cos_theta_max);

    sample->point = ref + *ref2p;
    sample->normal = Normalize(sample->point - center);
    *pdf = 1.0 / solid_angle;

    sample->front_face = true;

    sample->object = this;
    sample->material = GetMaterial();
}

} // namespace spt