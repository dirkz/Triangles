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

template <class T> T fade(T t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

template <class T> int fastfloor(T x)
{
    return x > 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
}

Noise::Noise()
{
    std::random_device dev;
    std::mt19937 rng{dev()};
    std::uniform_int_distribution<std::mt19937::result_type> dist(
        0, static_cast<int>(Gradients.size() - 1));

    for (int i = 0; i < GradientsRowPitch * GradientsRowPitch; ++i)
    {
        m_gradientIndices.push_back(dist(rng));
    }
}

double Noise::operator()(double x, double y)
{
    int i = fastfloor(x);
    int j = fastfloor(y);

    vec2 g00 = Gradient(i, j);
    vec2 g10 = Gradient(i + 1, j);
    vec2 g01 = Gradient(i, j + 1);
    vec2 g11 = Gradient(i + 1, j + 1);

    double u = x - static_cast<double>(i);
    double v = y - static_cast<double>(j);

    double n00 = glm::dot(g00, vec2{u, v});
    double n10 = glm::dot(g10, vec2{u - 1, v});
    double n01 = glm::dot(g01, vec2{u, v - 1});
    double n11 = glm::dot(g11, vec2{u - 1, v - 1});

    double nx0 = n00 * (1 - fade(u)) + n10 * fade(u);
    double nx1 = n01 * (1 - fade(u)) + n11 * fade(u);
    double nxy = nx0 * (1 - fade(v)) + nx1 * fade(v);

    return nxy;
}

glm::vec2 Noise::Gradient(int i, int j)
{
    return Gradients[m_gradientIndices[i + j * GradientsRowPitch]];
}

} // namespace triangles