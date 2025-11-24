#pragma once
#include <SDL3/SDL.h>

class PhysicsBoxApp {
private:
  SDL_Window *m_Window;
  SDL_Renderer *m_Renderer;
  bool m_Running = true;

  void Tick();
  void Update();
  void Render();
  void HandleEvents();

public:
  void MainLoop();
  PhysicsBoxApp();
  ~PhysicsBoxApp();
};
