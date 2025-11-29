#include "PhysicsBox/InputManager.h"

#include <utility>

InputManager::InputManager() : m_mousePos(0, 0) {}

void InputManager::updateFrameState() {
  m_prevKeyboardStates = m_keyboardStates;
  m_prevMouseStates = m_mouseStates;
}

void InputManager::processKeyboardEvents(SDL_Event& ev) {
  if (ev.type == SDL_EVENT_KEY_DOWN) {
    m_keyboardStates[ev.key.key] = true;
  } else if (ev.type == SDL_EVENT_KEY_UP) {
    m_keyboardStates[ev.key.key] = false;
  }
}

void InputManager::processMouseEvents(SDL_Event& ev) {
  if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    m_mouseStates[ev.button.button] = true;
  } else if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP) {
    m_mouseStates[ev.button.button] = false;
  }

  if (ev.type == SDL_EVENT_MOUSE_MOTION) {
    m_mousePos = {ev.motion.x, ev.motion.y};
  }
}

bool InputManager::wasKeyPressed(SDL_Keycode keycode) const {
  auto it = m_keyboardStates.find(keycode);
  auto prev_it = m_prevKeyboardStates.find(keycode);
  if (it == m_keyboardStates.end()) {
    return false;
  }
  if (prev_it == m_prevKeyboardStates.end()) {
    return it->second;
  }
  return it->second && !prev_it->second;
}

bool InputManager::wasMouseButtonPressed(int button) const {
  auto it = m_mouseStates.find(button);
  auto prev_it = m_prevMouseStates.find(button);
  if (it == m_mouseStates.end()) {
    return false;
  }
  if (prev_it == m_prevMouseStates.end()) {
    return it->second;
  }
  return it->second && !prev_it->second;
}

std::pair<int, int> InputManager::getMousePosition() const {
  return m_mousePos;
}

bool InputManager::isKeyDown(SDL_Keycode keycode) const {
  auto it = m_keyboardStates.find(keycode);
  auto prev_it = m_prevKeyboardStates.find(keycode);
  if (it == m_keyboardStates.end()) {
    return false;
  }
  if (prev_it == m_prevKeyboardStates.end()) {
    return it->second;
  }
  return it->second && prev_it->second;
}

bool InputManager::isMouseButtonDown(int button) const {
  auto it = m_mouseStates.find(button);
  auto prev_it = m_prevMouseStates.find(button);
  if (it == m_mouseStates.end()) {
    return false;
  }
  if (prev_it == m_prevMouseStates.end()) {
    return it->second;
  }
  return it->second && prev_it->second;
}
