#pragma once

namespace triangles
{

struct Triangles
{
    Triangles();
    ~Triangles();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

    SDL_GPUShader *LoadShader(const std::string &filenameBase, SDL_GPUShaderStage stage,
                              Uint32 numUniformBuffers = 0, Uint32 numSamplers = 0,
                              Uint32 numStorageBuffers = 0, Uint32 numStorageTextures = 0);

  private:
    SDL_Window *m_window;
    SDL_GPUDevice *m_device;
};

} // namespace triangles
