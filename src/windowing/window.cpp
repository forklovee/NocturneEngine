#include "windowing/window.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdexcept>

namespace NocEngine {

Window::Window()
  : m_size(640, 340), m_title("Window")
{
  init();
}

Window::Window(Math::Vector2<uint32_t> size, const char *title)
    : m_size(size), m_title(title)
{
  init();
}

Window::~Window()
{
  glfwDestroyWindow(m_window);
}

void Window::init() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHintString(GLFW_X11_CLASS_NAME, "Nocturnis Engine");

  m_window =
      glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);

  if (!m_window) {
    throw std::runtime_error("Failed to create GLFW window");
  }
  else
  {
    std::cout << "GLFW window " << m_title << " created (" << m_size.x << "x"
      << m_size.y << ")\n";
  }

  glfwMakeContextCurrent(m_window);

  if (!gladLoadGL()) {
    std::cerr << "Failed to load GLAD.\n";
    return;
  }
}

} // namespace NocEngine
