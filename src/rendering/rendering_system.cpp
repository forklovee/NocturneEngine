#include "rendering_system.h"
#include "shader.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>

#include <bitset>

#include "resource_manager.h"

#include "component_manager.h"
#include "components.h"
#include "entity_manager.h"
#include "meshdata.h"
#include "texture.h"
#include "material.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <chrono>
#include <limits>
#include <camera_component.h>
#include <light_component.h>

namespace NocEngine
{
    RenderingSystem::RenderingSystem()
        : m_baseShader("assets/shaders/base_vertex.glsl", "assets/shaders/base_fragment.glsl"),
            m_activeCamera(EntityManager::Get().CreateEntity()),
            m_env{}
    {
		CTransform& transformComponent = ComponentManager::Get().CreateComponent<CTransform>(m_activeCamera);
        transformComponent.rotation.x = glm::radians(15.f);
        transformComponent.position = glm::vec3(0.f, 0.f, -8.f);
        CCamera& cameraComponent = ComponentManager::Get().CreateComponent<CCamera>(m_activeCamera);
        
        m_whiteTexture = ResourceManager::Get().Load<Texture>("assets/images/white.bmp");

        glEnable(GL_DEPTH_TEST);
    }


    void RenderingSystem::Update()
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (m_activeCamera == INVALID_ENTITY) {
            std::cerr << "RenderingSystem: No Active Camera Set!\n";
            return;
        }

        m_baseShader.Use();

        // Process Camera
        CTransform& cameraTransform = ComponentManager::Get().GetComponent<CTransform>(m_activeCamera);
        glm::mat4 view = getMatrixFromCTransform(cameraTransform);

        CCamera& cameraProperties = GetActiveCameraComponent();
        glm::mat4 projection{};
        switch (cameraProperties.projection_type) {
            case CCamera::ProjectionType::Perspective:
                projection = glm::perspective(
                    glm::radians(cameraProperties.projection_fov),
                    1280 / 720.f, //TODO: change with actual window resolution
                    cameraProperties.near_plane, cameraProperties.far_plane);
                break;
            case CCamera::ProjectionType::Orthographic:
                projection = glm::ortho(
                    cameraProperties.ortho_left, cameraProperties.ortho_right,
                    cameraProperties.ortho_down, cameraProperties.ortho_top,
                    cameraProperties.near_plane, cameraProperties.far_plane);
                break;
        }

        // Set Ambient Light
        m_baseShader.SetUniformVec3("light.ambientColor", m_env.ambient_light);

        // Set PV matrices
        m_baseShader.SetUniformMat4("pvm.view", view);
        m_baseShader.SetUniformMat4("pvm.projection", projection);

        // Process Lights (Single light for now.)
        EntityManager::Get().ForEachWithBitmask(
            [this, &view](Entity entity) {
                CLightComponent& lc = ComponentManager::Get().GetComponent<CLightComponent>(entity);
                CTransform& tc = ComponentManager::Get().GetComponent<CTransform>(entity);
                glm::vec3 viewSpaceLightPosition = glm::vec3(view * glm::vec4(tc.position, 1.f));
                m_baseShader.SetUniformVec3("light.position", viewSpaceLightPosition);
                m_baseShader.SetUniformVec3("light.ambientColor", lc.ambientColor);
                m_baseShader.SetUniformVec3("light.diffuseColor", lc.diffuseColor);
                m_baseShader.SetUniformVec3("light.specularColor", lc.specularColor);
            },
            ComponentManager::Get().GetComponentBitset<CLightComponent>() |
            ComponentManager::Get().GetComponentBitset<CTransform>()
            );

        // Render all renderable entities
        EntityManager::Get().ForEachWithBitmask(
            [this](Entity entity) {
                renderEntity(entity);
            },
            getRequiredBitmask());
    }

    const std::bitset<64> RenderingSystem::GetRenderableEnityBitmask() const
    {
        ComponentManager& cm = ComponentManager::Get();
        return cm.GetComponentBitset<CMeshRenderer>() | cm.GetComponentBitset<CTransform>();
    }

    const Entity RenderingSystem::GetActiveCameraEntity() const
    {
        return m_activeCamera;
    }

    CCamera& RenderingSystem::GetActiveCameraComponent() const
    {
        assert(m_activeCamera != INVALID_ENTITY && "Active Camera Not Set!");
        return ComponentManager::Get().GetComponent<CCamera>(m_activeCamera);
    }

    const std::bitset<64> RenderingSystem::getRequiredBitmask() const
    {
        std::bitset<64> requiredBitmask{};
        ComponentManager& cm = ComponentManager::Get();
        requiredBitmask |= cm.GetComponentBitset<CTransform>();
        requiredBitmask |= cm.GetComponentBitset<CMeshRenderer>();
        return requiredBitmask;
    }

    void RenderingSystem::renderEntity(Entity entity)
    {
        ComponentManager& cm = ComponentManager::Get();
        CTransform& transform = cm.GetComponent<CTransform>(entity);
        CMeshRenderer& meshRenderer = cm.GetComponent<CMeshRenderer>(entity);

        if (!meshRenderer.mesh.IsValid()) {
            return;
        }
        if (!meshRenderer.gpu_mesh) { // Load GPU mesh, if not loaded
            meshRenderer.gpu_mesh = load_OpenGLMesh(meshRenderer.mesh);
        }

        Material* material = ResourceManager::Get().Get(meshRenderer.material);
        if (material)
        {
            if (!material->AlbedoTexture.IsValid()) {
                material->AlbedoTexture = m_whiteTexture;
            }

            if (!meshRenderer.gpu_texture) { // Load GPU texture, if not loaded
                meshRenderer.gpu_texture = load_OpenGLTexture(material->AlbedoTexture);
            }
        }

        // Set Model and Normal Matrices
		glm::mat4 model = getMatrixFromCTransform(transform);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
        m_baseShader.SetUniformMat4("pvm.model", model);
        m_baseShader.SetUniformMat4("pvm.normal", normalMatrix);

        // Set material properties
        if (material) {
            m_baseShader.SetUniformVec3("material.ambientColor", material->AmbientColor * m_env.ambient_light);
            m_baseShader.SetUniformVec3("material.diffuseColor", material->DiffuseColor);
            m_baseShader.SetUniformVec3("material.specularColor", material->SpecularColor);
            m_baseShader.SetUniformFloat("material.roughness", material->Roughness);
        }
        else {
            m_baseShader.SetUniformVec3("material.ambientColor", glm::vec3(1.f));
            m_baseShader.SetUniformVec3("material.diffuseColor", glm::vec3(1.f));
            m_baseShader.SetUniformVec3("material.specularColor", glm::vec3(1.f));
            m_baseShader.SetUniformFloat("material.roughness", 0.f);
        }

        // Bind mesh texture
        if (meshRenderer.gpu_texture) {
            meshRenderer.gpu_texture->Bind();
        }

        meshRenderer.gpu_mesh->Draw();
    }

    GPU_Mesh* RenderingSystem::getGPUMesh(const Handle<MeshData>& meshdata_handle)
    {
        auto it = m_gpuMeshes.find(meshdata_handle.id);
        if (it == m_gpuMeshes.end()) return nullptr;
	    if (it->second->generation == meshdata_handle.generation) return it->second.get();
        return nullptr;
    }

    GPU_Mesh* RenderingSystem::load_OpenGLMesh(const Handle<MeshData>& meshdata_handle)
    {
        if (!meshdata_handle.IsValid()) return nullptr;
        if (GPU_Mesh* gpu_mesh = getGPUMesh(meshdata_handle)){
            return gpu_mesh;
        }

        // If already exists, unload old texture
        uint32_t meshdata_id = meshdata_handle.id;
        if (m_gpuMeshes.contains(meshdata_id)) {
            m_gpuMeshes[meshdata_id]->Destroy();
        }

        MeshData* mesh_data = meshdata_handle.Get();
        if (!mesh_data) {
            return nullptr;
        }

        auto gpu_mesh = std::make_unique<OpenGL_Mesh>();
	    gpu_mesh->generation = meshdata_handle.generation;

        glGenVertexArrays(1, &gpu_mesh->VAO);
        glBindVertexArray(gpu_mesh->VAO);

        glGenBuffers(1, &gpu_mesh->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, gpu_mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, 
            mesh_data->vertices.size() * sizeof(float),
            mesh_data->vertices.data(),
            GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        // vertex texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        if (mesh_data->use_indices) {
            glGenBuffers(1, &gpu_mesh->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_mesh->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                mesh_data->indices.size() * sizeof(uint32_t),
                mesh_data->indices.data(),
                GL_STATIC_DRAW);
            gpu_mesh->index_count = mesh_data->indices.size();
        }
        else {
		    gpu_mesh->index_count = mesh_data->vertices.size() / 5; // 5 floats per vertex (3 for position, 2 for tex coords)
        }

        // Unbind Vertex Array & Buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	    m_gpuMeshes[meshdata_id] = std::move(gpu_mesh);
	    return m_gpuMeshes[meshdata_id].get();
    }

    GPU_Texture* RenderingSystem::getGPUTexture(const Handle<Texture>& texture_handle)
    {
        auto it = m_gpuTextures.find(texture_handle.id);
        if (it == m_gpuTextures.end()) return nullptr;
        if (it->second->generation == texture_handle.generation) return it->second.get();
        return nullptr;
    }

    GPU_Texture* RenderingSystem::load_OpenGLTexture(const Handle<Texture>& texture_handle)
    {
        if (!texture_handle.IsValid()) return nullptr;
        if (GPU_Texture* gpu_texture = getGPUTexture(texture_handle)) {
            return gpu_texture;
        }

	    // If already exists, unload old texture
	    uint32_t texture_id = texture_handle.id;
        if (m_gpuTextures.contains(texture_id)) {
		    m_gpuTextures[texture_id]->Destroy();
        }

	    Texture* texture = texture_handle.Get();
        if (!texture) {
            return nullptr;
        }

        if (texture->GetSize().length() == 0.f)
	        texture->Reload(); // Ensure texture is loaded, has data, size, etc.

	    auto gpu_texture = std::make_unique<OpenGL_Texture>();
	    gpu_texture->generation = texture_handle.generation;

	    glGenTextures(1, &gpu_texture->texture_id);
	    glBindTexture(GL_TEXTURE_2D, gpu_texture->texture_id);

	    GLint format = texture->GetChannels() == 4 ? GL_RGBA : GL_RGB;
	    glTexImage2D(GL_TEXTURE_2D, 0, format, 
            texture->GetSize().x, texture->GetSize().y, 0, format, GL_UNSIGNED_BYTE,
            texture->GetDataPtr());

	    texture->Unload(); // Free CPU memory after uploading to GPU

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        glBindTexture(GL_TEXTURE_2D, 0);

        m_gpuTextures[texture_id] = std::move(gpu_texture);
	    return m_gpuTextures[texture_id].get();
    }

    glm::mat4 RenderingSystem::getMatrixFromCTransform(const CTransform& transform_component) const
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, transform_component.position);
        matrix = glm::rotate(matrix, transform_component.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, transform_component.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, transform_component.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, transform_component.scale);
        return matrix;
    }

}