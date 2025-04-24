#pragma once

#include "stdafx.h"

namespace triangles
{

struct BasicUniform
{
    BasicUniform();
    ~BasicUniform();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

  private:
    void CreateGraphicsPipeline();

    void UploadBuffers();

    SDL_Window *m_window = nullptr;
    SDL_GPUDevice *m_device = nullptr;
    SDL_GPUShaderFormat m_supportedShaderFormats;
    SDL_GPUGraphicsPipeline *m_pipeline = nullptr;
    SDL_GPUBuffer *m_vertexBuffer = nullptr;
};

} // namespace triangles
