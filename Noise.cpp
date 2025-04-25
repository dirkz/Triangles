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

    for (int i = 0; i < GradientsRowPitch * GradientsRowPitch; ++i)
    {
        m_gradientIndices[i] = dist(rng);
    }
}

float Noise::operator()(float x, float y)
{
    int i = static_cast<int>(std::floor(x));
    int j = static_cast<int>(std::floor(y));

    vec2 g00 = Gradient(i, j);
    vec2 g10 = Gradient(i + 1, j);
    vec2 g01 = Gradient(i, j + 1);
    vec2 g11 = Gradient(i + 1, j + 1);

    float u = x - static_cast<float>(i);
    float v = y - static_cast<float>(j);

    float n00 = glm::dot(g00, vec2{u, v});
    float n10 = glm::dot(g10, vec2{u - 1, v});
    float n01 = glm::dot(g01, vec2{u, v - 1});
    float n11 = glm::dot(g11, vec2{u - 1, v - 1});

    float nx0 = n00 * (1 - smootherstep(u)) + n10 * smootherstep(u);
    float nx1 = n01 * (1 - smootherstep(u)) + n11 * smootherstep(u);
    float nxy = nx0 * (1 - smootherstep(v)) + nx1 * smootherstep(v);

    return nxy;
}

glm::vec2 Noise::Gradient(int i, int j)
{
    return Gradients[m_gradientIndices[i + j * GradientsRowPitch]];
}

} // namespace triangles