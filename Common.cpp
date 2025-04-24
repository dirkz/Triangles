#include "Common.h"

namespace triangles
{

SDL_Window *CreateWindow(const char *title)
{
    sdl::Init(SDL_INIT_VIDEO);

    SDL_DisplayID display = sdl::GetPrimaryDisplay();
    const SDL_DisplayMode *mode = sdl::GetCurrentDisplayMode(display);
    int width = mode->w / 2;
    int height = mode->h / 2;

    SDL_Window *window = sdl::CreateWindow(title, width, height, 0);
    sdl::SetWindowPosition(window, mode->w / 4, 100);

    return window;
}

SDL_GPUDevice *CreateDevice(SDL_Window *window)
{
#ifdef _DEBUG
    bool debugMode = true;
#else
    bool debugMode = false;
#endif

    SDL_GPUDevice *device = sdl::CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL,
        debugMode, nullptr);
    sdl::ClaimWindowForGPUDevice(device, window);

    return device;
}

SDL_GPUGraphicsPipeline *CreateGraphicsPipeline(
    SDL_Window *window, SDL_GPUDevice *device, SDL_GPUShader *vertexShader,
    SDL_GPUShader *fragmentShader,
    const std::span<SDL_GPUVertexBufferDescription> &vertexBufferDescriptions,
    const std::span<SDL_GPUVertexAttribute> &attributes)
{
    SDL_GPUColorTargetDescription colorTargetDescription{
        .format = sdl::GetGPUSwapchainTextureFormat(device, window)};

    SDL_GPUVertexInputState vertexInputState{
        .vertex_buffer_descriptions = vertexBufferDescriptions.data(),
        .num_vertex_buffers = static_cast<Uint32>(vertexBufferDescriptions.size()),
        .vertex_attributes = attributes.data(),
        .num_vertex_attributes = static_cast<Uint32>(attributes.size())};

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo{
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .vertex_input_state = vertexInputState,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {.color_target_descriptions = &colorTargetDescription,
                        .num_color_targets = 1}};

    SDL_GPUGraphicsPipeline *pipeline = sdl::CreateGPUGraphicsPipeline(device, &pipelineCreateInfo);

    return pipeline;
}

} // namespace triangles