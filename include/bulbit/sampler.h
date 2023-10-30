#pragma once

#include "common.h"

namespace bulbit
{

class Sampler
{
public:
    Sampler(int32 samples_per_pixel);

    virtual void StartPixel();
    virtual bool StartNextPixelSample();

    virtual Float Next1D() = 0;
    virtual Point2 Next2D() = 0;

    virtual std::unique_ptr<Sampler> Clone(int32 seed) = 0;

    const int32 samples_per_pixel;

protected:
    int32 current_pixel_sample;
};

inline Sampler::Sampler(int32 spp)
    : samples_per_pixel{ spp }
{
}

void Sampler::StartPixel()
{
    current_pixel_sample = 0;
}

bool Sampler::StartNextPixelSample()
{
    return ++current_pixel_sample != samples_per_pixel;
}

} // namespace bulbit
