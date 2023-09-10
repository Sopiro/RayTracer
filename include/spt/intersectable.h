#pragma once

#include "aabb.h"
#include "common.h"
#include "ray.h"

namespace spt
{

typedef i32 NodeProxy;
class Material;
class Intersectable;

struct Intersection
{
    f64 t;
    bool front_face;

    const Intersectable* object;

    Point3 point;
    Vec3 normal, tangent;
    UV uv;
};

inline void SetFaceNormal(Intersection* is, const Ray& ray, const Vec3& outward_normal, const Vec3& outward_tangent)
{
    if (Dot(ray.dir, outward_normal) < 0.0)
    {
        is->front_face = true;
        is->normal = outward_normal;
        is->tangent = outward_tangent;
    }
    else
    {
        is->front_face = false;
        is->normal = -outward_normal;
        is->tangent = -outward_tangent;
    }
}

class Intersectable
{
public:
    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const = 0;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const = 0;

    // Returns false if it's invalid
    virtual bool GetAABB(AABB* out_aabb) const = 0;

    // Returns random point on the surface
    virtual Vec3 Sample() const;

    // Returns random direction toward this object
    virtual Vec3 Sample(const Point3& origin) const;

    virtual f64 EvaluatePDF(const Ray& ray) const;

    // Input ray must hit this object
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const;

    // Returns the total object count
    virtual i32 GetSize() const;

    virtual void Rebuild();

    virtual const Material* GetMaterial() const;

protected:
    // BVH node proxy
    NodeProxy node;
};

inline Vec3 Intersectable::Sample() const
{
    assert(false);
    return zero_vec3;
}

// Returns random direction toward this object
inline Vec3 Intersectable::Sample(const Point3& origin) const
{
    assert(false);
    return zero_vec3;
}

inline f64 Intersectable::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0.0;
}

// Ray must hit this object
inline f64 Intersectable::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    assert(false);
    return 0.0;
}

inline i32 Intersectable::GetSize() const
{
    assert(false);
    return 0;
}

inline void Intersectable::Rebuild()
{
    assert(false);
}

inline const Material* Intersectable::GetMaterial() const
{
    assert(false);
    return nullptr;
}

} // namespace spt
