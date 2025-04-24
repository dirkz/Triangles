#pragma once

#include "stdafx.h"

namespace triangles
{

constexpr glm::vec4 Red{1, 0, 0, 1};
constexpr glm::vec4 Green{0, 1, 0, 1};
constexpr glm::vec4 Blue{0, 0, 1, 1};
constexpr glm::vec4 Yellow{1.0, 0.9, 0., 1};

SDL_Window *CreateWindow(const char *title);

SDL_GPUDevice *CreateDevice(SDL_Window *window);

SDL_GPUGraphicsPipeline *CreateGraphicsPipeline(
    SDL_Window *window, SDL_GPUDevice *device, SDL_GPUShader *vertexShader,
    SDL_GPUShader *fragmentShader,
    const std::span<SDL_GPUVertexBufferDescription> &vertexBufferDescriptions,
    const std::span<SDL_GPUVertexAttribute> &attributes);

double Elapsed(Uint64 numberOfTicksPerCycle, Uint64 now = sdl::GetTicks());

} // namespace triangles
