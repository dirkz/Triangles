#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorVertex
{
    PositionColorVertex(float x, float y, float z, DirectX::FXMVECTOR colorVector);

    float X, Y, Z;
    float R, G, B, A;
};

} // namespace triangles
