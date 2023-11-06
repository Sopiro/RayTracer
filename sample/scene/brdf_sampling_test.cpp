#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

Camera* BRDFSamplingTest(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Spectrum(15.0));

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0f) };
        scene.Add(CreateRectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        scene.Add(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        scene.Add(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        scene.Add(CreateRectXZ(tf, white));

        // top
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        scene.Add(CreateRectXZ(tf, white));
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.999f, -0.5f, Quat(pi, x_axis), Vec3(0.25f) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // Center sphere
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(Spectrum(1.0f)), ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(0.2f)));

        Float r = 0.25;
        auto sphere = CreateSharedRef<Sphere>(Vec3(0.5f, r, -0.5f), r, mat);

        scene.Add(sphere);
        // scene.AddLight(sphere);
    }

    // scene.Rebuild();
    // std::cout << scene.GetLights().GetCount() << std::endl;

    Float aspect_ratio = 16.0f / 9.0f;
    // Float aspect_ratio = 3.0f / 2.0f;
    // Float aspect_ratio = 4.0f / 3.0f;
    // Float aspect_ratio = 1.0f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 45;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("brdf-sampling", BRDFSamplingTest);

} // namespace bulbit