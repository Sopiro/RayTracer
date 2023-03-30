#include "raytracer/raytracer.h"

namespace spt
{

void NormalMapping(Scene& scene)
{
    Transform transform{ zero_vec3, Quat{ DegToRad(0.0), y_axis }, Vec3{ 0.02 } };
    std::shared_ptr<Model> model = std::make_shared<Model>("res/pbr_kabuto_samurai_helmet/scene.gltf", transform);

    scene.Add(model);

    scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
}

} // namespace spt