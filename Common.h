#pragma once

#include "stdafx.h"

namespace triangles
{

SDL_Window *CreateWindow(const char *title);
SDL_GPUDevice *CreateDevice(SDL_Window *window);

}
