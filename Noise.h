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
    glm::vec2 Gradient(int i, int j);

    std::vector<int> m_gradientIndices;
};

} // namespace triangles
