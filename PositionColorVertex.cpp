#include "PositionColorVertex.h"

namespace triangles
{

PositionColorVertex::PositionColorVertex(float x, float y, float z, glm::vec4 color)
    : m_position{x, y, z}, m_color{color}
{
}

} // namespace triangles
