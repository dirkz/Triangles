#include "PositionColorTextureVertex.h"

namespace triangles
{

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z, glm::vec4 color,
                                                       float u, float v)
    : Position{x, y, z}, Color{color}, Texture{u, v}
{
}

PositionColorTextureVertex PositionColorTextureVertex::Translated(float x, float y, float z)
{
    return PositionColorTextureVertex(Position.x + x, Position.y + y, Position.z + z, Color,
                                      Texture.x, Texture.y);
}

PositionColorTextureVertex PositionColorTextureVertex::TranslatedZ(float z)
{
    return Translated(0, 0, z);
}

PositionColorTextureVertex PositionColorTextureVertex::WithTexture(float u, float v)
{
    return PositionColorTextureVertex(Position.x, Position.y, Position.z, Color, u, v);
}

PositionColorTextureVertex PositionColorTextureVertex::WithColor(glm::vec4 color)
{
    return PositionColorTextureVertex(Position.x, Position.y, Position.z, color, Texture.x,
                                      Texture.y);
}

} // namespace triangles
