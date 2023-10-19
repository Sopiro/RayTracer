#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

void BVHTest(Scene& scene)
{
    auto gray = CreateSharedRef<Lambertian>(Spectrum(0.8f, 0.8f, 0.8f));
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto black = CreateSharedRef<Lambertian>(Spectrum(0.0f));

    Float n = 100.0f;
    Float w = 7.0f;
    Float h = w * 9.0f / 16.0f;
    Float r = 0.05f;

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

} // namespace bulbit