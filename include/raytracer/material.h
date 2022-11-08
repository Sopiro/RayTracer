#pragma once

#include "common.h"
#include "ray.h"

struct HitRecord;

class Material
{
public:
    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color& a)
        : albedo{ a }
    {
    }

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

public:
    Color albedo;
};

class Metal : public Material
{
public:
    Metal(const Color& a, double f)
        : albedo{ a }
        , fuzziness{ f }
    {
    }

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

public:
    Color albedo;
    double fuzziness;
};

class Dielectric : public Material
{
public:
    double ir; // Index of Refraction

    Dielectric(double index_of_refraction)
        : ir{ index_of_refraction }
    {
    }

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

private:
    static double Reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;

        return r0 + (1.0 - r0) * pow((1 - cosine), 5.0);
    }
};
