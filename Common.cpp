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

SDL_GPUDevice *CreateDevice(SDL_Window *window)
{
#ifdef _DEBUG
    bool debugMode = true;
#else
    bool debugMode = false;
#endif

    SDL_GPUDevice *device = sdl::CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL,
        debugMode, nullptr);
    sdl::ClaimWindowForGPUDevice(device, window);

    return device;
}

} // namespace triangles