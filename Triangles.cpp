#include "Triangles.h"

namespace triangles
{

Triangles::Triangles()
{
    sdl::Init(SDL_INIT_VIDEO);

    SDL_DisplayID display = sdl::GetPrimaryDisplay();
    const SDL_DisplayMode *mode = sdl::GetCurrentDisplayMode(display);
    int width = mode->w / 2;
    int height = mode->h / 2;

    m_window = sdl::CreateWindow("Triangles", width, height, 0);
    sdl::SetWindowPosition(m_window, mode->w / 4, 100);

#ifdef _DEBUG
    bool debugMode = true;
#else
    bool debugMode = false;
#endif

    m_device = sdl::CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV |
                                        SDL_GPU_SHADERFORMAT_MSL,
                                    debugMode, nullptr);
    sdl::ClaimWindowForGPUDevice(m_device, m_window);

    m_supportedShaderFormats = sdl::GetGPUShaderFormats(m_device);
    m_basePath = sdl::GetBasePath();

    const char *basicTriangle = "basic_triangle.hlsl";
    SDL_GPUShader *vertexShader = LoadShader(basicTriangle, SDL_GPU_SHADERSTAGE_VERTEX);
    sdl::DeviceOwned vertexDestructor{m_device, vertexShader};
    SDL_GPUShader *fragmentShader = LoadShader(basicTriangle, SDL_GPU_SHADERSTAGE_FRAGMENT);
    sdl::DeviceOwned fragmentDestructor{m_device, fragmentShader};
}

Triangles::~Triangles()
{
    if (m_device)
    {
        if (m_window)
        {
            sdl::ReleaseWindowFromGPUDevice(m_device, m_window);
        }

        sdl::DestroyGPUDevice(m_device);
    }

    if (m_window)
    {
        sdl::DestroyWindow(m_window);
    }
}

void Triangles::AppIterate()
{
}

bool Triangles::AppEvent(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_KEY_DOWN:
        return true;
    default:
        return false;
    }
}

std::string Triangles::ShaderStageString(SDL_GPUShaderStage stage) const
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

SDL_GPUShaderFormat Triangles::PreferredShaderFormat() const
{
    if (m_supportedShaderFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        return SDL_GPU_SHADERFORMAT_MSL;
    }
    if (m_supportedShaderFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        return SDL_GPU_SHADERFORMAT_DXIL;
    }
    if (m_supportedShaderFormats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        return SDL_GPU_SHADERFORMAT_SPIRV;
    }

    std::string errorMsg =
        std::format("unsupported shader format: {}", static_cast<int>(m_supportedShaderFormats));
    throw std::runtime_error{errorMsg};
}

std::string Triangles::PreferredShaderFormatString() const
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
        std::string errorMsg = std::format("unsupported shader format: {}",
                                           static_cast<int>(m_supportedShaderFormats));
        throw std::runtime_error{errorMsg};
    }
}

std::string Triangles::ShaderEntryPoint(SDL_GPUShaderStage stage) const
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

SDL_GPUShader *Triangles::LoadShader(const std::string &filenameBase, SDL_GPUShaderStage stage,
                                     Uint32 numUniformBuffers, Uint32 numSamplers,
                                     Uint32 numStorageBuffers, Uint32 numStorageTextures) const
{
    std::string stageString = ShaderStageString(stage);
    std::string shaderFormatString = PreferredShaderFormatString();
    std::string filename = std::format("{}.{}.{}", filenameBase, stageString, shaderFormatString);

    std::filesystem::path basePath{m_basePath};
    std::filesystem::path filepath = basePath / "shaders" / filename;

    std::string filepathString = filepath.string();
    size_t numBytes = 0;
    void *contents = sdl::LoadFile(filepathString.c_str(), &numBytes);
    sdl::Void p{contents};
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