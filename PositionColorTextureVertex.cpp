#include "PositionColorTextureVertex.h"

namespace triangles
{

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z, glm::vec4 color,
                                                       float u, float v)
    : m_position{x, y, z}, m_color{color}, m_texture{u, v}
{
}

bool PositionColorTextureVertex::operator<(const PositionColorTextureVertex &other) const
{
    return false;
}

} // namespace triangles