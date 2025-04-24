#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "TexturedQuad.h"

using namespace triangles;

static SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    try
    {
        TexturedQuad *app = new TexturedQuad{};
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
        TexturedQuad *app = static_cast<TexturedQuad *>(appstate);
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
        TexturedQuad *app = static_cast<TexturedQuad *>(appstate);
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
    TexturedQuad *app = static_cast<TexturedQuad *>(appstate);
    delete app;
}