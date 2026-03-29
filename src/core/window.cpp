#include "window.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>
#include <stdexcept>

#include <rendering_system.h>
#include <input_system.h>

#include "event_bus.h"
#include "events.h"

namespace NocEngine {

Window::Window(EventBus& eventBus, glm::vec2 size, const char *title)
    : m_eventBus(eventBus), m_size(size), m_title(title)
{
  init();
}

Window::~Window()
{
  glfwDestroyWindow(m_window);
  glfwTerminate();

  std::cout << "Window '" << m_title << "' destroyed.\n";
}

void Window::UpdateViewportSize() const
{
    m_eventBus.Notify(WindowSizeChangedEvent(m_size));
}

void Window::ClearScreen() const
{
  glClearColor(0, 0.15, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Present() const
{
  glfwSwapBuffers(m_window);
}

void Window::PollEvents() const
{
  glfwPollEvents();
}


void Window::errorCallback(int error, const char *description) {
  fprintf(stderr, "GLFWError: %s\n", description);
}

void Window::windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* targetWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!targetWindow) return;

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            return;
        }
    }

    targetWindow->m_eventBus.Notify(KeyEvent(key, scancode, action, mods));
}

void Window::framebufferSizeChangeCallback(GLFWwindow* window, int width, int height)
{
    Window* targetWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!targetWindow) return;

    glViewport(0, 0, width, height);
    targetWindow->m_eventBus.Notify(WindowSizeChangedEvent(width, height));
}

void Window::init() {
  // Create window
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHintString(GLFW_X11_CLASS_NAME, m_title.c_str());

  m_window =
      glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);

  if (!m_window) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(m_window);

  glfwSetWindowUserPointer(m_window, this);
  glfwSetErrorCallback(errorCallback);
  glfwSetKeyCallback(m_window, windowKeyCallback);
  glfwSetFramebufferSizeCallback(m_window, framebufferSizeChangeCallback);

  glfwSwapInterval(1);

  if (!gladLoadGL()) {
    throw std::runtime_error("Failed to load GLAD.");
  }

  std::cout << "Created window '" << m_title << "' (" << m_size.x << "x"
    << m_size.y << ")\n";
  
  // Configure renderer
  int fb_width, fb_height;
  glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
  glViewport(0, 0, fb_width, fb_height);
  m_eventBus.Notify(WindowSizeChangedEvent(fb_width, fb_height));
}

} // namespace NocEngine
