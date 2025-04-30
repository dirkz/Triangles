#pragma once

#include "stdafx.h"

namespace triangles
{

struct Triangle
{
    static std::vector<Triangle> Triangulate(const std::vector<Triangle> &triangles);
    static std::vector<Triangle> Triangulate(const std::vector<Triangle> &triangles, int count);

    Triangle(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2);

    void Normalize(float distance);

    std::array<Triangle, 4> Triangulate() const;

    inline DirectX::XMVECTOR V0() const
    {
        return m_v0;
    }

    inline DirectX::XMVECTOR V1() const
    {
        return m_v1;
    }

    inline DirectX::XMVECTOR V2() const
    {
        return m_v2;
    }

    inline std::array<DirectX::XMVECTOR, 3> Vectors()
    {
        return {m_v0, m_v1, m_v2};
    }

  private:
    DirectX::XMVECTOR m_v0;
    DirectX::XMVECTOR m_v1;
    DirectX::XMVECTOR m_v2;
};

} // namespace triangles
