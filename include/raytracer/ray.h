#pragma once

#include "common.h"

namespace spt
{

class Ray
{
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& dir)
        : origin{ origin }
        , dir{ dir }
    {
    }

    Vec3 At(Real t) const
    {
        return origin + dir * t;
    }

public:
    Vec3 origin;
    Vec3 dir;
};

} // namespace spt