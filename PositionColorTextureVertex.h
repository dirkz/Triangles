#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorTextureVertex
{
    PositionColorTextureVertex(float x, float y, float z, glm::vec4 color, float u, float v);

    glm::vec3 m_position;
    glm::vec4 m_color;
    glm::vec2 m_texture;

    friend std::hash;
    friend std::equal_to;
};

} // namespace triangles

template <> struct std::hash<triangles::PositionColorTextureVertex>
{
    size_t operator()(const triangles::PositionColorTextureVertex &vertex) const noexcept
    {
        size_t seed = 1997;
        auto hfloat = std::hash<float>{};

        boost::hash_combine(seed, hfloat(vertex.m_position.x));
        boost::hash_combine(seed, hfloat(vertex.m_position.y));
        boost::hash_combine(seed, hfloat(vertex.m_position.z));

        boost::hash_combine(seed, hfloat(vertex.m_color.x));
        boost::hash_combine(seed, hfloat(vertex.m_color.y));
        boost::hash_combine(seed, hfloat(vertex.m_color.y));
        boost::hash_combine(seed, hfloat(vertex.m_color.w));

        boost::hash_combine(seed, hfloat(vertex.m_texture.x));
        boost::hash_combine(seed, hfloat(vertex.m_texture.y));

        return seed;
    }
};

template <> struct std::equal_to<triangles::PositionColorTextureVertex>
{
    bool operator()(const triangles::PositionColorTextureVertex &v1,
                    const triangles::PositionColorTextureVertex &v2) const
    {
        glm::bvec3 equalPosition = glm::equal(v1.m_position, v2.m_position);
        glm::bvec3 equalColor = glm::equal(v1.m_color, v2.m_color);
        glm::bvec2 equalTexture = glm::equal(v1.m_texture, v2.m_texture);

        bool bPosition = glm::all(equalPosition);
        bool bColor = glm::all(equalColor);
        bool bTexture = glm::all(equalTexture);

        return bPosition && bColor && bTexture;
    }
};
