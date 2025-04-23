#include "ShaderLoader.h"

namespace triangles
{

ShaderLoader::ShaderLoader(SDL_GPUDevice *device)
    : m_device{device}, m_basePath{std::filesystem::path{sdl::GetBasePath()} / "shaders"},
      m_supportedShaderFormat{sdl::GetGPUShaderFormats(m_device)}
{
}

SDL_GPUShaderFormat ShaderLoader::PreferredShaderFormat() const
{
    if (m_supportedShaderFormat & SDL_GPU_SHADERFORMAT_MSL)
    {
        return SDL_GPU_SHADERFORMAT_MSL;
    }
    if (m_supportedShaderFormat & SDL_GPU_SHADERFORMAT_DXIL)
    {
        return SDL_GPU_SHADERFORMAT_DXIL;
    }
    if (m_supportedShaderFormat & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        return SDL_GPU_SHADERFORMAT_SPIRV;
    }

    std::string errorMsg =
        std::format("unsupported shader format: {}", static_cast<int>(m_supportedShaderFormat));
    throw std::runtime_error{errorMsg};
}

std::string ShaderLoader::PreferredShaderFormatString() const
{
    switch (PreferredShaderFormat())
    {
    case SDL_GPU_SHADERFORMAT_DXIL:
        return "dxil";
    case SDL_GPU_SHADERFORMAT_SPIRV:
        return "spirv";
    case SDL_GPU_SHADERFORMAT_MSL:
        return "msl";
    default:
        std::string errorMsg =
            std::format("unsupported shader format: {}", static_cast<int>(m_supportedShaderFormat));
        throw std::runtime_error{errorMsg};
    }
}

std::string ShaderLoader::ShaderEntryPoint(SDL_GPUShaderStage stage) const
{
    switch (stage)
    {
    case SDL_GPU_SHADERSTAGE_FRAGMENT:
        return "PS";
    case SDL_GPU_SHADERSTAGE_VERTEX:
        return "VS";
    default:
        std::string errorMsg = std::format("unsupported shader stage: {}", static_cast<int>(stage));
        throw std::runtime_error{errorMsg};
    }
}

} // namespace triangles