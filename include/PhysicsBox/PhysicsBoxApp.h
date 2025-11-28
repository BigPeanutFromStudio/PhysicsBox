#pragma once
#include <SDL3/SDL.h>

#include <vector>

#include "PhysicsBox/Grid.h"
#include "PhysicsBox/InputManager.h"

class PhysicsBoxApp {
 private:
  SDL_Window* m_Window;
  int m_windowHeight;
  int m_windowWidth;
  int m_pixelScale;
  SDL_Renderer* m_Renderer;
  std::vector<uint32_t> m_Pixels;
  SDL_Texture* m_Texture;
  bool m_Running = true;
  Grid m_grid;
  int m_gridUpdateRate;
  int m_updateGridTimer;
  InputManager m_InputManager;

  void Tick(Uint64 dt);
  void Update(Uint64 dt);
  void Render();
  void HandleEvents();

 public:
  void MainLoop();
  PhysicsBoxApp(int width, int height, int pixelScale, int updateRateMs);
  ~PhysicsBoxApp();
};
