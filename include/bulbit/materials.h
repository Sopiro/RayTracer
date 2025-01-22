#pragma once

#include "material.h"

namespace bulbit
{

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(
        const SpectrumTexture* albedo, const SpectrumTexture* normalmap = nullptr, const FloatTexture* alpha = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

public:
    const SpectrumTexture* albedo;

    const SpectrumTexture* normalmap;
    const FloatTexture* alpha;
};

class MirrorMaterial : public Material
{
public:
    MirrorMaterial(
        const SpectrumTexture* reflectance, const SpectrumTexture* normalmap = nullptr, const FloatTexture* alpha = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

public:
    const SpectrumTexture* reflectance;

    const SpectrumTexture* normalmap;
    const FloatTexture* alpha;
};

class DielectricMaterial : public Material
{
public:
    DielectricMaterial(
        Float eta, const FloatTexture* u_roughness, const FloatTexture* v_roughness, const SpectrumTexture* normalmap = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    Float eta;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* normalmap;
};

class ThinDielectricMaterial : public Material
{
public:
    ThinDielectricMaterial(Float eta);

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    Float eta;
};

class ConductorMaterial : public Material
{
public:
    ConductorMaterial(
        const SpectrumTexture* eta,
        const SpectrumTexture* k,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );
    ConductorMaterial(
        const SpectrumTexture* reflectance,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* eta;
    const SpectrumTexture* k;
};

class UnrealMaterial : public Material
{
public:
    UnrealMaterial(
        const SpectrumTexture* basecolor,
        const FloatTexture* metallic,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* emissive = nullptr,
        const SpectrumTexture* normalmap = nullptr,
        const FloatTexture* alpha = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* basecolor;
    const FloatTexture* metallic;
    const SpectrumTexture* emissive;

    const SpectrumTexture* normalmap;
    const FloatTexture* alpha;
};

class DiffuseLightMaterial : public Material
{
public:
    DiffuseLightMaterial(const SpectrumTexture* emission, bool two_sided = false, const FloatTexture* alpha = nullptr);

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

    bool two_sided;

private:
    const SpectrumTexture* emission;
    const FloatTexture* alpha;
};

class MixtureMaterial : public Material
{
public:
    MixtureMaterial(const Material* material1, const Material* material2, const FloatTexture* mix);

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

    const Material* ChooseMaterial(const Intersection& isect, const Vec3& wo) const;

private:
    const Material* materials[2];
    const FloatTexture* mixture_amount;
};

class SubsurfaceDiffusionMaterial : public Material
{
public:
    SubsurfaceDiffusionMaterial(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* reflectance;
    Spectrum l; // Mean free path
    Float eta;
};

class SubsurfaceRandomWalkMaterial : public Material
{
public:
    SubsurfaceRandomWalkMaterial(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        Float g = 0,
        const SpectrumTexture* normalmap = nullptr
    );

    Float GetAlpha(const Intersection& isect) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* reflectance;
    Spectrum l; // Mean free path
    Float eta;
    Float g;
};

inline Float Material::GetAlpha(const Intersection& isect) const
{
    return Dispatch([&](auto mat) { return mat->GetAlpha(isect); });
}

inline const SpectrumTexture* Material::GetNormalMap() const
{
    return Dispatch([](auto mat) { return mat->GetNormalMap(); });
}

inline Spectrum Material::Le(const Intersection& isect, const Vec3& wo) const
{
    return Dispatch([&](auto mat) { return mat->Le(isect, wo); });
}

inline bool Material::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return Dispatch([&](auto mat) { return mat->GetBSDF(bsdf, isect, wo, alloc); });
}

inline bool Material::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return Dispatch([&](auto mat) { return mat->GetBSSRDF(bssrdf, isect, wo, alloc); });
}

} // namespace bulbit
