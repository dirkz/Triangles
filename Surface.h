#pragma once

#include "stdafx.h"

namespace triangles
{

struct Surface
{
    Surface(int width, int height);
    ~Surface();

    void SetPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void SetPixel(int x, int y, Uint32 pixel);

    inline SDL_Surface* GetSurface() const
    {
        return m_surface;
    }

    inline int Width() const
    {
        return m_surface->w;
    }

    inline int Height() const
    {
        return m_surface->h;
    }

    inline int Pitch() const
    {
        return m_surface->pitch;
    }

  private:
    SDL_Surface *m_surface = nullptr;
};

} // namespace triangles
