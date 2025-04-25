#pragma once

#include "stdafx.h"

namespace triangles
{

constexpr int GradientsRowPitch = 256;

struct Noise
{
    Noise();

    double operator()(double x, double y);

  private:
};

} // namespace triangles
