#pragma once

#include "stdafx.h"

namespace triangles
{

struct IndexedQuad
{
    IndexedQuad();
    ~IndexedQuad();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

  private:
    void CreateGraphicsPipeline();

    void UploadBuffers();

    SDL_Window *m_window = nullptr;
    SDL_GPUDevice *m_device = nullptr;
    SDL_GPUGraphicsPipeline *m_pipeline = nullptr;
    SDL_GPUBuffer *m_vertexBuffer = nullptr;
    SDL_GPUBuffer *m_indexBuffer = nullptr;
};

} // namespace triangles
