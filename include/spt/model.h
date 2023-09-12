#pragma once

#include "common.h"
#include "mesh.h"
#include "transform.h"

namespace spt
{

class Model
{
public:
    Model(const std::string& path, const Transform& transform);
    virtual ~Model() = default;

    size_t GetMeshCount() const;
    const std::vector<Ref<Mesh>>& GetMeshes();

private:
    friend class Scene;

    std::vector<Ref<Texture>> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool srgb);
    Ref<Mesh> ProcessAssimpMesh(const aiMesh* mesh, const aiScene* scene, const Mat4& transform);
    void ProcessAssimpNode(const aiNode* node, const aiScene* scene, const Mat4& parent_transform);
    void LoadModel(const std::string& path, const Transform& transform);

    std::string folder;
    std::vector<Ref<Mesh>> meshes;
};

inline size_t Model::GetMeshCount() const
{
    return meshes.size();
}

inline const std::vector<Ref<Mesh>>& Model::GetMeshes()
{
    return meshes;
}

} // namespace spt