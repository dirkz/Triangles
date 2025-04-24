#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "IndexedQuad.h"

using namespace triangles;

static SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    try
    {
        IndexedQuad *app = new IndexedQuad{};
        *appstate = app;
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
        IndexedQuad *app = static_cast<IndexedQuad *>(appstate);
        app->AppIterate();
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
        IndexedQuad *app = static_cast<IndexedQuad *>(appstate);
        bool shouldQuit = app->AppEvent(event);
        if (shouldQuit)
        {
            return SDL_APP_SUCCESS;
        }
    }
    catch (...)
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    IndexedQuad *app = static_cast<IndexedQuad *>(appstate);
    delete app;
}