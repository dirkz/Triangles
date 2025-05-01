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

Triangle::Triangle(DirectX::FXMVECTOR p0, DirectX::FXMVECTOR p1, DirectX::FXMVECTOR p2,
                   DirectX::FXMVECTOR t0, DirectX::FXMVECTOR t1, DirectX::FXMVECTOR t2)
    : m_p0{p0}, m_p1{p1}, m_p2{p2}, m_tc0{t0}, m_tc1{t1}, m_tc2{t2}
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
    constexpr float halfWay = 0.5f;

    XMVECTOR m1 = lerp(m_p1, m_p0, halfWay);
    XMVECTOR m2 = lerp(m_p0, m_p2, halfWay);
    XMVECTOR m3 = lerp(m_p2, m_p1, halfWay);

    XMVECTOR tc1 = lerp(m_tc1, m_tc0, halfWay);
    XMVECTOR tc2 = lerp(m_tc0, m_tc2, halfWay);
    XMVECTOR tc3 = lerp(m_tc2, m_tc1, halfWay);

    return {Triangle{m_p0, m1, m2, m_tc0, tc1, tc2}, Triangle{m1, m3, m2, tc1, tc2, tc3},
            Triangle{m1, m_p1, m3, tc1, m_tc1, tc3}, Triangle{m3, m_p2, m2, tc3, m_tc2, tc2}};
}

} // namespace triangles