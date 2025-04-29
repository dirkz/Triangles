#include "PositionColorTextureVertex.h"

namespace triangles
{

using namespace DirectX;

PositionColorTextureVertex::PositionColorTextureVertex(float x, float y, float z,
                                                       DirectX::FXMVECTOR colorVector, float u,
                                                       float v)
    : PositionColorTextureVertex{XMVectorSet(x, y, z, 1), colorVector, u, v}
{
}

PositionColorTextureVertex::PositionColorTextureVertex(DirectX::FXMVECTOR p,
                                                       DirectX::FXMVECTOR colorVector, float u,
                                                       float v)
    : U{u}, V{v}
{
    XMFLOAT4 vector;
    XMStoreFloat4(&vector, p);
    X = vector.x;
    Y = vector.y;
    Z = vector.z;

    XMFLOAT4 color;
    XMStoreFloat4(&color, colorVector);
    R = color.x;
    G = color.y;
    B = color.z;
    A = color.w;
}

} // namespace triangles
