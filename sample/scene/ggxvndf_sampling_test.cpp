#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

void GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(0.1f)));

        // auto mat = CreateSharedRef<Dielectric>(1.5f);

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(3.0f) };
        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        scene.Add(model);
        // scene.AddLight(sphere);
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/scythian_tombs_2_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
}

} // namespace bulbit