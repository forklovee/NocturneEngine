#include "rendering_system.h"
#include "shader.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>

#include <bitset>

#include "component_manager.h"
#include "components.h"
#include "entity_manager.h"
#include "mesh.h"

namespace NocEngine
{

void RenderingSystem::Update()
{
    EntityManager::Get().ForEachWithBitmask(
        [this](Entity entity) {
            renderEntity(entity);
        },
        getRequiredBitmask());
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

    if (!isMeshLoadedOnGPU(meshRenderer)){
        loadToGPU(meshRenderer);
    }

    if (Shader* shader = meshRenderer.shader_ptr){
        shader->Use();
    }
    glBindVertexArray(meshRenderer.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool RenderingSystem::isMeshLoadedOnGPU(const CMeshRenderer& mr) const
{
    return mr.VAO != 0;
}

void RenderingSystem::loadToGPU(CMeshRenderer& mr) const
{
    if (!mr.mesh_ptr){
        return;
    }

    std::cout << "Load mesh. " << mr.mesh_ptr << "\n";
    const MeshData* mesh = mr.mesh_ptr;

    glGenVertexArrays(1, &mr.VAO);
    glBindVertexArray(mr.VAO);

    glGenBuffers(1, &mr.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mr.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh->vertices), mesh->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mr.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mr.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh->indices), mesh->indices.data(), GL_STATIC_DRAW);

    // Unbind Vertex Array & Buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}