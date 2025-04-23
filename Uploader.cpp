#include "Uploader.h"

namespace triangles
{

Uploader::Uploader(SDL_GPUDevice *device)
    : m_device{device}, m_uploadCommandBuffer{sdl::AcquireGPUCommandBuffer(device)},
      m_copyPass{sdl::BeginGPUCopyPass(m_uploadCommandBuffer)}
{
}

Uploader::~Uploader()
{
    for (SDL_GPUTransferBuffer *transferBuffer : m_transferBuffers)
    {
        SDL_ReleaseGPUTransferBuffer(m_device, transferBuffer);
    }
}

SDL_GPUBuffer *Uploader::UploadBuffer(SDL_GPUBufferUsageFlags usage, void *contents, Uint32 size)
{
    SDL_GPUBufferCreateInfo vertexBufferCreateInfo{.usage = usage, .size = size};
    SDL_GPUBuffer *vertexBuffer = sdl::CreateGPUBuffer(m_device, &vertexBufferCreateInfo);

    SDL_GPUTransferBufferCreateInfo vertexBufferTransferInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = size};

    SDL_GPUTransferBuffer *transferBuffer =
        sdl::CreateGPUTransferBuffer(m_device, &vertexBufferTransferInfo);
    m_transferBuffers.push_back(transferBuffer);

    void *pVertices = sdl::MapGPUTransferBuffer(m_device, transferBuffer, false);
    sdl::memcpy(pVertices, contents, size);
    sdl::UnmapGPUTransferBuffer(m_device, transferBuffer);

    SDL_GPUTransferBufferLocation transferBufferLocation{.transfer_buffer = transferBuffer,
                                                         .offset = 0};

    SDL_GPUBufferRegion gpuBufferRegion{.buffer = vertexBuffer, .offset = 0, .size = size};

    sdl::UploadToGPUBuffer(m_copyPass, &transferBufferLocation, &gpuBufferRegion, false);

    return vertexBuffer;
}

void Uploader::Finish()
{
    sdl::EndGPUCopyPass(m_copyPass);
    sdl::SubmitGPUCommandBuffer(m_uploadCommandBuffer);
}

} // namespace triangles