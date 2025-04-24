#include "TexturedQuad.h"

#include "Common.h"
#include "PositionColorVertex.h"
#include "ShaderLoader.h"
#include "Uploader.h"

namespace triangles
{

TexturedQuad::TexturedQuad() : m_surface{512, 512}
{
    m_window = CreateWindow("TexturedQuad");
    m_device = CreateDevice(m_window);
    CreateGraphicsPipeline();
    CreateSurfaceTexture();
    UploadBuffers();
}

TexturedQuad::~TexturedQuad()
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

void TexturedQuad::AppIterate()
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

        Uint64 now = sdl::GetTicks();  // ms
        constexpr Uint64 cycle = 6000; // ms
        double factor = Elapsed(cycle, now);

        double range = std::numbers::pi_v<double> * 2.0;
        float angle = static_cast<float>(factor * range);

        float s = std::sin(angle);
        float c = std::cos(angle);

        // Official counterclockwise rotation in a right-handed coordinate system:
        // https://en.wikipedia.org/wiki/Rotation_matrix#Basic_3D_rotations
        glm::mat4x4 rotation{
            c, -s, 0, 0, // row 1
            s, c,  0, 0, // row 2
            0, 0,  1, 0, // row 3
            0, 0,  0, 1  // row 4
        };

        float aspect = static_cast<float>(width) / height;

        glm::mat4x4 scale{1};
        scale = glm::scale(scale, glm::vec3{1.f / aspect, 1.f, 1.f});

        // But, GLM adheres to the GLSLang spec, so it expects matrices
        // to be constructed in column-major order (e.g. see here, page 84):
        // https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.20.pdf
        // So we have to transpose.
        glm::mat4x4 transformation = glm::transpose(rotation * scale);

        sdl::PushGPUVertexUniformData(commandBuffer, 0, &transformation, sizeof(transformation));

        SDL_GPUBufferBinding vertexBufferBinding{.buffer = m_vertexBuffer, .offset = 0};
        sdl::BindGPUVertexBuffers(renderPass, 0, &vertexBufferBinding, 1);

        SDL_GPUBufferBinding indexBufferBinding{.buffer = m_indexBuffer, .offset = 0};
        sdl::BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

        sdl::DrawGPUIndexedPrimitives(renderPass, m_numIndices, 1, 0, 0, 0);

        sdl::EndGPURenderPass(renderPass);
    }

    sdl::SubmitGPUCommandBuffer(commandBuffer);
}

bool TexturedQuad::AppEvent(SDL_Event *event)
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

void TexturedQuad::CreateGraphicsPipeline()
{
    ShaderLoader shaderLoader{m_device};

    const char *basicTriangle = "basic_triangle_uniform.hlsl";
    sdl::DeviceOwned vertexShader{
        m_device, shaderLoader.Load(basicTriangle, SDL_GPU_SHADERSTAGE_VERTEX, 1, 0, 0, 0)};
    sdl::DeviceOwned fragmentShader{
        m_device, shaderLoader.Load(basicTriangle, SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 0, 0, 0)};

    SDL_GPUVertexBufferDescription vertexBufferDescription{
        .slot = 0,
        .pitch = sizeof(PositionColorVertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };

    std::vector<SDL_GPUVertexBufferDescription> vertexBufferDescriptions{vertexBufferDescription};

    SDL_GPUVertexAttribute attributePosition{.location = 0,
                                             .buffer_slot = 0,
                                             .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                                             .offset = offsetof(PositionColorVertex, m_position)};

    SDL_GPUVertexAttribute attributeColor{.location = 1,
                                          .buffer_slot = 0,
                                          .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                                          .offset = offsetof(PositionColorVertex, m_color)};

    std::vector<SDL_GPUVertexAttribute> attributes{attributePosition, attributeColor};

    m_pipeline = triangles::CreateGraphicsPipeline(
        m_window, m_device, vertexShader.Get(), fragmentShader.Get(),
        std::span{vertexBufferDescriptions}, std::span{attributes});
}

void TexturedQuad::CreateSurfaceTexture()
{
    for (int x = 0; x < m_surface.Width(); ++x)
    {
        for (int y = 0; y < m_surface.Height(); ++y)
        {
        }
    }
}

void TexturedQuad::UploadBuffers()
{
    std::vector<PositionColorVertex> vertices{
        PositionColorVertex{-0.5, -0.5, 0, Red}, PositionColorVertex{0.5, -0.5, 0, Green},
        PositionColorVertex{0.5, 0.5, 0, Blue}, PositionColorVertex{-0.5, 0.5, 0, Yellow}};

    std::vector<Uint16> indices{0, 1, 2, 0, 2, 3};

    Uploader uploader{m_device};
    m_vertexBuffer = uploader.UploadBuffer(SDL_GPU_BUFFERUSAGE_VERTEX, std::span{vertices});
    m_indexBuffer = uploader.UploadBuffer(SDL_GPU_BUFFERUSAGE_INDEX, std::span{indices});

    uploader.Finish();

    m_numIndices = static_cast<Uint32>(indices.size());
}

} // namespace triangles