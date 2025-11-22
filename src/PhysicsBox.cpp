// Copyright 2025 Piotr "Peanut" Bielański

#include "PhysicsBox/MaterialTypeEnum.h"
#include <SDL3/SDL.h>
#include <iostream>

struct PhysicsBoxApp {

  SDL_Window *m_Window;
  SDL_Renderer *m_Renderer;
  bool m_Running = true;

  PhysicsBoxApp() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
      return;
    }

    m_Window = SDL_CreateWindow("PhysicsBox", 1280, 720, 0);

    if (!m_Window) {
      std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
      SDL_Quit();
    }

    m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
    if (!m_Renderer) {
      std::cerr << "SDL_Renderer failed: " << SDL_GetError() << "\n";
      SDL_DestroyWindow(m_Window);
      SDL_Quit();
    }
  }

  ~PhysicsBoxApp() { SDL_Quit(); }

  void Tick() {
    HandleEvents();
    Update();
    Render();
  }

  void MainLoop() {
    while (m_Running) {

      Tick();

      // 60 fps
      SDL_Delay(16);
    }

    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

  void Update() {}

  void Render() {

    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);

    SDL_RenderClear(m_Renderer);
    SDL_RenderPresent(m_Renderer);
  }

  void HandleEvents() {

    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_EVENT_QUIT:
        m_Running = false;
        break;
      case SDL_EVENT_KEY_DOWN:
        if (ev.key.scancode == SDL_SCANCODE_ESCAPE) {
          m_Running = false;
        }
        break;
      default:
        break;
      }
    }
  }
};
int main() {

  PhysicsBoxApp app;
  app.MainLoop();

  return 0;
}
