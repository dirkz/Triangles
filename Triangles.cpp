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
    sdl::SetWindowPosition(m_window, mode->w / 4, 100);

#ifdef _DEBUG
    bool debugMode = true;
#else
    bool debugMode = false;
#endif

    m_device = sdl::CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, nullptr);
    sdl::ClaimWindowForGPUDevice(m_device, m_window);
}

Triangles::~Triangles()
{
    sdl::ReleaseWindowFromGPUDevice(m_device, m_window);
    sdl::DestroyGPUDevice(m_device);
}

void Triangles::AppIterate()
{
}

bool Triangles::AppEvent(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_KEY_DOWN:
        return true;
    default:
        return false;
    }
}

} // namespace triangles