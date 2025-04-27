#include "PositionColorTextureVertex.h"

namespace triangles
{

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z, glm::vec4 color,
                                                       float u, float v)
    : Position{x, y, z}, Color{color}, Texture{u, v}
{
}

} // namespace triangles