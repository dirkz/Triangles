#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorTextureVertex
{
    PositionColorTextureVertex(float x, float y, float z, DirectX::FXMVECTOR colorVector, float u,
                               float v);

    float X, Y, Z;
    float R, G, B, A;
    float U, V;
};

} // namespace triangles

template <> struct std::hash<triangles::PositionColorTextureVertex>
{
    size_t operator()(const triangles::PositionColorTextureVertex &vertex) const noexcept
    {
        size_t seed = 1997;
        auto hfloat = std::hash<float>{};

        boost::hash_combine(seed, hfloat(vertex.X));
        boost::hash_combine(seed, hfloat(vertex.Y));
        boost::hash_combine(seed, hfloat(vertex.Z));

        boost::hash_combine(seed, hfloat(vertex.R));
        boost::hash_combine(seed, hfloat(vertex.G));
        boost::hash_combine(seed, hfloat(vertex.B));
        boost::hash_combine(seed, hfloat(vertex.A));

        boost::hash_combine(seed, hfloat(vertex.U));
        boost::hash_combine(seed, hfloat(vertex.V));

        return seed;
    }
};

template <> struct std::equal_to<triangles::PositionColorTextureVertex>
{
    bool operator()(const triangles::PositionColorTextureVertex &v1,
                    const triangles::PositionColorTextureVertex &v2) const
    {
        bool equalPosition = v1.X == v2.X && v1.Y == v2.Y && v1.Z == v2.Z;
        bool equalColor = v1.R == v2.R && v1.G == v2.G && v1.B == v2.B && v1.A == v2.A;
        bool equalTexture = v1.U == v2.U && v1.V == v2.V;

        return equalPosition && equalColor && equalTexture;
    }
};
