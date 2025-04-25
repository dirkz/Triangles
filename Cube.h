#pragma once

#include "stdafx.h"

#include "Surface.h"

namespace triangles
{

struct Cube
{
    Cube();
    ~Cube();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

  private:
    void CreateGraphicsPipeline();
    void CreateSurfaceTexture();

    void UploadBuffers();

    SDL_Window *m_window = nullptr;
    SDL_GPUDevice *m_device = nullptr;
    SDL_GPUGraphicsPipeline *m_pipeline = nullptr;
    SDL_GPUBuffer *m_vertexBuffer = nullptr;
    SDL_GPUBuffer *m_indexBuffer = nullptr;
    Uint32 m_numIndices = 0;
    Surface m_surface;
    SDL_GPUTexture *m_texture = nullptr;
    SDL_GPUSampler *m_sampler = nullptr;
};

} // namespace triangles
