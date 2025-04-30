#include "Triangle.h"

namespace triangles
{

Triangle::Triangle(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
    : m_v0{v0}, m_v1{v1}, m_v2{v2}
{
}

} // namespace triangles