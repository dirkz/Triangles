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

PositionColorTextureVertex PositionColorTextureVertex::Translated(float x, float y, float z)
{
    XMVECTOR color = XMVectorSet(R, G, B, A);
    return PositionColorTextureVertex{X + x, Y + y, Z + z, color, U, V};
}

PositionColorTextureVertex PositionColorTextureVertex::TranslatedZ(float z)
{
    return Translated(0, 0, z);
}

PositionColorTextureVertex PositionColorTextureVertex::WithTexture(float u, float v)
{
    XMVECTOR color = XMVectorSet(R, G, B, A);
    return PositionColorTextureVertex{X, Y, Z, color, u, v};
}

PositionColorTextureVertex XM_CALLCONV PositionColorTextureVertex::WithColor(FXMVECTOR color)
{
    return PositionColorTextureVertex{X, Y, Z, color, U, V};
}

} // namespace triangles
