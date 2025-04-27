#include "PositionColorTextureVertex.h"

namespace triangles
{

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z, glm::vec4 color,
                                                       float u, float v)
    : m_position{x, y, z}, m_color{color}, m_texture{u, v}
{
}

bool PositionColorTextureVertex::Less::operator()(const PositionColorTextureVertex &v1,
                                                  const PositionColorTextureVertex &v2) const
{
    return false;
}

} // namespace triangles