#pragma once

#include "bvh.h"
#include "common.h"
#include "directional_light.h"
#include "hittable.h"
#include "hittable_list.h"
#include "image_texture.h"
#include "solid_color.h"
#include "sphere.h"

namespace spt
{

class Scene : public Hittable
{
public:
    Scene();

    void Reset();
    void Add(const Ref<Hittable>& object);
    void AddLight(const Ref<Hittable>& object);

    virtual bool Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual void Rebuild() override;

    const HittableList& GetHittableList() const;

    bool HasAreaLights() const;
    const HittableList& GetAreaLights() const;

    const Ref<Texture>& GetEnvironmentMap() const;
    void SetEnvironmentMap(const Ref<Texture> color);
    Color GetSkyColor(Vec3 direction) const;

    bool HasDirectionalLight() const;
    const Ref<DirectionalLight>& GetDirectionalLight() const;
    void SetDirectionalLight(const Ref<DirectionalLight>& directional_light);

private:
    HittableList hittables;
    HittableList lights;

    Ref<Texture> environment_map;
    Ref<DirectionalLight> directional_light;
};

inline Scene::Scene()
    : environment_map{ SolidColor::Create(Color{ 0.0, 0.0, 0.0 }) }
    , directional_light{ nullptr }
{
}

inline void Scene::Reset()
{
    hittables.Clear();
    lights.Clear();
}

inline void Scene::Add(const Ref<Hittable>& object)
{
    hittables.Add(object);
}

inline void Scene::AddLight(const Ref<Hittable>& object)
{
    lights.Add(object);
}

inline void Scene::Rebuild()
{
    hittables.Rebuild();
}

inline bool Scene::Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const
{
    return hittables.Hit(ray, t_min, t_max, rec);
}

inline bool Scene::GetAABB(AABB& outAABB) const
{
    return hittables.GetAABB(outAABB);
}

inline f64 Scene::EvaluatePDF(const Ray& ray) const
{
    return hittables.EvaluatePDF(ray);
}

inline Vec3 Scene::GetRandomDirection(const Point3& origin) const
{
    return hittables.GetRandomDirection(origin);
}

inline const HittableList& Scene::GetHittableList() const
{
    return hittables;
}

inline bool Scene::HasAreaLights() const
{
    return lights.GetCount() > 0;
}

inline const HittableList& Scene::GetAreaLights() const
{
    return lights;
}

inline const Ref<Texture>& Scene::GetEnvironmentMap() const
{
    return environment_map;
}

inline void Scene::SetEnvironmentMap(const Ref<Texture> env_map)
{
    environment_map = env_map;
}

inline Color Scene::GetSkyColor(Vec3 dir) const
{
    dir.Normalize();

    f64 phi = atan2(-dir.z, dir.x) + pi;
    f64 theta = acos(-dir.y);

    f64 u = phi * inv_two_pi;
    f64 v = theta * inv_pi;

    return environment_map->Value(UV{ u, v }, zero_vec3);
}

inline bool Scene::HasDirectionalLight() const
{
    return directional_light != nullptr;
}

inline const Ref<DirectionalLight>& Scene::GetDirectionalLight() const
{
    return directional_light;
}

inline void Scene::SetDirectionalLight(const Ref<DirectionalLight>& dr)
{
    directional_light = dr;
}

} // namespace spt