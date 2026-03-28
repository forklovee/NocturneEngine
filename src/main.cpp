#include <iostream>
#include <stdio.h>

#include "ecs/components/components.h"
#include "ecs/component_manager.h"
#include "ecs/entity_manager.h"
#include "core/window.h"

#include "meshdata.h"
#include "rendering_system.h"
#include "resource_manager.h"
#include <light_component.h>
#include <chrono>
#include <array>

using namespace NocEngine;

namespace NocEngine {
    Entity CreateEntity() {
        return EntityManager::Get().CreateEntity();
    }

    void KillEntity(Entity entity) {
        EntityManager::Get().KillEntity(entity);
    }

    template<ValidComponent T>
    T& GetComponent(Entity entity) {
        return ComponentManager::Get().GetComponent<T>(entity);
    }

    template<ValidComponent T>
    T& CreateComponent(Entity entity) {
        return ComponentManager::Get().CreateComponent<T>(entity);
    }

    template<ValidComponent T>
    void DestroyComponent(Entity entity) {
        ComponentManager::Get().DestroyComponent<T>(entity);
    }

    Handle<MeshData> LoadMesh(FilePath meshPath = "") {
        return ResourceManager::Get().Load<MeshData>(meshPath);
    }

    Handle<Texture> LoadTexture(FilePath texturePath = "") {
        return ResourceManager::Get().Load<Texture>(texturePath);
    }

    Handle<Material> LoadMaterial(FilePath materialPath = "") {
        return ResourceManager::Get().Load<Material>(materialPath);
    }

    template<ResourceType T>
    T* GetResource(Handle<T> handle) {
        return ResourceManager::Get().Get(handle);
    }
}

int main() {
  Window window{{1280, 720}, "NocEngine" };
  
  RenderingSystem renderingSystem;

  Handle<Texture> creepyTex = LoadTexture("assets/images/example.jpg");
  Handle<Texture> kittyTex = LoadTexture("assets/images/cat.jpg");
  Handle<Texture> whiteTex = LoadTexture("assets/images/white.bmp");
  
  Handle<Material> exampleMat1 = LoadMaterial("assets/material/example1.mat");
  if (Material* mat = exampleMat1.Get()) {
      mat->AlbedoTexture = creepyTex;
      mat->AmbientColor = glm::vec3(1.f, 1.f, 1.f);
      mat->DiffuseColor = glm::vec3(1.f, 1.f, 1.f);
      mat->SpecularColor = glm::vec3(1.f, 1.f, 1.f);
      mat->Roughness = 0.0f;
  }
  Handle<Material> exampleMat2 = LoadMaterial("assets/material/example2.mat");
  if (Material* mat = exampleMat2.Get()) {
      mat->AlbedoTexture = kittyTex;
      mat->AmbientColor = glm::vec3(1.f, 1.f, 1.f);
      mat->DiffuseColor = glm::vec3(1.f, 1.f, 1.f);
      mat->SpecularColor = glm::vec3(1.f, 1.f, 1.f);
      mat->Roughness = 0.f;
  }

  // Entities and components creation test
  const int entitesToSpawn{ 10 };
  std::array<Entity, entitesToSpawn> entitesToTransform{};
  const uint8_t cols{ 5 };
  for (size_t i{}; i < entitesToSpawn; i++) {
    const uint8_t row{ i % cols };

    Entity e = CreateEntity();
    entitesToTransform[i] = e;

    CTransform& tc = CreateComponent<CTransform>(e);
    tc.position.x = -static_cast<float>(cols) * .5f + (row * 1.5f);
    tc.position.z = 4.0 - ((e / cols) * 1.5f);

    CMeshRenderer& mr = CreateComponent<CMeshRenderer>(e);
    mr.mesh = LoadMesh("");
    mr.material = (i % 2 == 0) ? exampleMat1 : exampleMat2;

    if (i % 3 == 0){
      CreateComponent<CBoxShape>(e);
    }
    if (i % 2 == 0){
      CreateComponent<CSphereShape>(e);
    }
  }

  Entity lightSource = CreateEntity();
  CTransform& lightSourceTrans = CreateComponent<CTransform>(lightSource);
  lightSourceTrans.position = glm::vec3(1.5f, 0.f, 0.f);
  lightSourceTrans.scale = glm::vec3(0.15f);
  CLightComponent& light = CreateComponent<CLightComponent>(lightSource);
  CMeshRenderer& mr = CreateComponent<CMeshRenderer>(lightSource);
  mr.mesh = LoadMesh("");
  mr.material = exampleMat1;

  float lastTime = glfwGetTime();
  while (!window.ShouldClose()) {
    float deltaTime = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    window.PollEvents();
    window.ClearScreen();

    // Process

    lightSourceTrans.position = glm::vec3(
        5.5f * std::sin(lastTime),
        1.f,
        2.5f
    );

    for (Entity& e : entitesToTransform) {
        CTransform& t = GetComponent<CTransform>(e);
        t.rotation.y += 0.2 * deltaTime;
    }

    // Process


    // Present
    renderingSystem.Update();
    window.Present();
  }

  std::cout << "Window closed.\n";
}
