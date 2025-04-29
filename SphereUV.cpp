#include "SphereUV.h"

namespace triangles
{

using namespace DirectX;

static XMVECTOR PointOnSphere(float angleXY, float angleXZ)
{
    float x = std::cos(angleXY);
    float y = std::sin(angleXY);
    XMVECTOR v = XMVectorSet(x, y, 0, 1);

    XMMATRIX rotation = XMMatrixRotationY(angleXZ);
    v = XMVector3Transform(v, rotation);

    return v;
}

SphereUV::SphereUV(FXMVECTOR color)
{
    constexpr float interval = XM_PI / 10.f;
    constexpr float limitXY = XM_PIDIV2 - interval;
    constexpr float stepXY = interval;
    constexpr float stepXZ = interval;
    for (float angleXY = -limitXY; angleXY < limitXY; angleXY += stepXY)
    {
        for (float angleXZ = 0; angleXZ < XM_2PI - stepXZ; angleXZ += stepXZ)
        {
            XMVECTOR v0 = PointOnSphere(angleXY, angleXZ);
            XMVECTOR v1 = PointOnSphere(angleXY, angleXZ + stepXZ);
            XMVECTOR v2 = PointOnSphere(angleXY + stepXY, angleXZ + stepXZ);
            XMVECTOR v3 = PointOnSphere(angleXY + stepXY, angleXZ);
            float u = angleXY + XM_PIDIV2 / XM_PI;
            float v = angleXZ / XM_2PI;
        }
    }
}

} // namespace triangles