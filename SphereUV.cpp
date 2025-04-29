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
    constexpr float distanceFromPoles = XM_PI / 10.f;
    constexpr float limitXY = XM_PIDIV2 - distanceFromPoles;
    constexpr float stepXY = distanceFromPoles;
    constexpr float stepXZ = distanceFromPoles;
    constexpr float stepU = stepXY / XM_PI;
    constexpr float stepV = stepXZ / XM_2PI;

    for (float angleXY = -limitXY; angleXY < limitXY; angleXY += stepXY)
    {
        for (float angleXZ = 0; angleXZ < XM_2PI - stepXZ; angleXZ += stepXZ)
        {
            XMVECTOR v0 = PointOnSphere(angleXY, angleXZ);
            XMVECTOR v1 = PointOnSphere(angleXY, angleXZ + stepXZ);
            XMVECTOR v2 = PointOnSphere(angleXY + stepXY, angleXZ + stepXZ);
            XMVECTOR v3 = PointOnSphere(angleXY + stepXY, angleXZ);

            float tu0 = (angleXY + XM_PIDIV2) / XM_PI;
            float tu1 = tu0 + stepU;
            float tv0 = angleXZ / XM_2PI;
            float tv1 = tv0 + stepV;

            PositionColorTextureVertex p0{v0, color, tu0, tv0};
            PositionColorTextureVertex p1{v1, color, tu1, tv0};
            PositionColorTextureVertex p2{v2, color, tu0, tv1};
            PositionColorTextureVertex p3{v3, color, tu1, tv1};

            m_vertexBuffer.Quad(p0, p1, p2, p3);
        }
    }
}

} // namespace triangles