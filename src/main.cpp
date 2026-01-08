#include <iostream>
#include <stdio.h>

#include "math.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

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

  while (!window.ShouldClose()) {
    std::cout << "Window shouldn't close!\n";

    glfwSwapBuffers(window.GetHandle());
    glfwPollEvents();
  }

  std::cout << "Window closed.\n";

  glfwTerminate();
}
