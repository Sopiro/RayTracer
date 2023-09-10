#pragma once

#include "math.h"

namespace spt
{

// https://www.pcg-random.org/
inline u32 PCGHash(u32 rng_state)
{
    u32 state = rng_state * 747796405u + 2891336453u;
    u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

inline u32 Rand(u32& seed)
{
    seed = PCGHash(seed);
    return seed;
}

inline thread_local u32 seed = 1234;

inline void Srand(u32 new_seed)
{
    seed = new_seed;
}

inline Real Rand()
{
    return Rand(seed) / Real(UINT32_MAX);
}

inline Real Rand(Real min, Real max)
{
    return min + (max - min) * Rand();
}

inline Vec2 RandVec2()
{
    return Vec2{ Rand(), Rand() };
}

inline Vec2 RandVec2(Real min, Real max)
{
    return Vec2{ Rand(min, max), Rand(min, max) };
}

inline Vec3 RandVec3()
{
    return Vec3{ Rand(), Rand(), Rand() };
}

inline Vec3 RandVec3(Real min, Real max)
{
    return Vec3{ Rand(min, max), Rand(min, max), Rand(min, max) };
}

inline Vec3 UniformSampleSphere()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real z = 1.0 - 2.0 * u1;
    Real r = sqrt(fmax(0.0, 1.0 - z * z));
    Real phi = two_pi * u2;

    Real x = r * cos(phi);
    Real y = r * sin(phi);

    return Vec3{ x, y, z };
}

// z > 0
inline Vec3 CosineSampleHemisphere()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real z = sqrt(Real(1.0) - u2);

    Real phi = two_pi * u1;
    Real su2 = sqrt(u2);
    Real x = cos(phi) * su2;
    Real y = sin(phi) * su2;

    return Vec3{ x, y, z };
}

inline Vec3 RandomInUnitSphere()
{
#if 0
    Real u1 = Rand();
    Real u2 = Rand();

    Real theta = two_pi * u1;
    Real phi = acos(2.0 * u2 - 1.0);

    Real sin_phi = sin(phi);

    Real x = sin_phi * cos(theta);
    Real y = sin_phi * sin(theta);
    Real z = cos(phi);

    return Vec3{ x, y, z };
#else
    // Rejection sampling
    Vec3 p;
    do
    {
        p = RandVec3(Real(-1.0), Real(1.0));
    }
    while (p.Length2() >= Real(1.0));

    return p;
#endif
}

inline Vec3 UniformSampleUnitDiskXY()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real r = sqrt(u1);
    Real theta = two_pi * u2;
    return Vec3{ r * cos(theta), r * sin(theta), Real(0.0) };
}

} // namespace spt