#include "PositionColorTextureVertex.h"

namespace triangles
{

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z,
                                                       std::array<float, 4> color, float u, float v)
    : X{x}, Y{y}, Z{z}, R{color[0]}, G{color[1]}, B{color[2]}, A{color[3]}, U{u}, V{v}
{
}

} // namespace triangles