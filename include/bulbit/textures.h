#pragma once

#include "hash.h"
#include "image.h"
#include "pool.h"
#include "texture.h"

namespace bulbit
{

template <typename T>
class ConstantTexture : public Texture<T>
{
public:
    ConstantTexture(T value)
        : value{ std::move(value) }
    {
    }

    T Evaluate(const Point2& uv) const
    {
        BulbitNotUsed(uv);
        return value;
    }

private:
    T value;
};

template <typename T>
class ImageTexture : public Texture<T>
{
public:
    ImageTexture(Image<T> image, TexCoordFilter texcoord_filter)
        : image{ std::move(image) }
        , texcoord_filter{ texcoord_filter }
    {
    }

    int32 GetWidth() const
    {
        return image.width;
    }

    int32 GetHeight() const
    {
        return image.height;
    }

    T Evaluate(const Point2& uv) const
    {
#if 0
        // Nearest sampling
        Float w = uv.x * image.width + 0.5f;
        Float h = uv.y * image.height + 0.5f;

        int32 i = int32(w);
        int32 j = int32(h);

        FilterTexCoord(&i, &j);

        return image[i + j * image.width];
#else
        // Bilinear sampling
        Float w = uv.x * image.width + 0.5f;
        Float h = uv.y * image.height + 0.5f;

        int32 i0 = int32(w), i1 = int32(w) + 1;
        int32 j0 = int32(h), j1 = int32(h) + 1;

        FilterTexCoord(&i0, &j0, image.width, image.height, texcoord_filter);
        FilterTexCoord(&i1, &j1, image.width, image.height, texcoord_filter);

        Float fu = w - std::floor(w);
        Float fv = h - std::floor(h);

        T v00 = image[i0 + j0 * image.width], v10 = image[i1 + j0 * image.width];
        T v01 = image[i0 + j1 * image.width], v11 = image[i1 + j1 * image.width];

        // clang-format off
    return (1-fu) * (1-fv) * v00 + (1-fu) * (fv) * v01 +
           (  fu) * (1-fv) * v10 + (  fu) * (fv) * v11;
        // clang-format on
#endif
    }

private:
    Image<T> image;
    TexCoordFilter texcoord_filter;
};

template <typename T>
class CheckerTexture : public Texture<T>
{
public:
    CheckerTexture(const Texture<T>* a, const Texture<T>* b, const Point2& resolution)
        : a{ a }
        , b{ b }
        , resolution{ resolution }
    {
    }

    virtual T Evaluate(const Point2& uv) const override
    {
        Point2i scale = uv * resolution;
        int32 c = scale.x + scale.y;

        if (c % 2)
        {
            return a->Evaluate(uv);
        }
        else
        {
            return b->Evaluate(uv);
        }
    }

private:
    const Texture<T>* a;
    const Texture<T>* b;
    Point2 resolution;
};

using FloatConstantTexture = ConstantTexture<Float>;
using SpectrumConstantTexture = ConstantTexture<Spectrum>;

using FloatImageTexture = ImageTexture<Float>;
using SpectrumImageTexture = ImageTexture<Spectrum>;

using FloatCheckerTexture = CheckerTexture<Float>;
using SpectrumCheckerTexture = CheckerTexture<Spectrum>;

namespace detail
{

template <typename T>
using key_1d = T;

template <typename T>
using key_2d = std::pair<const T*, int32>;

template <typename T>
using key_c = std::tuple<const Texture<T>*, const Texture<T>*, Point2>;

template <typename T>
struct hasher_1d
{
    size_t operator()(key_1d<T> key) const
    {
        return Hash(key);
    }
};

template <typename T>
struct hasher_2d
{
    size_t operator()(const key_2d<T>& image) const
    {
        const T* buffer = image.first;
        int32 size = image.second;
        return HashBuffer(buffer, size);
    }
};

template <typename T>
struct hasher_c
{
    size_t operator()(key_c<T> key) const
    {
        const Texture<T>* a = std::get<0>(key);
        const Texture<T>* b = std::get<1>(key);
        Point2 resolution = std::get<2>(key);

        return Hash(a, b, resolution);
    }
};

}; // namespace detail

class TexturePool
{
public:
    template <typename T>
    auto& GetPool1d()
    {
        if constexpr (std::is_same_v<T, Float>)
        {
            return pool_1d1f;
        }
        else if constexpr (std::is_same_v<T, Spectrum>)
        {
            return pool_1d3f;
        }
        else
        {
            static_assert(false, "Unsupported type for 1d");
        }
    }

    template <typename T>
    auto& GetPool2d()
    {
        if constexpr (std::is_same_v<T, Float>)
        {
            return pool_2d1f;
        }
        else if constexpr (std::is_same_v<T, Spectrum>)
        {
            return pool_2d3f;
        }
        else
        {
            static_assert(false, "Unsupported type for 2d");
        }
    }

    template <typename T>
    auto& GetPoolC()
    {
        if constexpr (std::is_same_v<T, Float>)
        {
            return pool_c1f;
        }
        else if constexpr (std::is_same_v<T, Spectrum>)
        {
            return pool_c3f;
        }
        else
        {
            static_assert(false, "Unsupported type for checker");
        }
    }

private:
    Pool<detail::key_1d<Float>, ConstantTexture<Float>, detail::hasher_1d<Float>> pool_1d1f;
    Pool<detail::key_1d<Spectrum>, ConstantTexture<Spectrum>, detail::hasher_1d<Spectrum>> pool_1d3f;

    Pool<detail::key_2d<Float>, ImageTexture<Float>, detail::hasher_2d<Float>> pool_2d1f;
    Pool<detail::key_2d<Spectrum>, ImageTexture<Spectrum>, detail::hasher_2d<Spectrum>> pool_2d3f;

    Pool<detail::key_c<Float>, CheckerTexture<Float>, detail::hasher_c<Float>> pool_c1f;
    Pool<detail::key_c<Spectrum>, CheckerTexture<Spectrum>, detail::hasher_c<Spectrum>> pool_c3f;
};

inline TexturePool texture_pool;

FloatConstantTexture* CreateFloatConstantTexture(Float value);
SpectrumConstantTexture* CreateSpectrumConstantTexture(const Spectrum& value);
SpectrumConstantTexture* CreateSpectrumConstantTexture(Float value);
SpectrumConstantTexture* CreateSpectrumConstantTexture(Float r, Float g, Float b);

FloatImageTexture* CreateFloatImageTexture(Image1 image);
SpectrumImageTexture* CreateSpectrumImageTexture(Image3 image);

FloatCheckerTexture* CreateFloatCheckerTexture(const FloatTexture* a, const FloatTexture* b, const Point2& resolution);
SpectrumCheckerTexture* CreateSpectrumCheckerTexture(
    const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution
);

} // namespace bulbit
