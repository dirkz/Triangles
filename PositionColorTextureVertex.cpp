#include "PositionColorTextureVertex.h"

namespace triangles
{

using namespace DirectX;

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z,
                                                       DirectX::FXMVECTOR colorVector, float u,
                                                       float v)
    : X{x}, Y{y}, Z{z}, U{u}, V{v}
{
    XMFLOAT4 color;
    XMStoreFloat4(&color, colorVector);
    R = color.x;
    G = color.y;
    B = color.z;
    A = color.w;
}

} // namespace triangles
