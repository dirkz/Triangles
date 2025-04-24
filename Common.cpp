#include "Common.h"

namespace triangles
{

SDL_Window *CreateWindow(const char *title)
{
    sdl::Init(SDL_INIT_VIDEO);

    SDL_DisplayID display = sdl::GetPrimaryDisplay();
    const SDL_DisplayMode *mode = sdl::GetCurrentDisplayMode(display);
    int width = mode->w / 2;
    int height = mode->h / 2;

    SDL_Window *window = sdl::CreateWindow(title, width, height, 0);
    sdl::SetWindowPosition(window, mode->w / 4, 100);

    return window;
}

} // namespace triangles