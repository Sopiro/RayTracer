#pragma once

#include "bvh.h"
#include "common.h"
#include "hittable.h"

#define USE_BVH 1

namespace spt
{

class HittableList : public Hittable
{
public:
    HittableList() = default;

    void Add(const Ref<Hittable>& object);
    void Clear();

    virtual bool Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual i32 GetSize() const override;
    virtual void Rebuild() override;

    const std::vector<Ref<Hittable>>& GetObjects() const;
    size_t GetCount() const;

private:
    BVH bvh;
    std::vector<Ref<Hittable>> objects;
};

inline void HittableList::Add(const Ref<Hittable>& object)
{
    AABB aabb;
    object->GetAABB(aabb);

    bvh.CreateNode(object.get(), aabb);
    objects.push_back(object);
}

inline void HittableList::Clear()
{
    bvh.Reset();
    objects.clear();
}

inline bool HittableList::Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const
{
#if USE_BVH
    return bvh.Hit(ray, t_min, t_max, rec);
#else
    HitRecord tmp;
    bool hit = false;
    f64 closest = t_max;

    for (const auto& object : objects)
    {
        if (object->Hit(ray, t_min, closest, tmp))
        {
            hit = true;
            closest = tmp.t;
            rec = tmp;
        }
    }

    return hit;
#endif
}

inline bool HittableList::GetAABB(AABB& outAABB) const
{
#if USE_BVH
    return bvh.GetAABB(outAABB);
#else
    if (objects.empty())
    {
        return false;
    }

    objects[0]->GetAABB(outAABB);

    AABB temp;
    for (i32 i = 1; i < objects.size(); ++i)
    {
        const auto& object = objects[i];

        if (object->GetAABB(temp) == false)
        {
            return false;
        }

        outAABB = Union(outAABB, temp);
    }

    return true;
#endif
}

inline f64 HittableList::EvaluatePDF(const Ray& ray) const
{
    return bvh.EvaluatePDF(ray);
}

inline Vec3 HittableList::GetRandomDirection(const Point3& origin) const
{
    return bvh.GetRandomDirection(origin);
}

inline i32 HittableList::GetSize() const
{
    return bvh.GetSize();
}

inline void HittableList::Rebuild()
{
    bvh.Rebuild();

    struct Callback
    {
        void TraverseCallback(const Node* node)
        {
            if (node->data)
            {
                node->data->Rebuild();
            }
        }
    } callback;

    bvh.Traverse(&callback);
}

inline const std::vector<Ref<Hittable>>& HittableList::GetObjects() const
{
    return objects;
}

inline size_t HittableList::GetCount() const
{
    return objects.size();
}

} // namespace spt