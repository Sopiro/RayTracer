#include "spt/pathtracer.h"

namespace spt
{

void ShipScene(Scene& scene)
{
    // Table
    {
        auto tf = Transform{ Point3{ 0.0, -2.2, 0.0 }, Quat{ DegToRad(0.0), y_axis }, Vec3{ 5.0 } };
        auto model = CreateSharedRef<Model>("res/wooden_table_02_4k/wooden_table_02_4k.gltf", tf);

        scene.Add(model);
    }

    // Ship
    {
        auto tf = Transform{ Point3{ 0.0, 2.0, 0.0 }, Quat{ DegToRad(90.0), y_axis }, Vec3{ 0.1 } };
        auto model = CreateSharedRef<Model>("res/ship_pinnace_4k/ship_pinnace_4k.gltf", tf);

        scene.Add(model);
    }

    // Light
    {
        double size = 0.5;

        auto white = CreateSharedRef<DiffuseLight>(SolidColor::Create(Color{ 30.0 }));
        white->two_sided = true;
        auto tf = Transform{ Point3{ 0.0, 5.0, -3.0 }, Quat{ pi / 4.0, x_axis }, Vec3{ size } };
        auto rect = RectXY(tf, white);

        scene.Add(rect);
        scene.AddLight(rect);

        tf = Transform{ Point3{ 0.0, 5.0, 3.0 }, Quat{ pi - pi / 4.0, x_axis }, Vec3{ size } };
        rect = RectXY(tf, white);

        scene.Add(rect);
        scene.AddLight(rect);

        tf = Transform{ Point3{ -3.0, 5.0, 0.0 }, Quat{ -pi / 4.0, z_axis }, Vec3{ size } };
        rect = RectYZ(tf, white);

        scene.Add(rect);
        scene.AddLight(rect);

        tf = Transform{ Point3{ 3.0, 5.0, 0.0 }, Quat{ pi + pi / 4.0, z_axis }, Vec3{ size } };
        rect = RectYZ(tf, white);

        scene.Add(rect);
        scene.AddLight(rect);
    }

    // Floor
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });
        mat->roughness_map = SolidColor::Create(Vec3{ 0.001 });

        double size = 9.0;
        double y = 2.1;

        auto tf = Transform{ Point3{ 0.0, y - size / 2.0, 0.0 }, Quat{ identity }, Vec3{ size } };
        auto rect = RectXZ(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3{ -size / 2.0, y, 0.0 }, Quat{ identity }, Vec3{ size } };
        rect = RectYZ(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3{ 0.0, y, -size / 2.0 }, Quat{ identity }, Vec3{ size } };
        rect = RectXY(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3{ 0.0, y - size / 2.0, 0.0 }, Quat{ identity }, Vec3{ size } };
        rect = RectXZ(tf, mat);
        scene.Add(rect);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.SetEnvironmentMap(SolidColor::Create(Color{ 0.0 }));
}

} // namespace spt