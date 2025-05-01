#pragma once

#include "stdafx.h"

namespace triangles
{

struct Triangle
{
    struct PointWithTexture
    {
        PointWithTexture(DirectX::FXMVECTOR p, DirectX::FXMVECTOR tc) : P{p}, TC{tc}
        {
        }

        DirectX::XMVECTOR P;
        DirectX::XMVECTOR TC;
    };

    static std::vector<Triangle> Triangulate(const std::vector<Triangle> &triangles);
    static std::vector<Triangle> Triangulate(const std::vector<Triangle> &triangles, int count);

    Triangle(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2,
             DirectX::FXMVECTOR t0 = DirectX::XMVectorZero(),
             DirectX::FXMVECTOR t1 = DirectX::XMVectorZero(),
             DirectX::FXMVECTOR t2 = DirectX::XMVectorZero());

    void Normalize(float distance);

    std::array<Triangle, 4> Triangulate() const;

    inline DirectX::XMVECTOR PV0() const
    {
        return m_p0;
    }

    inline DirectX::XMVECTOR P1() const
    {
        return m_p1;
    }

    inline DirectX::XMVECTOR P2() const
    {
        return m_p2;
    }

    inline std::array<DirectX::XMVECTOR, 3> Points()
    {
        return {m_p0, m_p1, m_p2};
    }

    inline std::array<PointWithTexture, 3> PointsWithTextures()
    {
        return {PointWithTexture{m_p0, m_tc0}, PointWithTexture{m_p1, m_tc1},
                PointWithTexture{m_p2, m_tc2}};
    }

  private:
    DirectX::XMVECTOR m_p0;
    DirectX::XMVECTOR m_p1;
    DirectX::XMVECTOR m_p2;

    DirectX::XMVECTOR m_tc0;
    DirectX::XMVECTOR m_tc1;
    DirectX::XMVECTOR m_tc2;
};

} // namespace triangles
