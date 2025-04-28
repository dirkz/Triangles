#include "PositionColorVertex.h"

namespace triangles
{

using namespace DirectX;

PositionColorVertex::PositionColorVertex(float x, float y, float z, DirectX::FXMVECTOR colorVector)
    : X{x}, Y{y}, Z{z}
{
    XMFLOAT4 color;
    XMStoreFloat4(&color, colorVector);
    R = color.x;
    G = color.y;
    B = color.z;
    A = color.w;
}

} // namespace triangles
