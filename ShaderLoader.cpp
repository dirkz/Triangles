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

std::string ShaderLoader::ShaderStageString(SDL_GPUShaderStage stage) const
{
    switch (stage)
    {
    case SDL_GPU_SHADERSTAGE_FRAGMENT:
        return "fragment";
    case SDL_GPU_SHADERSTAGE_VERTEX:
        return "vertex";
    default:
        std::string errorMsg = std::format("unsupported shader stage: {}", static_cast<int>(stage));
        throw std::runtime_error{errorMsg};
    }
}

SDL_GPUShader *ShaderLoader::LoadShader(const std::string &filenameBase, SDL_GPUShaderStage stage,
                                        Uint32 numUniformBuffers, Uint32 numSamplers,
                                        Uint32 numStorageBuffers, Uint32 numStorageTextures) const
{
    std::string stageString = ShaderStageString(stage);
    std::string shaderFormatString = PreferredShaderFormatString();
    std::string filename = std::format("{}.{}.{}", filenameBase, stageString, shaderFormatString);

    std::filesystem::path filepath = m_basePath / filename;

    std::string filepathString = filepath.string();
    size_t numBytes = 0;
    sdl::Void p{sdl::LoadFile(filepathString.c_str(), &numBytes)};
    Uint8 *pContents = static_cast<Uint8 *>(p.get());

    SDL_GPUShaderFormat format = PreferredShaderFormat();
    std::string entryPoint = ShaderEntryPoint(stage);
    SDL_GPUShaderCreateInfo createInfo{.code_size = numBytes,
                                       .code = pContents,
                                       .entrypoint = entryPoint.c_str(),
                                       .format = format,
                                       .stage = stage,
                                       .num_samplers = numSamplers,
                                       .num_storage_textures = numStorageTextures,
                                       .num_storage_buffers = numStorageBuffers,
                                       .num_uniform_buffers = numUniformBuffers};

    SDL_GPUShader *shader = sdl::CreateGPUShader(m_device, &createInfo);

    return shader;
}

} // namespace triangles