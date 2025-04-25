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

SDL_GPUBuffer *Uploader::UploadBuffer(SDL_GPUBufferUsageFlags usage, void *contents, size_t size)
{
    Uint32 sizeUint32 = static_cast<Uint32>(size);

    SDL_GPUBufferCreateInfo vertexBufferCreateInfo{.usage = usage, .size = sizeUint32};
    SDL_GPUBuffer *vertexBuffer = sdl::CreateGPUBuffer(m_device, &vertexBufferCreateInfo);

    SDL_GPUTransferBufferCreateInfo vertexBufferTransferInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = sizeUint32};

    SDL_GPUTransferBuffer *transferBuffer =
        sdl::CreateGPUTransferBuffer(m_device, &vertexBufferTransferInfo);
    m_transferBuffers.push_back(transferBuffer);

    void *pVertices = sdl::MapGPUTransferBuffer(m_device, transferBuffer, false);
    sdl::memcpy(pVertices, contents, size);
    sdl::UnmapGPUTransferBuffer(m_device, transferBuffer);

    SDL_GPUTransferBufferLocation transferBufferLocation{.transfer_buffer = transferBuffer,
                                                         .offset = 0};

    SDL_GPUBufferRegion gpuBufferRegion{.buffer = vertexBuffer, .offset = 0, .size = sizeUint32};

    sdl::UploadToGPUBuffer(m_copyPass, &transferBufferLocation, &gpuBufferRegion, false);

    return vertexBuffer;
}

SDL_GPUTexture *Uploader::UploadTexture(SDL_Surface *surface)
{
    SDL_GPUTextureCreateInfo textureCreateInfo{.type = SDL_GPU_TEXTURETYPE_2D,
                                               .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
                                               .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
                                               .width = static_cast<Uint32>(surface->w),
                                               .height = static_cast<Uint32>(surface->h),
                                               .layer_count_or_depth = 1,
                                               .num_levels = 1};

    SDL_GPUTexture *texture = sdl::CreateGPUTexture(m_device, &textureCreateInfo);

    Uint32 textureSizeInBytes = surface->h * surface->pitch;

    SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = textureSizeInBytes};

    SDL_GPUTransferBuffer *transferBuffer =
        sdl::CreateGPUTransferBuffer(m_device, &transferBufferCreateInfo);

    void *pTextureData = sdl::MapGPUTransferBuffer(m_device, transferBuffer, false);

    sdl::memcpy(pTextureData, surface->pixels, textureSizeInBytes);

    SDL_GPUTextureTransferInfo transferInfo{.transfer_buffer = transferBuffer, .offset = 0};
    SDL_GPUTextureRegion textureRegion{.texture = texture,
                                       .w = static_cast<Uint32>(surface->w),
                                       .h = static_cast<Uint32>(surface->h),
                                       .d = 1};

    sdl::UploadToGPUTexture(m_copyPass, &transferInfo, &textureRegion, false);

    return texture;
}

void Uploader::Finish()
{
    sdl::EndGPUCopyPass(m_copyPass);
    sdl::SubmitGPUCommandBuffer(m_uploadCommandBuffer);
}

} // namespace triangles