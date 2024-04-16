#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = scene.CreateMaterial<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(1.0f)),
                                                    ConstantColor::Create(Spectrum(0.1f)));

        // auto mat = scene.CreateMaterial<Dielectric>(1.5f);

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(3.0f) };
        auto model = Model("res/stanford/bunny.obj", tf);

        scene.AddModel(model);
        // scene.AddLight(sphere);
    }

    // scene.CreateLight<InfiniteAreaLight>("res/HDR/scythian_tombs_2_4k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    scene.CreateLight<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi / 2, y_axis)));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1920;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 10 };
    Point3 lookat{ 0, 1, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("ggxvndf", GGXVNDFSamplingTest);

} // namespace bulbit