#pragma once

#include <array>
#include <assert.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "math.h"
#include "math_util.h"
#include "types.h"

template <typename T>
using Ref = std::shared_ptr<T>;

namespace spt
{

constexpr double ray_tolerance = 0.00001;

using Color = Vec3;
using Point = Vec3;
using UV = Vec2;

} // namespace spt