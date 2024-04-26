#pragma once

#include "transform.h"

namespace bulbit
{

class Mesh;
class Material;
class Scene;

void CreateTriangles(Scene& scene, std::unique_ptr<Mesh> mesh);
void CreateRectXY(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));
void CreateRectXZ(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));
void CreateRectYZ(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));
void CreateBox(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));

} // namespace bulbit