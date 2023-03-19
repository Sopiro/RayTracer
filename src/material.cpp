#include "raytracer/material.h"
#include "raytracer/hittable.h"
#include "raytracer/onb.h"

namespace spt
{

bool Lambertian::Scatter(const Ray& in_ray, const HitRecord& in_rec, Color& out_alb, Ray& out_scattered, double& out_pdf) const
{
    ONB uvw;
    uvw.BuildFromW(in_rec.normal);

    // lambertian scattering
    Vec3 scatter_direction = uvw.GetLocal(RandomCosineDirection());

    out_scattered = Ray{ in_rec.p, scatter_direction.Normalized() };
    out_alb = albedo->Value(in_rec.uv, in_rec.p);
    out_pdf = Dot(uvw.w, out_scattered.dir) / pi;

    return true;
}

double Lambertian::ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    // Cosine density
    return Dot(in_rec.normal, in_scattered.dir) / pi;
}

bool Metal::Scatter(const Ray& in_ray, const HitRecord& in_rec, Color& out_alb, Ray& out_scattered, double& out_pdf) const
{
    Vec3 reflected = Reflect(in_ray.dir.Normalized(), in_rec.normal);
    out_scattered = Ray{ in_rec.p, reflected + fuzziness * RandomInUnitSphere() };
    out_alb = albedo;

    return Dot(out_scattered.dir, in_rec.normal) > 0;
}

bool Dielectric::Scatter(const Ray& in_ray, const HitRecord& in_rec, Color& out_alb, Ray& out_scattered, double& out_pdf) const
{
    out_alb = Color{ 1.0, 1.0, 1.0 };
    double refraction_ratio = in_rec.front_face ? (1.0 / ir) : ir;

    Vec3 unit_direction = in_ray.dir.Normalized();

    double cos_theta = Min(Dot(-unit_direction, in_rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Total Internal Reflection
    bool refractable = refraction_ratio * sin_theta < 1.0;
    Vec3 direction;

    if (refractable == false || Reflectance(cos_theta, refraction_ratio) > Rand())
    {
        direction = Reflect(unit_direction, in_rec.normal);
    }
    else
    {
        direction = Refract(unit_direction, in_rec.normal, refraction_ratio);
    }

    out_scattered = Ray{ in_rec.p, direction };

    return true;
}

bool Isotropic::Scatter(const Ray& in_ray, const HitRecord& in_rec, Color& out_alb, Ray& out_scattered, double& out_pdf) const
{
    out_scattered = Ray{ in_rec.p, RandomInUnitSphere() };
    out_alb = albedo->Value(in_rec.uv, in_rec.p);

    return true;
}

Color DiffuseLight::Emitted(const Ray& ray_in, const HitRecord& in_rec, const UV& in_uv, const Vec3& in_p) const
{
    if (in_rec.front_face)
    {
        return emit->Value(in_uv, in_p);
    }
    else
    {
        return Color{ 0.0 };
    }
}

} // namespace spt