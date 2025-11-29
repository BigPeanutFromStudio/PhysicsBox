#pragma once
#include <SDL3/SDL.h>

#include <vector>

#include "PhysicsBox/Grid.h"
#include "PhysicsBox/InputManager.h"

class PhysicsBoxApp {
 private:
  SDL_Window* m_window;
  int m_windowHeight;
  int m_windowWidth;
  int m_pixelScale;
  SDL_Renderer* m_renderer;
  std::vector<uint32_t> m_pixels;
  SDL_Texture* m_texture;
  bool m_running = true;
  Grid m_grid;
  int m_gridUpdateRate;
  int m_updateGridTimer;
  InputManager m_inputManager;

  void tick(Uint64 dt);
  void update(Uint64 dt);
  void render();
  void handleEvents();

 public:
  void mainLoop();
  PhysicsBoxApp(int width, int height, int pixelScale, int updateRateMs);
  ~PhysicsBoxApp();
};
