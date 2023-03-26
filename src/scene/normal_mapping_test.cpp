#include "raytracer/raytracer.h"

namespace spt
{

void NormalMapping(Scene& scene)
{
    Transform transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ DegToRad(0.0), Vec3{ 0.0, 1.0, 0.0 } }, Vec3{ 0.02 } };
    std::shared_ptr<Model> sponza = std::make_shared<Model>("res/pbr_kabuto_samurai_helmet/scene.gltf", transform);

    scene.Add(sponza);

    scene.SetSkyColor(Color{ 0.3, 0.3, 0.3 });
}

}