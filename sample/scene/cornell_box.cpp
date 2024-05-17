#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

std::unique_ptr<Camera> CornellBox(Scene& scene)
{
    // Materials
    auto red = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.65f, .05f, .05f));
    auto green = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.12f, .45f, .15f));
    auto blue = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.22f, .23f, .75f));
    auto white = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ColorImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = scene.CreateMaterial<DiffuseMaterial>(wakgood_texture);
    auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(15.0f));
    // auto light2 = scene.CreateMaterial<DiffuseLight>(Spectrum(17.0f, 12.0f, 4.0f));
    // auto mirror = scene.CreateMaterial<Metal>(Spectrum(.73f, .73f, .73f), 0.0f);

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0f) };
        CreateRectXY(scene, tf, wakgood_mat);

        // left
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        CreateRectYZ(scene, tf, red);

        // right
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        CreateRectYZ(scene, tf, green);

        // bottom
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        CreateRectXZ(scene, tf, white);

        // top
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        CreateRectXZ(scene, tf, white);
    }

    // Left block
    {
        Float hx = 0.14f;
        Float hy = 0.28f;
        Float hz = 0.14f;

        auto tf = Transform{ 0.33f, hy, -0.66f, Quat(DegToRad(18.0f), y_axis), Vec3(hx * 2.0f, hy * 2.0f, hz * 2.0f) };
        CreateBox(scene, tf, white);
    }

    // Right block
    {
        Float hx = 0.14f;
        Float hy = 0.14f;
        Float hz = 0.14f;

        auto tf = Transform{ 0.66f, hy, -0.33f, Quat(DegToRad(-18.0f), y_axis), Vec3(hx * 2.0f, hy * 2.0f, hz * 2.0f) };
        CreateBox(scene, tf, white);
    }

    // Right sphere
    {
        // auto mat = scene.CreateMaterial<Dielectric>(1.5f);
        // scene.CreatePrimitive<Sphere>(Vec3(0.65f, 0.15f, -0.3f), 0.15f, mat);
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.995f, -0.5f, Quat(pi, x_axis), Vec3(0.25f) };
        CreateRectXZ(scene, tf, light, true);

        // scene.CreatePrimitive<Sphere>(Vec3(0.5f, 0.9f, -0.5f), 0.05f, light);
        // scene.CreateLight<PointLight>(Point3(0.5f, 0.9f, -0.5f), Spectrum(0.25f));
        // scene.CreateLight<DirectionalLight>(Normalize(-Vec3(1, 1, 1)), Vec3(5.0f), 0.05f);
        // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr");
        // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
        // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    }

    // scene.Rebuild();
    // std::cout << "Lights: " << scene.GetAreaLights().size() << std::endl;

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 1.64f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0.0f;
    Float vFov = 35.0f;

    // return std::make_unique<OrthographicCamera>(lookfrom, lookat, y_axis, 1.1, 1.1, width);
    // return std::make_unique<SphericalCamera>(lookfrom, width, width);
    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, width, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("cornell-box", CornellBox);
