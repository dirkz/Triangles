#include "Triangle.h"

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

Triangle::Triangle(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
    : m_v0{v0}, m_v1{v1}, m_v2{v2}
{
}

void Triangle::Normalize(float distance)
{
    m_v0 = XMVectorScale(XMVector3Normalize(m_v0), distance);
    m_v1 = XMVectorScale(XMVector3Normalize(m_v1), distance);
    m_v2 = XMVectorScale(XMVector3Normalize(m_v2), distance);
}

std::array<Triangle, 4> Triangle::Triangulate() const
{
    XMVECTOR e1 = XMVectorSubtract(m_v1, m_v0);
    XMVECTOR e2 = XMVectorSubtract(m_v0, m_v2);
    XMVECTOR e3 = XMVectorSubtract(m_v2, m_v1);

    XMVECTOR m1 = XMVectorAdd(m_v0, XMVectorScale(e1, 0.5f));
    XMVECTOR m2 = XMVectorAdd(m_v2, XMVectorScale(e2, 0.5f));
    XMVECTOR m3 = XMVectorAdd(m_v1, XMVectorScale(e3, 0.5f));

    return {Triangle{m_v0, m1, m2}, Triangle{m1, m3, m2}, Triangle{m1, m_v1, m3},
            Triangle{m3, m_v2, m2}};
}

} // namespace triangles