#pragma once

#include "common.h"
#include "spectrum.h"

namespace bulbit
{

// Functions for microfacet BRDF

// Default reflectance of dielectrics
constexpr Vec3 default_reflectance(Float(0.04));
constexpr Float min_alpha = Float(0.002);

inline Float RoughnessToAlpha(Float roughness)
{
    return std::fmax(roughness * roughness, min_alpha);
}

inline Spectrum F0(Spectrum basecolor, Float metallic)
{
    return Lerp(default_reflectance, basecolor, metallic);
}

inline Spectrum F_Schlick(Spectrum f0, Float cosine_theta)
{
    return f0 + (/*f90*/ Spectrum(1) - f0) * std::pow(1 - cosine_theta, Float(5.0));
}

// Trowbridge-Reitz distribution
inline Float D_GGX(Float NoH, Float alpha2)
{
    Float b = (NoH * NoH * (alpha2 - 1) + 1);
    return alpha2 * inv_pi / (b * b + Float(1e-7));
}

inline Float G1_Smith(Float NoV, Float alpha2)
{
    return 2 * NoV / (NoV + std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV));
}

inline Float G2_Smith_Correlated(Float NoV, Float NoL, Float alpha2)
{
    Float g1 = NoV * std::sqrt(alpha2 + (1 - alpha2) * NoL * NoL);
    Float g2 = NoL * std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV);
    return 2 * NoL * NoV / (g1 + g2);
}

inline Float V_Smith_Correlated(Float NoV, Float NoL, Float alpha2)
{
    Float g1 = NoV * std::sqrt(alpha2 + (1 - alpha2) * NoL * NoL);
    Float g2 = NoL * std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV);
    return Float(0.5) / (g1 + g2);
}

} // namespace bulbit
