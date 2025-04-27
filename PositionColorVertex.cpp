#include "PositionColorVertex.h"

namespace triangles
{

PositionColorVertex::PositionColorVertex(float x, float y, float z, glm::vec4 color)
    : Position{x, y, z}, Color{color}
{
}

} // namespace triangles
