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

  private:
    SDL_GPUDevice *m_device;
    std::filesystem::path m_basePath;
    SDL_GPUShaderFormat m_supportedShaderFormat;
};

} // namespace triangles
