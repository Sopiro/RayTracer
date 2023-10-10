#pragma once

#include "common.h"
#include "spectrum.h"

namespace spt
{

class Texture
{
public:
    virtual Color Value(const Point2& uv) const = 0;
};

} // namespace spt