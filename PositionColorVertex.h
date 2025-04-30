#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorVertex
{
    PositionColorVertex(float x, float y, float z, DirectX::FXMVECTOR colorVector);

    float X, Y, Z;
    float R, G, B, A;
};

} // namespace triangles

template <> struct std::hash<triangles::PositionColorVertex>
{
    size_t operator()(const triangles::PositionColorVertex &vertex) const noexcept
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

        return seed;
    }
};

template <> struct std::equal_to<triangles::PositionColorVertex>
{
    bool operator()(const triangles::PositionColorVertex &v1,
                    const triangles::PositionColorVertex &v2) const
    {
        bool equalPosition = v1.X == v2.X && v1.Y == v2.Y && v1.Z == v2.Z;
        bool equalColor = v1.R == v2.R && v1.G == v2.G && v1.B == v2.B && v1.A == v2.A;

        return equalPosition && equalColor;
    }
};
