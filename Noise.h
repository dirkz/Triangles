#pragma once

#include "stdafx.h"

namespace triangles
{

struct Noise
{
    double operator()(double x, double y, double z);
};

} // namespace triangles
