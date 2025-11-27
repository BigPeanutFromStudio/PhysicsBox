#pragma once

#include <SDL3/SDL.h>

#include <unordered_map>
#include <utility>

class InputManager {
 private:
  std::unordered_map<SDL_Keycode, bool> m_PrevKeyboardStates;
  std::unordered_map<SDL_Keycode, bool> m_KeyboardStates;
  std::unordered_map<int, bool> m_PrevMouseStates;
  std::unordered_map<int, bool> m_MouseStates;
  std::pair<int, int> m_MousePos;

 public:
  InputManager();

  bool wasKeyPressed(SDL_Keycode keycode) const;
  bool isKeyDown(SDL_Keycode keycode) const;

  bool wasMouseButtonPressed(int button) const;
  bool isMouseButtonDown(int button) const;
  std::pair<int, int> getMousePosition() const;

  void processKeyboardEvents(SDL_Event& ev);
  void processMouseEvents(SDL_Event& ev);
  void updateFrameState();
};
