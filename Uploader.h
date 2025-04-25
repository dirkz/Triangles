#pragma once

#include "stdafx.h"

namespace triangles
{

struct Uploader
{
    Uploader(SDL_GPUDevice *device);
    ~Uploader();

    SDL_GPUBuffer *UploadBuffer(SDL_GPUBufferUsageFlags usage, void *contents, size_t size);

    template <class T>
    SDL_GPUBuffer *UploadBuffer(SDL_GPUBufferUsageFlags usage, std::span<T> contents)
    {
        size_t size = sizeof(T) * contents.size();
        return UploadBuffer(usage, contents.data(), size);
    }

    SDL_GPUTexture *UploadTexture(SDL_Surface *surface);

    void Finish();

  private:
    SDL_GPUDevice *m_device;
    SDL_GPUCommandBuffer *m_uploadCommandBuffer;
    SDL_GPUCopyPass *m_copyPass;
    std::vector<SDL_GPUTransferBuffer *> m_transferBuffers;
};

} // namespace triangles
