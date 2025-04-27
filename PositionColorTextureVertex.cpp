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
    return m_position.x < other.m_position.x && m_position.y < other.m_position.y &&
           m_position.z < other.m_position.z && m_color.x < other.m_color.x &&
           m_color.y < other.m_color.y && m_color.z < other.m_color.z &&
           m_color.w < other.m_color.w && m_texture.x < other.m_texture.x &&
           m_texture.y < other.m_texture.y;
}

} // namespace triangles