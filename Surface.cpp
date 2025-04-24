#include "Surface.h"

namespace triangles
{

constexpr int BytesPerPixel = 4;

Surface::Surface(int width, int height)
{
    m_surface = sdl::CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
}

Surface::~Surface()
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
    }
}

void Surface::SetPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Uint32 pixel = r + (g << 8) + (b << 16) + (a << 24);
    SetPixel(x, y, pixel);
}

void Surface::SetPixel(int x, int y, Uint32 pixel)
{
    Uint8 *base = static_cast<Uint8 *>(m_surface->pixels);
    Uint8 *row = base + m_surface->pitch * y;
    Uint32 *pPixel = reinterpret_cast<Uint32 *>(row + x * BytesPerPixel);
    *pPixel = pixel;
}

} // namespace triangles