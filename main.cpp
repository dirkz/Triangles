#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "Triangles.h"

using namespace triangles;

static SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    try
    {
        Triangles *triangles = new Triangles{};
        *appstate = triangles;
    }
    catch (...)
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    try
    {
        Triangles *triangles = static_cast<Triangles *>(appstate);
        triangles->AppIterate();
    }
    catch (...)
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    try
    {
        Triangles *triangles = static_cast<Triangles *>(appstate);
        triangles->AppEvent(event);
    }
    catch (...)
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    Triangles *triangles = static_cast<Triangles *>(appstate);
    delete triangles;
}