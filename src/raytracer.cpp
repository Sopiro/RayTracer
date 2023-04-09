#pragma once

#include "raytracer/raytracer.h"

#define SAMPLE_ALL_LIGHTS 0
#define MIN_BOUNCES 2
#define MAX_REJECTION 20

namespace spt
{

Color PathTrace(const Scene& scene, Ray ray, size_t bounce_count)
{
    Color accu{ 0.0 };
    Color abso{ 1.0 };
    bool was_specular = false;

    for (size_t bounce = 0; bounce < bounce_count; ++bounce)
    {
        HitRecord rec;
        if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
        {
            accu += scene.GetSkyColor(ray.dir) * abso;
            break;
        }

        Color emitted = rec.mat->Emit(ray, rec);

        ScatterRecord srec;
        if (rec.mat->Scatter(ray, rec, srec) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                accu += emitted * abso;
            }
            break;
        }

        if (srec.is_specular == true)
        {
            abso *= srec.attenuation;
            ray = srec.specular_ray;
            was_specular = true;
            continue;
        }
        else
        {
            was_specular = false;
        }

        // Direct illuminations

        if (scene.HasDirectionalLight())
        {
            auto sun = scene.GetDirectionalLight();
            Ray to_sun{ rec.point + rec.normal * ray_tolerance, -sun->dir };
            HitRecord rec2;
            if (scene.Hit(to_sun, ray_tolerance, infinity, rec2) == false)
            {
                accu += rec.mat->Evaluate(ray, rec, to_sun) * sun->radiance * abso;
            }
        }

#if SAMPLE_ALL_LIGHTS
        if (scene.HasLights())
        {
            // Multiple importance sampling

            // Sampling all lights
            auto& lights = scene.GetLights().GetObjects();
            for (size_t i = 0; i < lights.size(); ++i)
            {
                HittablePDF light_pdf{ lights[i].get(), rec.point };

                // Importance sample lights
                Ray to_light{ rec.point, light_pdf.Generate() };
                if (Dot(to_light.dir, rec.normal) > 0.0)
                {
                    double light_p = light_pdf.Evaluate(to_light.dir);
                    assert(light_p > 0.0);

                    double light_brdf_p = srec.pdf->Evaluate(to_light.dir);
                    if (light_brdf_p > 0.0)
                    {
                        double light_w = PowerHeuristic(light_p, light_brdf_p);

                        HitRecord rec2;
                        if (scene.Hit(to_light, ray_tolerance, infinity, rec2))
                        {
                            accu +=
                                rec2.mat->Emit(to_light, rec2) * rec.mat->Evaluate(ray, rec, to_light) * abso * light_w / light_p;
                        }
                    }
                }

                // Importance sample BRDF
                Ray scattered{ rec.point, srec.pdf->Generate() };
                if (Dot(scattered.dir, rec.normal) > 0.0)
                {
                    double brdf_p = srec.pdf->Evaluate(scattered.dir);
                    assert(brdf_p > 0.0);

                    double brdf_light_p = light_pdf.Evaluate(scattered.dir);
                    if (brdf_light_p > 0.0)
                    {
                        double brdf_w = PowerHeuristic(brdf_p, brdf_light_p);

                        HitRecord rec2;
                        if (scene.Hit(scattered, ray_tolerance, infinity, rec2))
                        {
                            accu +=
                                rec2.mat->Emit(scattered, rec2) * rec.mat->Evaluate(ray, rec, scattered) * abso * brdf_w / brdf_p;
                        }
                    }
                }
            }
        }
#else
        if (scene.HasLights())
        {
            // Multiple importance sampling

            HittablePDF light_pdf{ &scene.GetLights(), rec.point };

            // Importance sample lights
            Ray to_light{ rec.point, light_pdf.Generate() };
            if (Dot(to_light.dir, rec.normal) > 0.0)
            {
                double light_p = light_pdf.Evaluate(to_light.dir);
                assert(light_p > 0.0);

                double light_brdf_p = srec.pdf->Evaluate(to_light.dir);
                if (light_brdf_p > 0.0)
                {
                    double light_w = PowerHeuristic(light_p, light_brdf_p);

                    HitRecord rec2;
                    if (scene.Hit(to_light, ray_tolerance, infinity, rec2))
                    {
                        accu += rec2.mat->Emit(to_light, rec2) * rec.mat->Evaluate(ray, rec, to_light) * abso * light_w / light_p;
                    }
                }
            }

            // Importance sample BRDF
            Ray scattered{ rec.point, srec.pdf->Generate() };
            if (Dot(scattered.dir, rec.normal) > 0.0)
            {
                double brdf_p = srec.pdf->Evaluate(scattered.dir);
                assert(brdf_p > 0.0);

                double brdf_light_p = light_pdf.Evaluate(scattered.dir);
                if (brdf_light_p > 0.0)
                {
                    double brdf_w = PowerHeuristic(brdf_p, brdf_light_p);

                    HitRecord rec2;
                    if (scene.Hit(scattered, ray_tolerance, infinity, rec2))
                    {
                        accu += rec2.mat->Emit(scattered, rec2) * rec.mat->Evaluate(ray, rec, scattered) * abso * brdf_w / brdf_p;
                    }
                }
            }
        }
#endif

        // Sample new search direction based on BRDF
#if 0
        Vec3 new_direction = srec.pdf->Generate();
        double pdf_value;

        if (Dot(rec.normal, new_direction) > 0.0)
        {
            pdf_value = srec.pdf->Evaluate(new_direction);
        }
        else
        {
            CosinePDF diffusePDF{ rec.normal };
            new_direction = diffusePDF.Generate();
            pdf_value = diffusePDF.Evaluate(new_direction);
        }
#else
        int32 count = 0;
        Vec3 new_direction;
        do
        {
            new_direction = srec.pdf->Generate();
        }
        while (Dot(rec.normal, new_direction) <= 0.0 && count++ < MAX_REJECTION);

        if (count >= MAX_REJECTION)
        {
            break;
        }

        double pdf_value = srec.pdf->Evaluate(new_direction);
#endif

        assert(pdf_value > 0.0);
        Ray scattered{ rec.point, new_direction };

        accu += emitted * abso;
        abso *= rec.mat->Evaluate(ray, rec, scattered) / pdf_value;
        ray = scattered;

        // Russian roulette
        if (bounce > MIN_BOUNCES)
        {
            double rr = fmax(abso.x, fmax(abso.y, abso.z));
            // double rr = Luma(abso);
            if (Rand() > rr)
            {
                break;
            }

            abso *= 1.0 / rr;
        }
    }

    return accu;
}

} // namespace spt
