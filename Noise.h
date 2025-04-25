#pragma once

#include "stdafx.h"

namespace triangles
{

struct Noise
{
    Noise();

    float operator()(float x, float y);
};

} // namespace triangles
