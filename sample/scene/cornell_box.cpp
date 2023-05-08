#include "spt/pathtracer.h"

namespace spt
{

void CornellBox(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color{ .65, .05, .05 });
    auto green = CreateSharedRef<Lambertian>(Color{ .12, .45, .15 });
    auto blue = CreateSharedRef<Lambertian>(Color{ .22, .23, .75 });
    auto white = CreateSharedRef<Lambertian>(Color{ .73, .73, .73 });
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color{ 15.0 });

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3{ 0.5, 0.5, -1.0 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3{ 0.0, 0.5, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectYZ(tf, red));

        // right
        tf = Transform{ Vec3{ 1.0, 0.5, -0.5 }, Quat{ pi, y_axis }, Vec3{ 1.0 } };
        scene.Add(RectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3{ 0.5, 0, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectXZ(tf, white));

        // top
        tf = Transform{ Vec3{ 0.5, 1.0, -0.5 }, Quat{ pi, x_axis }, Vec3{ 1.0 } };
        scene.Add(RectXZ(tf, white));
    }

    // Left block
    {
        double hx = 0.13;
        double hy = 0.26;
        double hz = 0.13;

        auto tf = Transform{ 0.3, hy, -0.6, Quat(DegToRad(25.0), y_axis), Vec3{ hx * 2.0, hy * 2.0, hz * 2.0 } };
        auto box = Box(tf, white);

        scene.Add(box);
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.999, -0.5, Quat{ pi, x_axis }, Vec3{ 0.25 } };
        auto l = RectXZ(tf, light);

        scene.Add(l);
        scene.AddLight(l);
    }

    // Right sphere
    {
        // auto mat = SharedRef<Dielectric>(1.5);
        // auto sphere = SharedRef<Sphere>(Vec3(0.65, 0.15, -0.3), 0.15, mat);

        // scene.Add(sphere);
        // scene.AddLight(sphere);
    }

    // Right block
    {
        double hx = 0.13;
        double hy = 0.13;
        double hz = 0.13;

        auto tf = Transform{ 0.7, hy, -0.3, Quat(DegToRad(-25.0), y_axis), Vec3{ hx * 2.0, hy * 2.0, hz * 2.0 } };
        auto box = Box(tf, white);

        scene.Add(box);
    }

    scene.SetEnvironmentMap(SolidColor::Create(Vec3{ 0.0, 0.0, 0.0 }));

    // scene.Rebuild();
    // std::cout << "Lights: " << scene.GetLights().GetCount() << std::endl;
}

} // namespace spt