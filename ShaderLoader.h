#pragma once

#include "stdafx.h"

namespace triangles
{

struct ShaderLoader
{
    ShaderLoader(SDL_GPUDevice *device);

    SDL_GPUShaderFormat PreferredShaderFormat() const;
    std::string PreferredShaderFormatString() const;
    std::string ShaderEntryPoint(SDL_GPUShaderStage stage) const;
    std::string ShaderStageString(SDL_GPUShaderStage stage) const;

    SDL_GPUShader *LoadShader(const std::string &filenameBase, SDL_GPUShaderStage stage,
                              Uint32 numUniformBuffers, Uint32 numSamplers,
                              Uint32 numStorageBuffers, Uint32 numStorageTextures) const;

  private:
    SDL_GPUDevice *m_device;
    std::filesystem::path m_basePath;
    SDL_GPUShaderFormat m_supportedShaderFormat;
};

} // namespace triangles
