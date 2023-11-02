#include "bulbit/pathtracer.h"
#include "bulbit/infinite_area_light.h"
#include "bulbit/light.h"
#include "bulbit/pdf.h"
#include "bulbit/scene.h"
#include "bulbit/util.h"

namespace bulbit
{

PathTracer::PathTracer(const Ref<Sampler> sampler, int32 bounces, Float rr)
    : SamplerIntegrator(sampler)
    , max_bounces{ bounces }
    , rr_probability{ rr }
{
}

Spectrum PathTracer::Li(const Scene& scene, const Ray& primary_ray, Sampler& sampler) const
{
    Spectrum radiance(0), throughput(1);
    bool was_specular_bounce = false;
    Ray ray = primary_ray;

    for (int32 bounce = 0;; ++bounce)
    {
        Intersection is;
        bool found_intersection = scene.Intersect(&is, ray, Ray::epsilon, infinity);
        const Material* mat = is.material;

        if (bounce == 0 || was_specular_bounce)
        {
            if (found_intersection)
            {
                if (mat->IsLightSource())
                {
                    radiance += throughput * mat->Emit(is, ray.d);
                }
            }
            else
            {
                const std::vector<InfiniteAreaLight*>& lights = scene.GetInfiniteAreaLights();
                for (InfiniteAreaLight* light : lights)
                {
                    radiance += throughput * light->Emit(ray);
                }
            }
        }

        if (found_intersection == false || bounce >= max_bounces)
        {
            break;
        }

        Interaction ir;
        if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
        {
            break;
        }

        if (was_specular_bounce = ir.is_specular)
        {
            throughput *= ir.attenuation;
            ray = ir.specular_ray;
            continue;
        }

        // BSDF
        const PDF* pdf = ir.GetScatteringPDF();

        radiance += throughput * mat->Emit(is, ray.d);

        // Estimate direct light
        // Multiple importance sampling with balance heuristic (Direct light + BRDF)

        const std::vector<Ref<Light>>& lights = scene.GetLights();
        size_t num_lights = lights.size();
        if (num_lights > 0)
        {
            // Sample one light uniformly
            size_t index = std::min(size_t(sampler.Next1D() * num_lights), num_lights - 1);
            Light* light = lights[index].get();
            Float light_weight = Float(num_lights);

            Vec3 to_light;
            Float light_pdf;
            Float visibility;
            Spectrum li = light->Sample(&to_light, &light_pdf, &visibility, is, sampler.Next2D());

            Ray shadow_ray{ is.point, to_light };

            // Importance sample light
            Float light_brdf_pdf = pdf->Evaluate(to_light);
            if (li.IsBlack() == false && light_brdf_pdf > 0)
            {
                if (scene.IntersectAny(shadow_ray, Ray::epsilon, visibility) == false)
                {
                    Spectrum f = mat->Evaluate(is, ray.d, to_light);
                    if (light->IsDeltaLight())
                    {
                        radiance += throughput * light_weight / light_pdf * li * f;
                    }
                    else
                    {
                        Float mis_weight = PowerHeuristic(1, light_pdf, 1, light_brdf_pdf);
                        radiance += throughput * light_weight * mis_weight / light_pdf * li * f;
                    }
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BRDF
                Vec3 scattered = pdf->Sample(sampler.Next2D());
                shadow_ray = Ray{ is.point, scattered };

                Float brdf_light_pdf = light->EvaluatePDF(shadow_ray);
                if (brdf_light_pdf > 0)
                {
                    Intersection is2;
                    if (scene.Intersect(&is2, shadow_ray, Ray::epsilon, infinity))
                    {
                        if (is2.object == ((AreaLight*)light)->GetPrimitive())
                        {
                            Float brdf_pdf = pdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            li = is2.material->Emit(is2, scattered);
                            if (li.IsBlack() == false)
                            {
                                Spectrum f = mat->Evaluate(is, ray.d, scattered);
                                radiance += throughput * light_weight * mis_weight / brdf_pdf * li * f;
                            }
                        }
                    }
                    else
                    {
                        li = light->Emit(shadow_ray);
                        if (li.IsBlack() == false)
                        {
                            Float brdf_pdf = pdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            Spectrum f = mat->Evaluate(is, ray.d, scattered);
                            radiance += throughput * light_weight * mis_weight / brdf_pdf * li * f;
                        }
                    }
                }
            }
        }

        // Sample new path direction based on BRDF
        Vec3 wi = pdf->Sample(sampler.Next2D());
        Float pdf_value = pdf->Evaluate(wi);
        if (pdf_value <= 0)
        {
            break;
        }

        Ray scattered{ is.point, wi };

        throughput *= mat->Evaluate(is, ray.d, wi) / pdf_value;
        ray = scattered;

        // Russian roulette
        const int32 min_bounces = 3;
        if (bounce > min_bounces)
        {
            Float rr = std::fmin(rr_probability, throughput.Luminance());
            if (sampler.Next1D() > rr)
            {
                break;
            }

            throughput *= 1 / rr;
        }
    }

    return radiance;
}

} // namespace bulbit
