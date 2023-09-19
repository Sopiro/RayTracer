#include "spt/pathtracer.h"

namespace spt
{

void CornellBoxLucy(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color{ .65, .05, .05 });
    auto green = CreateSharedRef<Lambertian>(Color{ .12, .45, .15 });
    auto blue = CreateSharedRef<Lambertian>(Color{ .22, .23, .75 });
    auto white = CreateSharedRef<Lambertian>(Color{ .73, .73, .73 });
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color{ 1.0 });

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3{ 0.5, 0.5, -1.0 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(CreateRectXY(tf, white));

        // left
        tf = Transform{ Vec3{ 0.0, 0.5, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3{ 1.0, 0.5, -0.5 }, Quat{ pi, y_axis }, Vec3{ 1.0 } };
        scene.Add(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3{ 0.5, 0, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        auto bottom = CreateRectXZ(tf, light);
        scene.AddAreaLight(bottom);

        // top
        tf = Transform{ Vec3{ 0.5, 1.0, -0.5 }, Quat{ pi, x_axis }, Vec3{ 1.0 } };
        scene.Add(CreateRectXZ(tf, white));
    }

    // // Lights
    // {
    //     auto tf = Transform{ 0.5, 0.999, -0.5, Quat{ pi, x_axis }, Vec3{ 0.25 } };
    //     auto l = RectXZ(tf, light);

    //     scene.Add(l);
    //     scene.AddLight(l);
    // }

    {
        // Lucy
        Transform transform{ Point3{ 0.5, 0.0, -0.5 }, Quat{ identity }, Vec3{ 0.85 } };
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor_map = SolidColor::Create(Color{ 1.0 });
        mat->metallic_map = SolidColor::Create(Color{ 1.0 });
        mat->roughness_map = SolidColor::Create(Color{ 0.2 });

        // auto mat = CreateSharedRef<Dielectric>(1.5);

        Material::fallback = mat;
        Ref<Model> model = CreateSharedRef<Model>("res/stanford/lucy.obj", transform);
        scene.Add(model);
    }
}

} // namespace spt