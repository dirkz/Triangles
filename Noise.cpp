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

template <class T> T smootherstep(T t)
{
    T t3 = t * t * t;
    T t4 = t3 * t;
    T t5 = t4 * t;

    return 6 * t5 - 15 * t4 + 10 * t3;
}

Noise::Noise()
{
    std::random_device dev;
    std::mt19937 rng{dev()};
    std::uniform_int_distribution<std::mt19937::result_type> dist(
        0, static_cast<int>(Gradients.size() - 1));

    for (int i = 0; i < NumGradients; ++i)
    {
        m_gradientIndices[i] = dist(rng);
    }
}

float Noise::operator()(float x, float y)
{
    return 0.0f;
}

glm::vec2 Noise::Gradient(int i)
{
    return Gradients[m_gradientIndices[i]];
}

} // namespace triangles