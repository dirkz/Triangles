#include "Noise.h"

namespace triangles
{

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
}

double Noise::operator()(double x, double y)
{
    return 0;
}

} // namespace triangles