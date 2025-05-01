#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "BasicTriangle.h"
#include "BasicUniform.h"
#include "Cube.h"
#include "IcosahedronSample.h"
#include "IndexedQuad.h"
#include "TexturedQuad.h"

using namespace triangles;

using App = IcosahedronSample;

static SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    try
    {
        App *app = new App{};
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
        App *app = static_cast<App *>(appstate);
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
        App *app = static_cast<App *>(appstate);
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
    App *app = static_cast<App *>(appstate);
    delete app;
}