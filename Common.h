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

double Elapsed(Uint64 numberOfTicksPerCycle, Uint64 now = sdl::GetTicks());

inline constexpr Uint32 RGB(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
{
    return r + (g << 8) + (b << 16) + (a << 24);
}

} // namespace triangles
