#include "Noise.h"

namespace triangles
{

using namespace glm;

constexpr float D = 1.f / 1.41421f;
constexpr std::array<vec2, 8> Gradients{vec2{0, 1},  vec2{D, D},   vec2{1, 0},  vec2{D, -D},
                                        vec2{0, -1}, vec2{-D, -D}, vec2{-1, 0}, vec2{-D, D}};

template <class T> T lerp(T a, T b, T factor)
{
    return a + ((b - a) * factor);
}

template <class T> T clamp(T x, T y, T lower = 0, T upper = 1)
{
    if (x < lower)
    {
        return lower;
    }

    if (x > upper)
    {
        return upper;
    }

    return x;
}

Noise::Noise()
{
}

float Noise::operator()(float x, float y)
{
    return 0.0f;
}

} // namespace triangles