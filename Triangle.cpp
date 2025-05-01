#include "Triangle.h"

#include "Common.h"

namespace triangles
{

using namespace DirectX;

std::vector<Triangle> Triangle::Triangulate(const std::vector<Triangle> &triangles)
{
    std::vector<Triangle> triangulized;

    for (const Triangle &triangle : triangles)
    {
        std::array<Triangle, 4> ts = triangle.Triangulate();
        for (const Triangle &t : ts)
        {
            triangulized.push_back(t);
        }
    }

    return triangulized;
}

std::vector<Triangle> Triangle::Triangulate(const std::vector<Triangle> &triangles, int count)
{
    if (count <= 0)
    {
        return triangles;
    }

    std::vector<Triangle> result = Triangulate(triangles);

    if (count == 1)
    {
        return result;
    }

    for (int i = 1; i < count; ++i)
    {
        result = Triangulate(result);
    }

    return result;
}

Triangle::Triangle(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2,
                   DirectX::FXMVECTOR t0, DirectX::FXMVECTOR t1, DirectX::FXMVECTOR t2)
    : m_p0{v0}, m_p1{v1}, m_p2{v2}, m_tc0{t0}, m_tc1{t1}, m_tc2{t2}
{
}

void Triangle::Normalize(float distance)
{
    m_p0 = XMVectorScale(XMVector3Normalize(m_p0), distance);
    m_p1 = XMVectorScale(XMVector3Normalize(m_p1), distance);
    m_p2 = XMVectorScale(XMVector3Normalize(m_p2), distance);
}

std::array<Triangle, 4> Triangle::Triangulate() const
{
    XMVECTOR m1 = lerp(m_p1, m_p0, 0.5f);
    XMVECTOR m2 = lerp(m_p0, m_p2, 0.5f);
    XMVECTOR m3 = lerp(m_p2, m_p1, 0.5f);

    XMVECTOR t1 = lerp(m_tc1, m_tc0, 0.5f);
    XMVECTOR t2 = lerp(m_tc0, m_tc2, 0.5f);
    XMVECTOR t3 = lerp(m_tc2, m_tc1, 0.5f);

    return {Triangle{m_p0, m1, m2, m_tc0, t1, t2}, Triangle{m1, m3, m2, t1, t2, t3},
            Triangle{m1, m_p1, m3, t1, m_tc1, t3}, Triangle{m3, m_p2, m2, t3, m_tc2, t2}};
}

} // namespace triangles