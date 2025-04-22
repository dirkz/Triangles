#pragma once

namespace triangles
{

struct Triangles
{
    Triangles();
    ~Triangles();

    void AppIterate();
    bool AppEvent(SDL_Event *event);

  private:
    std::string ShaderStageString(SDL_GPUShaderStage stage) const;
    std::string ShaderFormatString() const;
    std::string ShaderEntryPoint(SDL_GPUShaderStage stage) const;

    SDL_GPUShader *LoadShader(const std::string &filenameBase, SDL_GPUShaderStage stage,
                              Uint32 numUniformBuffers = 0, Uint32 numSamplers = 0,
                              Uint32 numStorageBuffers = 0, Uint32 numStorageTextures = 0) const;

    SDL_Window *m_window;
    SDL_GPUDevice *m_device;
    SDL_GPUShaderFormat m_supportedShaderFormats;
    std::string m_basePath;
};

} // namespace triangles
