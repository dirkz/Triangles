#pragma once

namespace triangles
{

struct Triangles
{
    Triangles();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

  private:
    SDL_Window *m_window;
};

} // namespace triangles
