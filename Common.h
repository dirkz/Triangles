#pragma once

#include "stdafx.h"

namespace triangles
{

SDL_Window *CreateWindow(const char *title);

SDL_GPUDevice *CreateDevice(SDL_Window *window);

SDL_GPUGraphicsPipeline *CreateGraphicsPipeline(
    SDL_Window *window, SDL_GPUDevice *device, SDL_GPUShader *vertexShader,
    SDL_GPUShader *fragmentShader,
    const std::span<SDL_GPUVertexBufferDescription> &vertexBufferDescriptions,
    const std::span<SDL_GPUVertexAttribute> &attributes);

double Elapsed(Uint64 numberOfTicksPerCycle, Uint32 now = sdl::GetTicks());

} // namespace triangles
