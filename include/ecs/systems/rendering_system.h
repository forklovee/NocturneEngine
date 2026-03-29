#pragma once
#include "system.h"

#include <memory>
#include <bitset>
#include "components.h"
#include "entity.h"

#include <window.h>
#include "gpu_mesh.h"
#include "gpu_texture.h"
#include "shader.h"
#include "meshdata.h"
#include "texture.h"
#include "handle.h"
#include <camera_component.h>
#include <world_environment.h>
#include <events.h>

namespace NocEngine {

class RenderingSystem: public ISystem
{
public:
    RenderingSystem(EventBus& eventBus);
    ~RenderingSystem() = default;

    virtual void Update() override;

    const std::bitset<64> GetRenderableEnityBitmask() const;
    const Entity GetActiveCameraEntity() const;
    CCamera& GetActiveCameraComponent() const;

private:
    const std::bitset<64> getRequiredBitmask() const;

    void renderEntity(Entity entity);
    GPU_Mesh* getGPUMesh(const Handle<MeshData>& meshdata_handle);
    GPU_Mesh* load_OpenGLMesh(const Handle<MeshData>& meshdata_handle);

    GPU_Texture* getGPUTexture(const Handle<Texture>& texture_handle);
    GPU_Texture* load_OpenGLTexture(const Handle<Texture>& texture_handle);
    
    glm::mat4 getMatrixFromCTransform(const CTransform& transform_component) const;

    void onWindowSizeChanged(const WindowSizeChangedEvent& event);

private:
	std::unordered_map<uint32_t, std::unique_ptr<GPU_Mesh>> m_gpuMeshes{};
	std::unordered_map<uint32_t, std::unique_ptr<GPU_Texture>> m_gpuTextures{};
    Handle<Texture> m_whiteTexture;
    Shader m_baseShader;
	Entity m_activeCamera{};
    WorldEnvironment m_env{};

    glm::uvec2 m_screenResolution;
};

}