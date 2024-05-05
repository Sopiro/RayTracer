#pragma once

#include "camera.h"
#include "film.h"
#include "sampler.h"
#include "scene.h"

namespace bulbit
{

class Integrator
{
public:
    virtual ~Integrator() = default;

    virtual void Render(Film* film, const Camera& camera) = 0;

protected:
    Integrator(const Scene* scene, const Intersectable* accel)
        : scene{ scene }
        , accel{ accel }
    {
    }

    bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const
    {
        return accel->Intersect(out_is, ray, t_min, t_max);
    }

    bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const
    {
        return accel->IntersectAny(ray, t_min, t_max);
    }

    const Scene* scene;
    const Intersectable* accel;
};

class SamplerIntegrator : public Integrator
{
public:
    SamplerIntegrator(const Scene* scene, const Intersectable* accel, const Sampler* sampler);
    virtual ~SamplerIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const = 0;

    virtual void Render(Film* film, const Camera& camera) override;

private:
    const Sampler* sampler_prototype;
};

class DebugIntegrator : public SamplerIntegrator
{
public:
    DebugIntegrator(const Scene* scene, const Intersectable* accel, const Sampler* sampler);
    virtual ~DebugIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;
};

// Whitted-style raytracing
class WhittedStyle : public SamplerIntegrator
{
public:
    WhittedStyle(const Scene* scene, const Intersectable* accel, const Sampler* sampler, int32 max_depth);
    virtual ~WhittedStyle() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override
    {
        return Li(ray, sampler, 0);
    }

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_depth;
};

// This integrator evaluates ambient occlusion
class AmbientOcclusion : public SamplerIntegrator
{
public:
    AmbientOcclusion(const Scene* scene, const Intersectable* accel, const Sampler* sampler, Float ao_range);
    virtual ~AmbientOcclusion() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    // maximum range to consider occlusuion
    Float range;
};

class NaivePathIntegrator : public SamplerIntegrator
{
public:
    NaivePathIntegrator(const Scene* scene,
                        const Intersectable* accel,
                        const Sampler* sampler,
                        int32 max_bounces,
                        Float russian_roulette_probability = Float(0.95));
    virtual ~NaivePathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override
    {
        return Li(ray, sampler, 0);
    }

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_bounces;
    Float rr_probability;
};

// Uni-directional path tracer
class PathIntegrator : public SamplerIntegrator
{
public:
    PathIntegrator(const Scene* scene,
                   const Intersectable* accel,
                   const Sampler* sampler,
                   int32 max_bounces,
                   Float russian_roulette_probability = Float(0.95));
    virtual ~PathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    std::vector<Light*> infinite_lights;

    int32 max_bounces;
    Float rr_probability;
};

} // namespace bulbit
