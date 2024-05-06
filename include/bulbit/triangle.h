#pragma once

#include "mesh.h"
#include "primitive.h"
#include "ray.h"

namespace bulbit
{

class Triangle : public Primitive
{
public:
    Triangle(const Mesh* mesh, size_t tri_index);

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    virtual PrimitiveSample Sample(const Point2& u) const override;
    virtual PrimitiveSample Sample(const Point3& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Float PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;

    virtual const Material* GetMaterial() const override;

private:
    friend class Scene;

    Vec3 GetNormal(Float u, Float v, Float w) const;
    Vec3 GetTangent(Float u, Float v, Float w) const;
    Point2 GetTexCoord(Float u, Float v, Float w) const;

    const Mesh* mesh;
    const int32* v;
};

inline Triangle::Triangle(const Mesh* mesh, size_t tri_index)
    : mesh{ mesh }
{
    v = &mesh->indices[tri_index * 3];
}

inline AABB Triangle::GetAABB() const
{
    const Vec3 aabb_offset{ epsilon * 10.0 };

    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 min = Min(Min(p0, p1), p2) - aabb_offset;
    Vec3 max = Max(Max(p0, p1), p2) + aabb_offset;

    return AABB(min, max);
}

inline Float Triangle::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, Ray::epsilon, infinity) == false)
    {
        return 0.0;
    }

    return PDFValue(is, ray);
}

inline Float Triangle::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    Float distance_squared = hit_is.t * hit_is.t * hit_ray.d.Length2();
    Float cosine = std::fabs(Dot(hit_ray.d, hit_is.normal) / hit_ray.d.Length());

    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Float area = Float(0.5) * Cross(e1, e2).Length();

    return distance_squared / (cosine * area);
}

inline const Material* Triangle::GetMaterial() const
{
    return mesh->material;
}

inline Vec3 Triangle::GetNormal(Float _u, Float _v, Float _w) const
{
    const Vec3& n0 = mesh->normals[v[0]];
    const Vec3& n1 = mesh->normals[v[1]];
    const Vec3& n2 = mesh->normals[v[2]];

    Vec3 n = _w * n0 + _u * n1 + _v * n2;
    n.Normalize();

    return n;
}

inline Vec3 Triangle::GetTangent(Float _u, Float _v, Float _w) const
{
    const Vec3& t0 = mesh->tangents[v[0]];
    const Vec3& t1 = mesh->tangents[v[1]];
    const Vec3& t2 = mesh->tangents[v[2]];

    Vec3 t = _w * t0 + _u * t1 + _v * t2;
    t.Normalize();

    return t;
}

inline Point2 Triangle::GetTexCoord(Float _u, Float _v, Float _w) const
{
    const Point2& u0 = mesh->texCoords[v[0]];
    const Point2& u1 = mesh->texCoords[v[1]];
    const Point2& u2 = mesh->texCoords[v[2]];

    Point2 uv = _w * u0 + _u * u1 + _v * u2;

    return uv;
}

} // namespace bulbit