#include "Triangles.h"

namespace triangles
{

Triangles::Triangles()
{
    sdl::Init(SDL_INIT_VIDEO);
    SDL_DisplayID display = sdl::GetPrimaryDisplay();
    const SDL_DisplayMode *mode = sdl::GetCurrentDisplayMode(display);
    int width = mode->w / 2;
    int height = mode->h / 2;
    m_window = sdl::CreateWindow("Triangles", width, height, 0);
    sdl::SetWindowPosition(m_window, mode->w / 4, 50);
}

void Triangles::AppIterate()
{
}

void Triangles::AppEvent(SDL_Event *event)
{
}

} // namespace triangles