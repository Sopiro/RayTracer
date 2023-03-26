#include "raytracer/mesh.h"
#include "raytracer/lambertian.h"
#include "raytracer/triangle.h"

namespace spt
{

Mesh::Mesh(std::vector<Vertex> _vertices,
           std::vector<uint32> _indices,
           std::array<std::shared_ptr<Texture>, 5> _textures,
           const Mat4& _transform)
    : vertices{ std::move(_vertices) }
    , indices{ std::move(_indices) }
    , textures{ std::move(_textures) }
    , transform{ _transform }
{
    material = std::make_shared<PBRMaterial>();

    material->albedo = HasAlbedoTexture() ? textures[albedo] : SolidColor::Create(1.0, 0.0, 1.0);
    material->normal = HasNormalTexture() ? textures[normal] : SolidColor::Create(0.5, 0.5, 1.0);
    material->roughness = HasRoughnessTexture() ? textures[roughness] : SolidColor::Create(0.1, 0.1, 0.1);
    material->metalness = HasMetalnessTexture() ? textures[metalness] : SolidColor::Create(0.0, 0.0, 0.0);
    material->ao = HasAOTexture() ? textures[ao] : SolidColor::Create(1.0, 1.0, 1.0);

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex& v = vertices[i];

        Vec4 vP = transform * Vec4{ v.position, 1.0 };
        Vec4 vN = transform * Vec4{ v.normal, 0.0 };
        Vec4 vT = transform * Vec4{ v.tangent, 0.0 };
        vN.Normalize();
        vT.Normalize();

        v.position.Set(vP.x, vP.y, vP.z);
        v.normal.Set(vN.x, vN.y, vN.z);
        v.tangent.Set(vT.x, vT.y, vT.z);
    }

    // Bake BVH

    triangles.reserve(indices.size() / 3 + 1);

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32 index0 = indices[i];
        uint32 index1 = indices[i + 1];
        uint32 index2 = indices[i + 2];

        Triangle& t = triangles.emplace_back(vertices[index0], vertices[index1], vertices[index2], material);
        AABB aabb;
        t.GetAABB(aabb);
        bvh.CreateNode(&t, aabb);
    }
}

} // namespace spt