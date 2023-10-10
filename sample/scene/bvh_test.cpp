#include "spt/spt.h"

namespace spt
{

void BVHTest(Scene& scene)
{
    auto gray = CreateSharedRef<Lambertian>(Spectrum(0.8, 0.8, 0.8));
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65, .05, .05));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12, .45, .15));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22, .23, .75));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73, .73, .73));
    auto black = CreateSharedRef<Lambertian>(Spectrum(0.0));

    Float n = 100.0;
    Float w = 7.0;
    Float h = w * 9.0 / 16.0;
    Float r = 0.05;

    for (int32 y = 0; y < n; ++y)
    {
        for (int32 x = 0; x < n; ++x)
        {
            Vec3 pos;
            // pos.x = x / n * w - w / 2.0;
            // pos.y = y / n * w - w / 2.0;

            pos.x = Rand(-w, w);
            pos.y = Rand(-h, h);
            pos.z = -1;

            scene.Add(CreateSharedRef<Sphere>(pos, r, green));
        }
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
}

} // namespace spt