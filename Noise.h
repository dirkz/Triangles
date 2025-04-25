#pragma once

#include "stdafx.h"

namespace triangles
{

constexpr int NumGradients = 256;

struct Noise
{
    Noise();

    float operator()(float x, float y);

  private:
    glm::vec2 Gradient(int i);

    std::array<int, NumGradients> m_gradientIndices;
};

} // namespace triangles
