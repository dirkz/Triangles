#include "Triangles.h"

#include "PositionColorVertex.h"

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

    CreateGraphicsPipeline();
    UploadBuffers();
}

Triangles::~Triangles()
{
    if (m_device)
    {
        if (m_pipeline)
        {
            sdl::ReleaseGPUGraphicsPipeline(m_device, m_pipeline);
        }

        if (m_vertexBuffer)
        {
            sdl::ReleaseGPUBuffer(m_device, m_vertexBuffer);
        }

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

SDL_GPUGraphicsPipelineCreateInfo Triangles::PipelineCreateInfo(SDL_GPUShader *vertexShader,
                                                                SDL_GPUShader *fragmentShader) const
{
    SDL_GPUColorTargetDescription colorTargetDescription{
        .format = sdl::GetGPUSwapchainTextureFormat(m_device, m_window)};

    SDL_GPUVertexBufferDescription vertexBufferDescription{
        .slot = 0,
        .pitch = sizeof(PositionColorVertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };

    SDL_GPUVertexAttribute attributePosition{.location = 0,
                                             .buffer_slot = 0,
                                             .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                                             .offset = offsetof(PositionColorVertex, Position)};

    SDL_GPUVertexAttribute attributeColor{.location = 1,
                                          .buffer_slot = 0,
                                          .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                                          .offset = offsetof(PositionColorVertex, Color)};

    std::array<SDL_GPUVertexAttribute, 2> attributes{attributePosition, attributeColor};

    SDL_GPUVertexInputState vertexInputState{.vertex_buffer_descriptions = &vertexBufferDescription,
                                             .num_vertex_buffers = 1,
                                             .vertex_attributes = attributes.data(),
                                             .num_vertex_attributes =
                                                 static_cast<Uint32>(attributes.size())};

    SDL_GPUGraphicsPipelineCreateInfo createInfo{
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .vertex_input_state = vertexInputState,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {.color_target_descriptions = &colorTargetDescription,
                        .num_color_targets = 1}};

    return createInfo;
}

void Triangles::CreateGraphicsPipeline()
{
    const char *basicTriangle = "basic_triangle.hlsl";
    sdl::DeviceOwned vertexShader{m_device, LoadShader(basicTriangle, SDL_GPU_SHADERSTAGE_VERTEX)};
    sdl::DeviceOwned fragmentShader{m_device,
                                    LoadShader(basicTriangle, SDL_GPU_SHADERSTAGE_FRAGMENT)};

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo =
        PipelineCreateInfo(vertexShader.Get(), fragmentShader.Get());

    m_pipeline = sdl::CreateGPUGraphicsPipeline(m_device, &pipelineCreateInfo);
}

void Triangles::UploadBuffers()
{
    glm::vec4 red{1, 0, 0, 1};
    glm::vec4 green{0, 1, 0, 1};
    glm::vec4 blue{0, 0, 1, 1};

    std::vector<PositionColorVertex> vertices{PositionColorVertex{glm::vec3{-0.5, -0.5, 0}, red},
                                              PositionColorVertex{glm::vec3{0.5, -0.5, 0}, green},
                                              PositionColorVertex{glm::vec3{0, 0.5, 0}, blue}};

    Uint32 sizeVertices = static_cast<Uint32>(vertices.end() - vertices.begin());

    SDL_GPUBufferCreateInfo vertexBufferCreateInfo{.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
                                                   .size = sizeVertices};
    m_vertexBuffer = sdl::CreateGPUBuffer(m_device, &vertexBufferCreateInfo);

    SDL_GPUTransferBufferCreateInfo vertexBufferTransferInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = sizeVertices};

    sdl::DeviceOwned transferBuffer{
        m_device, sdl::CreateGPUTransferBuffer(m_device, &vertexBufferTransferInfo)};

    PositionColorVertex *pVertices = static_cast<PositionColorVertex *>(
        sdl::MapGPUTransferBuffer(m_device, transferBuffer.Get(), false));
    sdl::memcpy(pVertices, vertices.data(), sizeVertices);
    sdl::UnmapGPUTransferBuffer(m_device, transferBuffer.Get());
}

} // namespace triangles