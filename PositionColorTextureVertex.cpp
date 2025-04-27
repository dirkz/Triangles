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
    glm::bvec3 lessThanPosition = glm::lessThan(m_position, other.m_position);
    bool bPosition = glm::all(lessThanPosition);
    return glm::all(glm::lessThan(m_position, other.m_position)) &&
           glm::all(glm::lessThan(m_color, other.m_color)) &&
           glm::all(glm::lessThan(m_texture, other.m_texture));
}

} // namespace triangles