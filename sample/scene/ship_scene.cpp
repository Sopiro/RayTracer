#include "spt/spt.h"

namespace spt
{

void ShipScene(Scene& scene)
{
    // Table
    {
        auto tf = Transform{ Point3(0.0, -2.2, 0.0), Quat(DegToRad(0.0), y_axis), Vec3(5.0) };
        auto model = CreateSharedRef<Model>("res/wooden_table_02_4k/wooden_table_02_4k.gltf", tf);

        scene.Add(model);
    }

    // Ship
    {
        auto tf = Transform{ Point3(0.0, 2.0, 0.0), Quat(DegToRad(90.0), y_axis), Vec3(0.1) };
        auto model = CreateSharedRef<Model>("res/ship_pinnace_4k/ship_pinnace_4k.gltf", tf);

        scene.Add(model);
    }

    // Light
    {
        Float size = 0.5;

        auto white = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(30.0)));
        white->two_sided = true;
        auto tf = Transform{ Point3(0.0, 5.0, -3.0), Quat(pi / 4.0, x_axis), Vec3(size) };
        auto rect = CreateRectXY(tf, white);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0, 5.0, 3.0), Quat(pi - pi / 4.0, x_axis), Vec3(size) };
        rect = CreateRectXY(tf, white);

        scene.AddLight(rect);

        tf = Transform{ Point3(-3.0, 5.0, 0.0), Quat(-pi / 4.0, z_axis), Vec3(size) };
        rect = CreateRectYZ(tf, white);

        scene.AddLight(rect);

        tf = Transform{ Point3(3.0, 5.0, 0.0), Quat(pi + pi / 4.0, z_axis), Vec3(size) };
        rect = CreateRectYZ(tf, white);

        scene.AddLight(rect);
    }

    // Floor
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0));
        mat->metallic = ConstantColor::Create(Spectrum(0.0));
        mat->roughness = ConstantColor::Create(Spectrum(0.001));

        Float size = 9.0;
        Float y = 2.1;

        auto tf = Transform{ Point3(0.0, y - size / 2.0, 0.0), identity, Vec3(size) };
        auto rect = CreateRectXZ(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3(-size / 2.0, y, 0.0), identity, Vec3(size) };
        rect = CreateRectYZ(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3(0.0, y, -size / 2.0), identity, Vec3(size) };
        rect = CreateRectXY(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3(0.0, y - size / 2.0, 0.0), identity, Vec3(size) };
        rect = CreateRectXZ(tf, mat);
        scene.Add(rect);
    }
}

} // namespace spt