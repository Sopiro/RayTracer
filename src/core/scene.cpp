#include "bulbit/scene.h"
#include "bulbit/material.h"
#include "bulbit/sphere.h"
#include "bulbit/triangle.h"

namespace bulbit
{

Scene::Scene()
    : resource{ 64 * 1024 }
    , pool{ &resource }
    , allocator{ &pool }
{
}

Scene::~Scene()
{
    for (Primitive* p : primitives)
    {
        allocator.delete_object(p);
    }
}

void Scene::AddPrimitive(const std::unique_ptr<Primitive> primitive)
{
    Primitive* p = primitive->Clone(&allocator);
    primitives.push_back(p);

    const Material* mat = GetMaterial(p->GetMaterialIndex());
    if (mat->IsLightSource())
    {
        // Create area light
        auto area_light = std::make_shared<AreaLight>(p);
        area_light->material = mat;
        lights.push_back(area_light);
    }
}

void Scene::AddMesh(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        CreatePrimitive<Triangle>(mesh, i);
    }
}

void Scene::AddModel(const Model& model)
{
    MaterialIndex offset = (MaterialIndex)materials.size();
    materials.insert(materials.end(), model.materials.begin(), model.materials.end());

    for (Ref<Mesh> mesh : model.meshes)
    {
        mesh->material += offset;
        AddMesh(mesh);
    }
}

void Scene::AddLight(const Ref<Light> light)
{
    assert(light->type != Light::Type::area_light);

    if (light->type != Light::Type::area_light)
    {
        lights.push_back(light);
    }

    if (light->type == Light::Type::infinite_area_light)
    {
        infinite_lights.push_back((InfiniteAreaLight*)light.get());
    }
}

void Scene::BuildAccelerationStructure()
{
    // accel.reset(new DynamicBVH(primitives));
    accel.reset(new BVH(primitives));
}

void Scene::Clear()
{
    accel.reset();
    primitives.clear();
    lights.clear();
    infinite_lights.clear();
}

} // namespace bulbit
