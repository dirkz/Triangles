#include "BasicUniform.h"

#include "PositionColorVertex.h"
#include "ShaderLoader.h"
#include "Uploader.h"

namespace triangles
{

BasicUniform::BasicUniform()
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

BasicUniform::~BasicUniform()
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

void BasicUniform::AppIterate()
{
    SDL_GPUCommandBuffer *commandBuffer = sdl::AcquireGPUCommandBuffer(m_device);

    SDL_GPUTexture *swapchainTexture;
    Uint32 width, height;
    sdl::WaitAndAcquireGPUSwapchainTexture(commandBuffer, m_window, &swapchainTexture, &width,
                                           &height);

    if (swapchainTexture)
    {
        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor{0.0f, 0.0f, 0.0f, 1.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass *renderPass =
            sdl::BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);

        sdl::BindGPUGraphicsPipeline(renderPass, m_pipeline);

        SDL_GPUBufferBinding bufferBinding{.buffer = m_vertexBuffer, .offset = 0};
        sdl::BindGPUVertexBuffers(renderPass, 0, &bufferBinding, 1);

        sdl::DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

        sdl::EndGPURenderPass(renderPass);
    }

    sdl::SubmitGPUCommandBuffer(commandBuffer);
}

bool BasicUniform::AppEvent(SDL_Event *event)
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

void BasicUniform::CreateGraphicsPipeline()
{
    ShaderLoader shaderLoader{m_device};

    const char *basicTriangle = "basic_triangle_uniform.hlsl";
    sdl::DeviceOwned vertexShader{
        m_device, shaderLoader.Load(basicTriangle, SDL_GPU_SHADERSTAGE_VERTEX, 0, 0, 0, 0)};
    sdl::DeviceOwned fragmentShader{
        m_device, shaderLoader.Load(basicTriangle, SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 0, 0, 0)};

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
                                             .offset = offsetof(PositionColorVertex, m_position)};

    SDL_GPUVertexAttribute attributeColor{.location = 1,
                                          .buffer_slot = 0,
                                          .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                                          .offset = offsetof(PositionColorVertex, m_color)};

    std::array<SDL_GPUVertexAttribute, 2> attributes{attributePosition, attributeColor};

    SDL_GPUVertexInputState vertexInputState{.vertex_buffer_descriptions = &vertexBufferDescription,
                                             .num_vertex_buffers = 1,
                                             .vertex_attributes = attributes.data(),
                                             .num_vertex_attributes =
                                                 static_cast<Uint32>(attributes.size())};

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo{
        .vertex_shader = vertexShader.Get(),
        .fragment_shader = fragmentShader.Get(),
        .vertex_input_state = vertexInputState,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {.color_target_descriptions = &colorTargetDescription,
                        .num_color_targets = 1}};

    m_pipeline = sdl::CreateGPUGraphicsPipeline(m_device, &pipelineCreateInfo);
}

void BasicUniform::UploadBuffers()
{
    glm::vec4 red{1, 0, 0, 1};
    glm::vec4 green{0, 1, 0, 1};
    glm::vec4 blue{0, 0, 1, 1};

    std::vector<PositionColorVertex> vertices{PositionColorVertex{-0.5, -0.5, 0, red},
                                              PositionColorVertex{0.5, -0.5, 0, green},
                                              PositionColorVertex{0, 0.5, 0, blue}};

    Uploader uploader{m_device};
    m_vertexBuffer = uploader.UploadBuffer(SDL_GPU_BUFFERUSAGE_VERTEX, std::span{vertices});

    uploader.Finish();
}

} // namespace triangles