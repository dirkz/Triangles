#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorVertex
{
    PositionColorVertex(float x, float y, float z, glm::vec4 color);

    glm::vec3 m_position;
    float m_padding = 0;
    glm::vec4 m_color;
};

} // namespace triangles
