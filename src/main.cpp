#include <iostream>
#include <stdio.h>

#include "math.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "component_manager.h"
#include "windowing/window.h"

void error_callback(int error, const char *description) {
  fprintf(stderr, "GLFWError: %s\n", description);
}

int main() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  NocEngine::Window window{{1280, 720}, "ECS Debug"};

  NocEngine::ComponentManager& cm = NocEngine::ComponentManager::Get();


  while (!window.ShouldClose()) {
    glfwSwapBuffers(window.GetHandle());
    glfwPollEvents();
  }

  std::cout << "Window closed.\n";

  glfwTerminate();
}
