#include <PhysicsBox/PhysicsBoxApp.h>
#include <iostream>

PhysicsBoxApp::PhysicsBoxApp() {

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

PhysicsBoxApp::~PhysicsBoxApp() { SDL_Quit(); }

void PhysicsBoxApp::Update() {}

void PhysicsBoxApp::Render() {
  SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);

  SDL_RenderClear(m_Renderer);
  SDL_RenderPresent(m_Renderer);
}

void PhysicsBoxApp::HandleEvents() {

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

void PhysicsBoxApp::Tick() {
  HandleEvents();
  Update();
  Render();
}

void PhysicsBoxApp::MainLoop() {
  Uint64 lastTime;
  Uint64 fps;

  while (m_Running) {
    Uint64 currentTick = SDL_GetTicks();
    Tick();
    SDL_Delay(16);
    fps++;
    Uint64 deltaTime = SDL_GetTicks() - currentTick;
    if (currentTick > lastTime + 1000) {
      lastTime = currentTick;
      std::string title = "PhysicsBox " + std::to_string(fps);
      SDL_SetWindowTitle(m_Window, title.c_str());
      fps = 0;
    }
  }

  SDL_DestroyRenderer(m_Renderer);
  SDL_DestroyWindow(m_Window);
  SDL_Quit();
}
