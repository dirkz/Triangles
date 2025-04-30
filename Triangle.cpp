#include "Triangle.h"

namespace triangles
{

using namespace DirectX;

Triangle::Triangle(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
    : m_v0{v0}, m_v1{v1}, m_v2{v2}
{
}

void Triangle::Normalize(float length)
{
    m_v0 = XMVectorScale(XMVector3Normalize(m_v0), length);
    m_v1 = XMVectorScale(XMVector3Normalize(m_v1), length);
    m_v2 = XMVectorScale(XMVector3Normalize(m_v2), length);
}

} // namespace triangles