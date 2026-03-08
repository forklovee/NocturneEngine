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

using namespace NocEngine;

namespace NocEngine {
    Entity CreateEntity() {
        return EntityManager::Get().CreateEntity();
    }

    template<ValidComponent T>
    T& CreateComponent(Entity entity) {
        return ComponentManager::Get().CreateComponent<T>(entity);
    }
}

int main() {
  Window window{{1280, 720}, "NocEngine" };
  RenderingSystem renderingSystem;

  ResourceManager& resource_manager = ResourceManager::Get();

  Entity lightSource = CreateEntity();
  CTransform& tc = CreateComponent<CTransform>(lightSource);
  tc.position = glm::vec3(1.5f, 0.f, 0.f);
  tc.scale = glm::vec3(0.5f);
  CLightComponent& light = CreateComponent<CLightComponent>(lightSource);
  CMeshRenderer& mr = CreateComponent<CMeshRenderer>(lightSource);
  mr.meshdata_handle = resource_manager.Load<NocEngine::MeshData>("");
  mr.texture_handle = resource_manager.Load<Texture>("assets/images/white.bmp");

  // Entities and components creation test
  const uint8_t cols{ 5 };
  for (size_t i{}; i < 10; i++) {
    const uint8_t row{ i % cols };

    Entity e = CreateEntity();
    CTransform& tc = CreateComponent<CTransform>(e);
    tc.position.x = -static_cast<float>(cols) * .5f + (row * 1.5f);
    tc.position.z = 4.0 - ((e / cols) * 1.5f);

    CMeshRenderer& mr = CreateComponent<CMeshRenderer>(e);
    mr.meshdata_handle = resource_manager.Load<NocEngine::MeshData>("");
    mr.texture_handle = resource_manager.Load<Texture>(
        (i % 2) ? "assets/images/example.jpg" : "assets/images/cat.jpg"
    );

    if (i % 3 == 0){
      CreateComponent<CBoxShape>(e);
    }
    if (i % 2 == 0){
      CreateComponent<CSphereShape>(e);
    }
  }

  std::vector<CTransform>& transforms = ComponentManager::Get().GetComponents<CTransform>();

  std::bitset<64> renderableMeshesBitmask{ renderingSystem.GetRenderableEnityBitmask() };

  while (!window.ShouldClose()) {
    float time = glfwGetTime();
    
    window.PollEvents();
    window.ClearScreen();

    tc.position = glm::vec3(
        5.5f * std::sin(time),
        5.5f * std::cos(time),
        5.5f * std::sin(time)
    );

    renderingSystem.Update();

    window.Present();
  }

  std::cout << "Window closed.\n";
}
