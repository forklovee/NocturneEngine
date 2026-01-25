#include <iostream>
#include <stdio.h>

#include "math.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "components/components.h"
#include "components/component_manager.h"
#include "entities/entity_manager.h"
#include "windowing/window.h"

using namespace NocEngine;

void error_callback(int error, const char *description) {
  fprintf(stderr, "GLFWError: %s\n", description);
}

int main() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  Window window{{1280, 720}, "ECS Debug"};

  EntityManager& em = EntityManager::Get();
  ComponentManager& cm = ComponentManager::Get();
  
  for (size_t i{}; i < 1024; i++) {
    Entity& e = em.CreateEntity();
    cm.CreateComponent<CTransform>(e);
    cm.CreateComponent<CBoxShape>(e);
    cm.CreateComponent<CSphereShape>(e);
  }

  while (!window.ShouldClose()) {
    em.Update();
    glfwSwapBuffers(window.GetHandle());
    glfwPollEvents();
  }


  const std::vector<Entity>& entities = em.GetEntities();
  std::cout << "Entities (" << entities.size() << ")\n";
  for (Entity& e: em.GetEntities()) {
    std::cout << e.GetId() << " " << e.GetComponentBits() << "\n";
  }

  std::cout << "Window closed.\n";

  glfwTerminate();
}
