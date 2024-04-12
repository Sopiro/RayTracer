#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> RebootScene(Scene& scene)
{
    // https://sketchfab.com/3d-models/reboot-dramatic-54ec601a3c4e4f6d8600fd28174c527c
    {
        auto mat = std::make_shared<Microfacet>(ConstantColor::Create(0.0), ConstantColor::Create(Spectrum(0.0f)),
                                                ConstantColor::Create(Spectrum(0.001f)));

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(0.01f) };
        auto model = Model("res/reboot_dramatic_scene/scene.gltf", tf);

        scene.AddModel(model);
    }

    {
        auto red = scene.CreateMaterial<DiffuseLight>(Spectrum(14.0f, 0.0f, 0.0f));
        auto sphere = std::make_shared<Sphere>(Vec3(0.0f, 3.0f, -4.0f), 1.0f, red);

        scene.AddPrimitive(sphere);
    }

    {
        auto white = scene.CreateMaterial<DiffuseLight>(Spectrum(8.0f));
        auto tf = Transform{ Vec3(0.0f, 8.0f, 0.0f), Quat(DegToRad(180.0f), x_axis), Vec3(3.0f) };
        auto rect = CreateRectXZ(tf, white);

        scene.AddMesh(rect);
    }

    // scene.CreateLight<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/photo_studio_01_1k.hdr"));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -4, 3.5f, -4 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0.02f;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("reboot", RebootScene);

} // namespace bulbit