#include "spt/spt.h"

namespace spt
{

void MISTest1(Scene& scene)
{
    {
        auto floor = RandomMicrofacetMaterial();
        floor->basecolor = SolidColor::Create(Vec3(0.73));
        floor->roughness = SolidColor::Create(Vec3(1.0));
        floor->metallic = SolidColor::Create(Vec3(0.0));

        auto tf = Transform{ Vec3(0.0, 0.0, 0.0), Quat(identity), Vec3(3.0, 1.0, 3.0) };
        scene.Add(CreateRectXZ(tf, floor));

        tf = Transform{ Vec3(0.0, 0.0, -1.0), Quat(identity), Vec3(3.0, 3.0, 1.0) };
        scene.Add(CreateRectXY(tf, floor));
    }

    Float y = 0.05;
    Float z = 0.0;

    // plates
    {
        auto m1 = RandomMicrofacetMaterial();
        m1->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m1->metallic = SolidColor::Create(Vec3(1.0));
        m1->roughness = SolidColor::Create(Vec3(0.1));

        auto m2 = RandomMicrofacetMaterial();
        m2->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m2->metallic = SolidColor::Create(Vec3(1.0));
        m2->roughness = SolidColor::Create(Vec3(0.05));

        auto m3 = RandomMicrofacetMaterial();
        m3->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m3->metallic = SolidColor::Create(Vec3(1.0));
        m3->roughness = SolidColor::Create(Vec3(0.02));

        auto m4 = RandomMicrofacetMaterial();
        m4->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m4->metallic = SolidColor::Create(Vec3(1.0));
        m4->roughness = SolidColor::Create(Vec3(0.005));

        Float h = 0.2;
        Float dh = 0.025;
        Float w = 1.0;

        Float a1 = DegToRad(15.0);
        Float a2 = DegToRad(25.0);
        Float a3 = DegToRad(35.0);
        Float a4 = DegToRad(45.0);

        auto tf = Transform{ Vec3(0.0, y, z), Quat(a1, x_axis), Vec3(w, h, h - dh) };
        scene.Add(CreateRectXZ(tf, m1));

        y += (std::sin(a1) * h + std::sin(a2) * h) / 2.0;
        z -= (std::cos(a1) * h + std::cos(a2) * h) / 2.0;

        tf = Transform{ Vec3(0.0, y, z), Quat(a2, x_axis), Vec3(w, h, h - dh) };
        scene.Add(CreateRectXZ(tf, m2));

        y += (std::sin(a2) * h + std::sin(a3) * h) / 2.0;
        z -= (std::cos(a2) * h + std::cos(a3) * h) / 2.0;

        tf = Transform{ Vec3(0.0, y, z), Quat(a3, x_axis), Vec3(w, h, h - dh) };
        scene.Add(CreateRectXZ(tf, m3));

        y += (std::sin(a3) * h + std::sin(a4) * h) / 2.0;
        z -= (std::cos(a3) * h + std::cos(a4) * h) / 2.0;

        tf = Transform{ Vec3(0.0, y, z), Quat(a4, x_axis), Vec3(w, h, h - dh) };
        scene.Add(CreateRectXZ(tf, m4));
    }

    // std::cout << y << ", " << z << std::endl;

    // Lights
    {
        auto light1 = CreateSharedRef<DiffuseLight>(Color(3000.0));
        auto light2 = CreateSharedRef<DiffuseLight>(Color(300.0));
        auto light3 = CreateSharedRef<DiffuseLight>(Color(30.0));
        auto light4 = CreateSharedRef<DiffuseLight>(Color(3.0));

        Float lh = y;
        Float xg = 0.16;

        Float r = 0.1;
        auto l1 = CreateSharedRef<Sphere>(Vec3(-xg * 3.0, y + lh, z), r / 27.0, light1);
        auto l2 = CreateSharedRef<Sphere>(Vec3(-xg, y + lh, z), r / 9.0, light2);
        auto l3 = CreateSharedRef<Sphere>(Vec3(xg, y + lh, z), r / 3.0, light3);
        auto l4 = CreateSharedRef<Sphere>(Vec3(xg * 3.0, y + lh, z), r, light4);

        scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>(SolidColor::Create(Vec3(0.01))));
}

void MISTest2(Scene& scene)
{
    {
        auto floor_mat = RandomMicrofacetMaterial();
        floor_mat->basecolor = SolidColor::Create(Vec3(0.4));
        floor_mat->roughness = SolidColor::Create(Vec3(1.0));
        floor_mat->metallic = SolidColor::Create(Vec3(0.0));

        auto floor = CreateSharedRef<Model>("res/veach_mi/floor.obj", Transform{ identity });
        floor->GetMeshes()[0]->SetMaterial(floor_mat);
        scene.Add(floor);
    }

    // plates
    {
        auto m1 = RandomMicrofacetMaterial();
        m1->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m1->metallic = SolidColor::Create(Vec3(1.0));
        m1->roughness = SolidColor::Create(Vec3(0.005));

        auto m2 = RandomMicrofacetMaterial();
        m2->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m2->metallic = SolidColor::Create(Vec3(1.0));
        m2->roughness = SolidColor::Create(Vec3(0.02));

        auto m3 = RandomMicrofacetMaterial();
        m3->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m3->metallic = SolidColor::Create(Vec3(1.0));
        m3->roughness = SolidColor::Create(Vec3(0.05));

        auto m4 = RandomMicrofacetMaterial();
        m4->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m4->metallic = SolidColor::Create(Vec3(1.0));
        m4->roughness = SolidColor::Create(Vec3(0.1));

        auto p1 = CreateSharedRef<Model>("res/veach_mi/plate1.obj", Transform{ identity });
        p1->GetMeshes()[0]->SetMaterial(m1);
        scene.Add(p1);
        auto p2 = CreateSharedRef<Model>("res/veach_mi/plate2.obj", Transform{ identity });
        p2->GetMeshes()[0]->SetMaterial(m2);
        scene.Add(p2);
        auto p3 = CreateSharedRef<Model>("res/veach_mi/plate3.obj", Transform{ identity });
        p3->GetMeshes()[0]->SetMaterial(m3);
        scene.Add(p3);
        auto p4 = CreateSharedRef<Model>("res/veach_mi/plate4.obj", Transform{ identity });
        p4->GetMeshes()[0]->SetMaterial(m4);
        scene.Add(p4);
    }

    // Lights
    {
        auto light1 = CreateSharedRef<DiffuseLight>(Color(800));
        auto light3 = CreateSharedRef<DiffuseLight>(Color(901.803));
        auto light2 = CreateSharedRef<DiffuseLight>(Color(100));
        auto light4 = CreateSharedRef<DiffuseLight>(Color(11.1111));
        auto light5 = CreateSharedRef<DiffuseLight>(Color(1.23457));

        auto l1 = CreateSharedRef<Sphere>(Vec3(10, 10, 4), 0.5, light1);
        auto l3 = CreateSharedRef<Sphere>(Vec3(-3.75, 0, 0), 0.03333, light3);
        auto l2 = CreateSharedRef<Sphere>(Vec3(-1.25, 0, 0), 0.1, light2);
        auto l4 = CreateSharedRef<Sphere>(Vec3(1.25, 0, 0), 0.3, light4);
        auto l5 = CreateSharedRef<Sphere>(Vec3(3.75, 0, 0), 0.9, light5);

        scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
        scene.AddLight(l5);
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>(SolidColor::Create(Vec3(0.0))));
}

void MISTestWak(Scene& scene)
{
    {
        auto floor = RandomMicrofacetMaterial();
        floor->basecolor = ImageTexture::Create("res/wakdu.jpg");
        floor->roughness = SolidColor::Create(Vec3(1.0));
        floor->metallic = SolidColor::Create(Vec3(0.0));

        Float s = 20.0;
        auto tf = Transform{ Vec3(0.0, -4.0, -4.0), Quat(identity), Vec3(s, 1.0, s) };
        scene.Add(CreateRectXZ(tf, floor));

        tf = Transform{ Vec3(0.0, -4.0, -4.0), Quat(identity), Vec3(s, s, 1.0) };
        scene.Add(CreateRectXY(tf, floor));
    }

    // plates
    {
        auto m1 = RandomMicrofacetMaterial();
        m1->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m1->metallic = SolidColor::Create(Vec3(1.0));
        m1->roughness = SolidColor::Create(Vec3(0.005));

        auto m2 = RandomMicrofacetMaterial();
        m2->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m2->metallic = SolidColor::Create(Vec3(1.0));
        m2->roughness = SolidColor::Create(Vec3(0.02));

        auto m3 = RandomMicrofacetMaterial();
        m3->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m3->metallic = SolidColor::Create(Vec3(1.0));
        m3->roughness = SolidColor::Create(Vec3(0.05));

        auto m4 = RandomMicrofacetMaterial();
        m4->basecolor = SolidColor::Create(Vec3(0.07, 0.09, 0.13));
        m4->metallic = SolidColor::Create(Vec3(1.0));
        m4->roughness = SolidColor::Create(Vec3(0.1));

        auto p1 = CreateSharedRef<Model>("res/veach_mi/plate1.obj", Transform{ identity });
        p1->GetMeshes()[0]->SetMaterial(m1);
        scene.Add(p1);
        auto p2 = CreateSharedRef<Model>("res/veach_mi/plate2.obj", Transform{ identity });
        p2->GetMeshes()[0]->SetMaterial(m2);
        scene.Add(p2);
        auto p3 = CreateSharedRef<Model>("res/veach_mi/plate3.obj", Transform{ identity });
        p3->GetMeshes()[0]->SetMaterial(m3);
        scene.Add(p3);
        auto p4 = CreateSharedRef<Model>("res/veach_mi/plate4.obj", Transform{ identity });
        p4->GetMeshes()[0]->SetMaterial(m4);
        scene.Add(p4);
    }

    // Lights
    {
        auto light1 = CreateSharedRef<DiffuseLight>(Color(800));
        auto light3 = CreateSharedRef<DiffuseLight>(Color(901.803, 0, 0));
        auto light2 = CreateSharedRef<DiffuseLight>(Color(100, 100 / 5, 0));
        auto light4 = CreateSharedRef<DiffuseLight>(Color(11.1111, 11.1111, 0));
        auto light5 = CreateSharedRef<DiffuseLight>(Color(0, 1.23457, 0));

        auto l1 = CreateSharedRef<Sphere>(Vec3(10, 10, 4), 0.5, light1);
        auto l3 = CreateSharedRef<Sphere>(Vec3(-3.75, 0, 0), 0.03333, light3);
        auto l2 = CreateSharedRef<Sphere>(Vec3(-1.25, 0, 0), 0.1, light2);
        auto l4 = CreateSharedRef<Sphere>(Vec3(1.25, 0, 0), 0.3, light4);
        auto l5 = CreateSharedRef<Sphere>(Vec3(3.75, 0, 0), 0.9, light5);

        // scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
        scene.AddLight(l5);
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>(SolidColor::Create(Vec3(0.333))));
}

} // namespace spt