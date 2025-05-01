#include "IcosahedronSample.h"

#include "Common.h"
#include "Icosahedron.h"
#include "Noise.h"
#include "PositionColorVertex.h"
#include "ShaderLoader.h"
#include "Triangle.h"
#include "Uploader.h"

namespace triangles
{

using namespace DirectX;

constexpr int TextureSize = 512;

IcosahedronSample::IcosahedronSample()
    : m_window{CreateWindow("Icosahedron")}, m_device{CreateDevice(m_window)},
      m_surface{TextureSize, TextureSize}
{
    CreateGraphicsPipeline();
    CreateSurfaceTexture();
    CreateGeometry();
    UploadBuffers();

    SDL_GPUSamplerCreateInfo samplerCreateInfo{
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .mag_filter = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE};

    m_sampler = sdl::CreateGPUSampler(m_device, &samplerCreateInfo);
}

IcosahedronSample::~IcosahedronSample()
{
    if (m_device)
    {
        if (m_texture)
        {
            sdl::ReleaseGPUTexture(m_device, m_texture);
        }

        if (m_sampler)
        {
            sdl::ReleaseGPUSampler(m_device, m_sampler);
        }

        if (m_pipeline)
        {
            sdl::ReleaseGPUGraphicsPipeline(m_device, m_pipeline);
        }

        if (m_vertexBuffer)
        {
            sdl::ReleaseGPUBuffer(m_device, m_vertexBuffer);
        }

        if (m_indexBuffer)
        {
            sdl::ReleaseGPUBuffer(m_device, m_indexBuffer);
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

void IcosahedronSample::AppIterate()
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

        XMMATRIX translation = XMMatrixTranslation(0.f, 0.f, 2.f);
        XMMATRIX rotationY = XMMatrixRotationY(m_rotationHorizontal);
        XMMATRIX rotationX = XMMatrixRotationX(m_rotationVertical);
        XMMATRIX rotation = XMMatrixMultiply(rotationX, rotationY);
        XMMATRIX model = XMMatrixMultiply(rotation, translation);

        float aspect = static_cast<float>(width) / height;

        XMVECTOR eyePosition = XMVectorSet(0.f, 0.f, -1.f, 1.f);
        XMVECTOR lookAt = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
        XMMATRIX view = XMMatrixLookAtLH(eyePosition, lookAt, upDirection);

        XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspect, 0.1f, 10.f);

        XMMATRIX m = XMMatrixMultiply(model, view);
        m = XMMatrixMultiply(m, projection);
        XMFLOAT4X4 transformation;

        // This stores row-major, so no extra transpose is needed.
        XMStoreFloat4x4(&transformation, m);

        sdl::PushGPUVertexUniformData(commandBuffer, 0, &transformation, sizeof(transformation));

        SDL_GPUBufferBinding vertexBufferBinding{.buffer = m_vertexBuffer, .offset = 0};
        sdl::BindGPUVertexBuffers(renderPass, 0, &vertexBufferBinding, 1);

        SDL_GPUBufferBinding indexBufferBinding{.buffer = m_indexBuffer, .offset = 0};
        sdl::BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

        SDL_GPUTextureSamplerBinding textureBinding{.texture = m_texture, .sampler = m_sampler};
        sdl::BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);

        sdl::DrawGPUIndexedPrimitives(renderPass, m_numIndices, 1, 0, 0, 0);

        sdl::EndGPURenderPass(renderPass);
    }

    sdl::SubmitGPUCommandBuffer(commandBuffer);
}

bool IcosahedronSample::AppEvent(SDL_Event *event)
{
    constexpr float RotationLeftRight = XM_PI / 10.f;
    constexpr float RotationUpDown = XM_PI / 10.f;

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_KEY_DOWN:
        switch (event->key.key)
        {
        case SDLK_LEFT:
            m_rotationHorizontal -= RotationLeftRight;
            return false;
        case SDLK_RIGHT:
            m_rotationHorizontal += RotationLeftRight;
            return false;
        case SDLK_UP:
            m_rotationVertical -= RotationUpDown;
            return false;
        case SDLK_DOWN:
            m_rotationVertical += RotationUpDown;
            return false;
        default:
            return true;
        }
    default:
        return false;
    }
}

void IcosahedronSample::CreateGraphicsPipeline()
{
    ShaderLoader shaderLoader{m_device};

    const char *basicTriangle = "position_color_texture_projection.hlsl";
    sdl::DeviceOwned vertexShader{
        m_device, shaderLoader.Load(basicTriangle, SDL_GPU_SHADERSTAGE_VERTEX, 1, 0, 0, 0)};
    sdl::DeviceOwned fragmentShader{
        m_device, shaderLoader.Load(basicTriangle, SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 1, 0, 0)};

    SDL_GPUVertexBufferDescription vertexBufferDescription{
        .slot = 0,
        .pitch = sizeof(PositionColorTextureVertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };

    std::vector<SDL_GPUVertexBufferDescription> vertexBufferDescriptions{vertexBufferDescription};

    SDL_GPUVertexAttribute attributePosition{.location = 0,
                                             .buffer_slot = 0,
                                             .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                                             .offset = offsetof(PositionColorTextureVertex, X)};

    SDL_GPUVertexAttribute attributeColor{.location = 1,
                                          .buffer_slot = 0,
                                          .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                                          .offset = offsetof(PositionColorTextureVertex, R)};

    SDL_GPUVertexAttribute attributeTexture{.location = 2,
                                            .buffer_slot = 0,
                                            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                                            .offset = offsetof(PositionColorTextureVertex, U)};

    std::vector<SDL_GPUVertexAttribute> attributes{attributePosition, attributeColor,
                                                   attributeTexture};

    m_pipeline = triangles::CreateGraphicsPipeline(
        m_window, m_device, vertexShader.Get(), fragmentShader.Get(),
        std::span{vertexBufferDescriptions}, std::span{attributes});
}

void IcosahedronSample::CreateSurfaceTexture()
{
    Noise noise{};

    for (int x = 0; x < m_surface.Width(); ++x)
    {
        for (int y = 0; y < m_surface.Height(); ++y)
        {
            double u = static_cast<double>(x) / 10.f;
            double v = static_cast<double>(y) / 10.f;
            double n = noise(u, v, 0);
            Uint8 c = static_cast<Uint8>(std::clamp(static_cast<int>(255.f * n), 0, 255));
            Uint32 color = RGB(c, c, c);
            m_surface.SetPixel(x, y, color);
        }
    }
}

void IcosahedronSample::CreateGeometry()
{
    Icosahedron icosahedron{};
    m_vertices = icosahedron.Vertices();
}

void IcosahedronSample::UploadBuffers()
{
    std::vector<PositionColorTextureVertex> vertices = m_vertices.Vertices();
    auto indices = m_vertices.Indices();

    Uploader uploader{m_device};
    m_vertexBuffer = uploader.UploadBuffer(SDL_GPU_BUFFERUSAGE_VERTEX, std::span{vertices});
    m_indexBuffer = uploader.UploadBuffer(SDL_GPU_BUFFERUSAGE_INDEX, std::span{indices});
    m_texture = uploader.UploadTexture(m_surface.GetSurface());

    uploader.Finish();

    m_numIndices = static_cast<Uint32>(indices.size());
}

} // namespace triangles