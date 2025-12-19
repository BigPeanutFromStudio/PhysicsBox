#include "PhysicsBox/windows/Window.h"

Window::Window(ImVec2 windowSize)
    : m_windowSize(windowSize)
    , m_opened(true) {}

void Window::createWindow() {
    ImGui::Begin("Window");
    ImGui::End();
}

void Window::closeWindow() {
    m_opened = false;
}

void Window::openWindow() {
    m_opened = true;
}
