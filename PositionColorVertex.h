#pragma once

#include "stdafx.h"

namespace triangles
{

struct PositionColorVertex
{
    PositionColorVertex(float x, float y, float z, std::array<float, 4> color);

    float X, Y, Z;
    float R, G, B, A;
};

} // namespace triangles
