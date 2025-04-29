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
    constexpr float DistanceFromPoles = XM_PI / 10.f;
    constexpr float LimitXY = XM_PIDIV2 - DistanceFromPoles;
    constexpr float StepXY = DistanceFromPoles;
    constexpr float StepXZ = DistanceFromPoles;
    constexpr float StepU = StepXY / XM_PI;
    constexpr float StepV = StepXZ / XM_2PI;

    for (float angleXY = -LimitXY; angleXY < LimitXY; angleXY += StepXY)
    {
        for (float angleXZ = 0; angleXZ < XM_2PI - StepXZ; angleXZ += StepXZ)
        {
            XMVECTOR v0 = PointOnSphere(angleXY, angleXZ);
            XMVECTOR v1 = PointOnSphere(angleXY, angleXZ + StepXZ);
            XMVECTOR v2 = PointOnSphere(angleXY + StepXY, angleXZ + StepXZ);
            XMVECTOR v3 = PointOnSphere(angleXY + StepXY, angleXZ);

            float tu0 = angleXZ / XM_2PI;
            float tu1 = tu0 + StepV;
            float tv0 = (angleXY + XM_PIDIV2) / XM_PI;
            float tv1 = tv0 + StepU;

            PositionColorTextureVertex p0{v0, color, tu0, tv0};
            PositionColorTextureVertex p1{v1, color, tu1, tv0};
            PositionColorTextureVertex p2{v2, color, tu1, tv1};
            PositionColorTextureVertex p3{v3, color, tu0, tv1};

            m_vertexBuffer.Quad(p0, p1, p2, p3);
        }
    }
}

} // namespace triangles