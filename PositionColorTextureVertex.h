#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorTextureVertex
{
    PositionColorTextureVertex(float x, float y, float z, glm::vec4 color, float u, float v);

    PositionColorTextureVertex Translated(float x, float y, float z);
    PositionColorTextureVertex TranslatedZ(float z);
    PositionColorTextureVertex WithTexture(float u, float v);
    PositionColorTextureVertex WithColor(glm::vec4 color);

    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 Texture;
};

} // namespace triangles

template <> struct std::hash<triangles::PositionColorTextureVertex>
{
    size_t operator()(const triangles::PositionColorTextureVertex &vertex) const noexcept
    {
        size_t seed = 1997;
        auto hfloat = std::hash<float>{};

        boost::hash_combine(seed, hfloat(vertex.Position.x));
        boost::hash_combine(seed, hfloat(vertex.Position.y));
        boost::hash_combine(seed, hfloat(vertex.Position.z));

        boost::hash_combine(seed, hfloat(vertex.Color.x));
        boost::hash_combine(seed, hfloat(vertex.Color.y));
        boost::hash_combine(seed, hfloat(vertex.Color.y));
        boost::hash_combine(seed, hfloat(vertex.Color.w));

        boost::hash_combine(seed, hfloat(vertex.Texture.x));
        boost::hash_combine(seed, hfloat(vertex.Texture.y));

        return seed;
    }
};

template <> struct std::equal_to<triangles::PositionColorTextureVertex>
{
    bool operator()(const triangles::PositionColorTextureVertex &v1,
                    const triangles::PositionColorTextureVertex &v2) const
    {
        glm::bvec3 equalPosition = glm::equal(v1.Position, v2.Position);
        glm::bvec3 equalColor = glm::equal(v1.Color, v2.Color);
        glm::bvec2 equalTexture = glm::equal(v1.Texture, v2.Texture);

        bool bPosition = glm::all(equalPosition);
        bool bColor = glm::all(equalColor);
        bool bTexture = glm::all(equalTexture);

        return bPosition && bColor && bTexture;
    }
};
