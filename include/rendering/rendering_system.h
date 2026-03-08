#pragma once

#include <memory>
#include <bitset>
#include "components.h"
#include "entity.h"

#include "gpu_mesh.h"
#include "gpu_texture.h"
#include "shader.h"
#include "meshdata.h"
#include "texture.h"
#include "resource_handle.h"
#include <camera_component.h>
#include <world_environment.h>

namespace NocEngine {

class RenderingSystem
{
public:
    RenderingSystem();
    ~RenderingSystem() = default;

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& other) = delete;
	RenderingSystem(RenderingSystem&& other) noexcept = delete;
	RenderingSystem& operator=(RenderingSystem&& other) noexcept = delete;

    void Update();

    const std::bitset<64> GetRenderableEnityBitmask() const;
    const Entity GetActiveCameraEntity() const;
    CCamera& GetActiveCameraComponent() const;

private:
    const std::bitset<64> getRequiredBitmask() const;

    void renderEntity(Entity entity);
    GPU_Mesh* getGPUMesh(const ResourceHandle<MeshData>& meshdata_handle);
    GPU_Mesh* load_OpenGLMesh(const ResourceHandle<MeshData>& meshdata_handle);

    GPU_Texture* getGPUTexture(const ResourceHandle<Texture>& texture_handle);
    GPU_Texture* load_OpenGLTexture(const ResourceHandle<Texture>& texture_handle);
    void createWhiteTexture();
    
    glm::mat4 getMatrixFromCTransform(const CTransform& transform_component) const;

private:
	std::unordered_map<uint32_t, std::unique_ptr<GPU_Mesh>> m_gpuMeshes{};
	std::unordered_map<uint32_t, std::unique_ptr<GPU_Texture>> m_gpuTextures{};
    GPU_Texture* m_white_texture;
    Shader m_baseShader;
	Entity m_activeCamera{};
    WorldEnvironment m_env{};
};

}