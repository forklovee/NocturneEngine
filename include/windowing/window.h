#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <math.h>

#include <string>

namespace NocEngine {

class Window {
public:
  Window();
  Window(Math::Vector2<uint32_t> size, const char *title);
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;
  Window(Window &&other) = delete;
  Window &operator=(Window &&other) = delete;

  bool ShouldClose() const { return glfwWindowShouldClose(m_window); };
  GLFWwindow* GetHandle() const { return m_window; };

private:
  void init();

private:
  GLFWwindow *m_window;
  std::string m_title{};
  Math::Vector2<uint32_t> m_size{};
};

} // namespace NocEngine
