#pragma once

#include "stdafx.h"

namespace triangles
{

struct Triangle
{
    static std::vector<Triangle> Triangulate(const std::vector<Triangle> &triangles);
    static std::vector<Triangle> Triangulate(const std::vector<Triangle> &triangles, int count);

    Triangle(DirectX::FXMVECTOR p0, DirectX::FXMVECTOR p1, DirectX::FXMVECTOR p2);

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

  private:
    DirectX::XMVECTOR m_p0;
    DirectX::XMVECTOR m_p1;
    DirectX::XMVECTOR m_p2;
};

} // namespace triangles
