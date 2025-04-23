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
    std::string ShaderStageString(SDL_GPUShaderStage stage) const;
    SDL_GPUShaderFormat PreferredShaderFormat() const;
    std::string PreferredShaderFormatString() const;
    std::string ShaderEntryPoint(SDL_GPUShaderStage stage) const;

    SDL_GPUShader *LoadShader(const std::string &filenameBase, SDL_GPUShaderStage stage,
                              Uint32 numUniformBuffers = 0, Uint32 numSamplers = 0,
                              Uint32 numStorageBuffers = 0, Uint32 numStorageTextures = 0) const;

    SDL_GPUGraphicsPipelineCreateInfo PipelineCreateInfo(SDL_GPUShader *vertexShader,
                                                         SDL_GPUShader *fragmentShader) const;
    void CreateGraphicsPipeline();

    void UploadBuffers();

    SDL_Window *m_window = nullptr;
    SDL_GPUDevice *m_device = nullptr;
    SDL_GPUShaderFormat m_supportedShaderFormats;
    std::string m_basePath;
    SDL_GPUGraphicsPipeline *m_pipeline = nullptr;
    SDL_GPUBuffer *m_vertexBuffer = nullptr;
};

} // namespace triangles
