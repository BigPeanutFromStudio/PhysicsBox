#include "PhysicsBox/InputManager.h"

#include <utility>

InputManager::InputManager() : m_MousePos(0, 0) {}

void InputManager::updateFrameState() {
  m_PrevKeyboardStates = m_KeyboardStates;
  m_PrevMouseStates = m_MouseStates;
}

void InputManager::processKeyboardEvents(SDL_Event& ev) {
  if (ev.type == SDL_EVENT_KEY_DOWN) {
    m_KeyboardStates[ev.key.key] = true;
  } else if (ev.type == SDL_EVENT_KEY_UP) {
    m_KeyboardStates[ev.key.key] = false;
  }
}

void InputManager::processMouseEvents(SDL_Event& ev) {
  if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    m_MouseStates[ev.button.button] = true;
  } else if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP) {
    m_MouseStates[ev.button.button] = false;
  }

  if (ev.type == SDL_EVENT_MOUSE_MOTION) {
    m_MousePos = {ev.motion.x, ev.motion.y};
  }
}

bool InputManager::wasKeyPressed(SDL_Keycode keycode) const {
  auto it = m_KeyboardStates.find(keycode);
  auto prev_it = m_PrevKeyboardStates.find(keycode);
  // TODO: Do this separately so it can work first frame
  if (it == m_KeyboardStates.end() || prev_it == m_PrevKeyboardStates.end()) {
    return false;
  }
  return it->second && !prev_it->second;
}

bool InputManager::wasMouseButtonPressed(int button) const {
  auto it = m_MouseStates.find(button);
  auto prev_it = m_PrevMouseStates.find(button);
  // TODO: Do this separately so it can work first frame
  if (it == m_MouseStates.end() || prev_it == m_PrevMouseStates.end()) {
    return false;
  }
  return it->second && !prev_it->second;
}

std::pair<int, int> InputManager::getMousePosition() const {
  return m_MousePos;
}

bool InputManager::isKeyDown(SDL_Keycode keycode) const {
  auto it = m_KeyboardStates.find(keycode);
  auto prev_it = m_PrevKeyboardStates.find(keycode);
  // TODO: Do this separately so it can work first frame
  if (it == m_KeyboardStates.end() || prev_it == m_PrevKeyboardStates.end()) {
    return false;
  }
  return it->second && prev_it->second;
}

bool InputManager::isMouseButtonDown(int button) const {
  auto it = m_MouseStates.find(button);
  auto prev_it = m_PrevMouseStates.find(button);
  // TODO: Do this separately so it can work first frame
  if (it == m_MouseStates.end() || prev_it == m_PrevMouseStates.end()) {
    return false;
  }
  return it->second && prev_it->second;
}
