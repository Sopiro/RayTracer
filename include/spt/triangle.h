#pragma once

#include "common.h"
#include "hittable.h"
#include "ray.h"

namespace spt
{

struct Vertex
{
    Point3 position;
    Vec3 normal;
    Vec3 tangent;
    UV texCoord;
};

class Triangle : public Hittable
{
public:
    Triangle() = default;
    Triangle(const Point3& point0, const Point3& point1, const Point3& point2, const Ref<Material>& material);
    Triangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const Ref<Material>& material);

    Vec3 GetNormal(f64 u, f64 v, f64 w) const;
    Vec3 GetTangent(f64 u, f64 v, f64 w) const;
    UV GetTexCoord(f64 u, f64 v, f64 w) const;

    virtual bool Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual f64 PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual i32 GetSize() const override;

public:
    Vertex v0, v1, v2;
    Vec3 e1, e2;

    Vec3 face_normal;
    bool two_sided;
    f64 area;
    Ref<Material> material;
};

inline Triangle::Triangle(const Point3& p0, const Point3& p1, const Point3& p2, const Ref<Material>& material)
    : two_sided{ true }
    , material{ material }
{
    e1 = p1 - p0;
    e2 = p2 - p0;
    face_normal = Cross(e1, e2);
    area = face_normal.Normalize() * 0.5;

    // Setup vertices
    {
        Vec3 tangent = e1.Normalized();

        v0.position = p0;
        v1.position = p1;
        v2.position = p2;

        v0.normal = face_normal;
        v1.normal = face_normal;
        v2.normal = face_normal;

        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;

        v0.texCoord.SetZero();
        v1.texCoord.SetZero();
        v2.texCoord.SetZero();
    }
}

inline Triangle::Triangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const Ref<Material>& material)
    : v0{ vertex0 }
    , v1{ vertex1 }
    , v2{ vertex2 }
    , two_sided{ true }
    , material{ material }
{
    e1 = v1.position - v0.position;
    e2 = v2.position - v0.position;

    face_normal = Cross(e1, e2);
    area = face_normal.Normalize() * 0.5;
};

inline Vec3 Triangle::GetNormal(f64 u, f64 v, f64 w) const
{
    return (w * v0.normal + u * v1.normal + v * v2.normal).Normalized();
}

inline Vec3 Triangle::GetTangent(f64 u, f64 v, f64 w) const
{
    return (w * v0.tangent + u * v1.tangent + v * v2.tangent).Normalized();
}

inline UV Triangle::GetTexCoord(f64 u, f64 v, f64 w) const
{
    return w * v0.texCoord + u * v1.texCoord + v * v2.texCoord;
}

static constexpr Vec3 aabb_offset{ epsilon * 10.0 };

inline bool Triangle::GetAABB(AABB& outAABB) const
{
    outAABB.min = Min(Min(v0.position, v1.position), v2.position) - aabb_offset;
    outAABB.max = Max(Max(v0.position, v1.position), v2.position) + aabb_offset;

    return true;
}

inline f64 Triangle::EvaluatePDF(const Ray& ray) const
{
    HitRecord rec;
    if (Hit(ray, ray_offset, infinity, rec) == false)
    {
        return 0.0;
    }

    return PDFValue(ray, rec);
}

inline f64 Triangle::PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const
{
    f64 distance_squared = hit_rec.t * hit_rec.t * hit_ray.dir.Length2();
    f64 cosine = fabs(Dot(hit_ray.dir, hit_rec.normal) / hit_ray.dir.Length());

    return distance_squared / (cosine * area);
}

inline Vec3 Triangle::GetRandomDirection(const Point3& origin) const
{
    f64 u = Rand(0.0, 1.0);
    f64 v = Rand(0.0, 1.0);

    if (u + v > 1.0)
    {
        u = 1.0 - u;
        v = 1.0 - v;
    }

    Point3 random_point = v0.position + e1 * u + e2 * v;

    return (random_point - origin).Normalized();
}

inline i32 Triangle::GetSize() const
{
    return 1;
}

} // namespace spt