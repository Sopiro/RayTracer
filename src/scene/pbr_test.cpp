#include "raytracer/raytracer.h"

namespace spt
{

void PBRTest(Scene& scene)
{
    // Srand(1234);

    double r = 0.3;
    double cx = 10.0;
    double cz = 7.0;
    double xgap = 0.16;
    double zgap = 0.14;
    double xstep = 2.0 * r + xgap;
    double zstep = 2.0 * r + zgap;

    for (int32 z = 0; z < cz; ++z)
    {
        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.0;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = z * zstep - ((cz - 1) * zstep / 2.0);

            auto mat = RandomPBRMaterial();
            scene.Add(std::make_shared<Sphere>(pos, r, mat));
        }
    }

    // Ground
    {
        auto mat = std::make_shared<PBRMaterial>();
        mat->albedo_map = SolidColor::Create(Vec3{ 1.0 } * 0.9);
        mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
        mat->roughness_map = SolidColor::Create(Vec3{ 0.1 });
        mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });
        mat->ao_map = SolidColor::Create(Vec3{ 0.0 });
        mat->emissive_map = SolidColor::Create(Vec3{ 0.0 });

        auto tf1 = Transform{ Vec3{ 0.5, -r, -0.5 }, Quat{ identity }, Vec3{ 100.0 } };
        auto ground = RectXZ(tf1, mat);

        scene.Add(ground);
    }

    // Light
    {
        auto tf2 = Transform{ Vec3{ -4.0, 2.5, 0.0 }, Quat{ DegToRad(-40.0), z_axis }, Vec3{ 1.0, 1.0, 4.0 } };
        auto light = std::make_shared<DiffuseLight>(Color{ 10.0 });
        auto rect = RectYZ(tf2, light);

        scene.Add(rect);
        scene.AddLight(rect);
    }

    // {
    //     auto tf2 = Transform{ Vec3{ 4.0, 2.5, 0.0 }, Quat{ DegToRad(180 + 50), z_axis }, Vec3{ 1.0, 1.0, 4.0 } };
    //     auto light = std::make_shared<DiffuseLight>(Color{ 8.0 });
    //     auto rect = RectYZ(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf2 = Transform{ Vec3{ 0.0, 2.5, -4.0 }, Quat{ DegToRad(40), x_axis }, Vec3{ 4.0, 1.0, 1.0 } };
    //     auto light = std::make_shared<DiffuseLight>(Color{ 8.0 });
    //     auto rect = RectXY(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    scene.SetEnvironmentMap(SolidColor::Create({ 0.0, 0.0, 0.0 }));
}

} // namespace spt