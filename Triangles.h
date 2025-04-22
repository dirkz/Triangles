#pragma once

namespace triangles
{

struct Triangles
{
    Triangles();
    ~Triangles();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

  private:
    SDL_Window *m_window;
    SDL_GPUDevice *m_device;
};

} // namespace triangles
