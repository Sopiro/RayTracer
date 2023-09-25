#pragma once

#include "bvh.h"
#include "model.h"
#include "triangle.h"

namespace spt
{

// Represents aggregate primitive
class Aggregate : public Primitive
{
public:
    virtual ~Aggregate() = default;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    virtual void Sample(Intersection* sample, f64* pdf) const override;
    virtual void Sample(Intersection* sample, f64* pdf, Vec3* ref2p, const Point3& ref) const override;

    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;

    virtual const Material* GetMaterial() const override;

    void Add(const Ref<Primitive> object);
    void Add(const Ref<Mesh> mesh);
    void Add(const Ref<Model> model);

    void Reset();
    void Rebuild();

private:
    BVH bvh;

    std::vector<Ref<Primitive>> objects;
};

inline void Aggregate::GetAABB(AABB* out_aabb) const
{
    if (bvh.nodeCount == 0)
    {
        out_aabb->min.SetZero();
        out_aabb->max.SetZero();
    }

    *out_aabb = bvh.nodes[bvh.root].aabb;
}

inline void Aggregate::Sample(Intersection* sample, f64* pdf) const
{
    assert(false);
}

inline void Aggregate::Sample(Intersection* sample, f64* pdf, Vec3* ref2p, const Point3& ref) const
{
    size_t count = objects.size();
    size_t index = std::min(size_t(Rand() * count), count - 1);
    Primitive* object = objects[index].get();

    object->Sample(sample, pdf, ref2p, ref);

    *pdf = EvaluatePDF(Ray(ref, *ref2p));
}

inline f64 Aggregate::EvaluatePDF(const Ray& ray) const
{
    struct Callback
    {
        f64 sum;
        f64 weight;

        Intersection is;

        f64 RayCastCallback(const Ray& ray, f64 t_min, f64 t_max, Intersectable* object)
        {
            bool hit = object->Intersect(&is, ray, t_min, t_max);

            if (hit)
            {
                sum += ((Primitive*)is.object)->PDFValue(is, ray);
            }

            return t_max;
        }
    } callback;

    callback.sum = 0.0;

    bvh.RayCast(ray, ray_epsilon, infinity, &callback);

    return callback.sum / f64(objects.size());
}

inline f64 Aggregate::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    assert(false);
    return 0.0;
}

inline const Material* Aggregate::GetMaterial() const
{
    assert(false);
    return nullptr;
}

inline void Aggregate::Add(const Ref<Primitive> object)
{
    objects.push_back(object);

    AABB aabb;
    object->GetAABB(&aabb);
    bvh.CreateNode(object.get(), aabb);
}

inline void Aggregate::Add(const Ref<Mesh> mesh)
{
    for (i32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        objects.push_back(tri);

        AABB aabb;
        tri->GetAABB(&aabb);
        bvh.CreateNode(tri.get(), aabb);
    }
}

inline void Aggregate::Add(const Ref<Model> model)
{
    auto& meshes = model->GetMeshes();

    for (size_t i = 0; i < meshes.size(); ++i)
    {
        Add(meshes[i]);
    }
}

inline void Aggregate::Reset()
{
    bvh.Reset();
    objects.clear();
}

inline void Aggregate::Rebuild()
{
    bvh.Rebuild();
}

} // namespace spt
