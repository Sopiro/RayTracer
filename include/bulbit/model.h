#pragma once

#include "mesh.h"
#include "transform.h"

namespace bulbit
{

class Texture;
class Material;

// simply a collection of meshes
class Model
{
public:
    Model(const std::string& filename, const Transform& transform);
    virtual ~Model() = default;

    const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;

private:
    friend class Scene;

    std::vector<Texture*> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb);
    const Material* CreateMaterial(const aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void Load(const std::string& filename, const Transform& transform);

    std::string folder;

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Material>> materials;
};

inline const std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes() const
{
    return meshes;
}

} // namespace bulbit