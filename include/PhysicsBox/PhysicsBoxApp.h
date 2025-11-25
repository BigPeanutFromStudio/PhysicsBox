#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "PhysicsBox/Grid.h"

class PhysicsBoxApp {
 private:
  SDL_Window* m_Window;
  int m_windowHeight;
  int m_windowWidth;
  int m_pixelScale;
  SDL_Renderer* m_Renderer;
  SDL_Texture* m_Texture;
  bool m_Running = true;
  Grid m_grid;

  void Tick();
  void Update();
  void Render();
  void HandleEvents();

 public:
  void MainLoop();
  PhysicsBoxApp(int width, int height, int pixelScale);
  ~PhysicsBoxApp();
};
