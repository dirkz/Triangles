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

  private:
    SDL_Surface *m_surface;
};

} // namespace triangles
