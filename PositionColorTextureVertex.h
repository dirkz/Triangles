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

    struct Less
    {
        bool operator()(const PositionColorTextureVertex &v1,
                        const PositionColorTextureVertex &v2) const;
    };

    friend Less;
};

} // namespace triangles
