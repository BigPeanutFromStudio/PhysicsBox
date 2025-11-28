#include <PhysicsBox/PhysicsBoxApp.h>
#include <SDL3/SDL_stdinc.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

PhysicsBoxApp::PhysicsBoxApp(int w, int h, int pixelScale, int updateRateMs)
    : m_windowHeight(h),
      m_windowWidth(w),
      m_pixelScale(pixelScale),
      m_grid(static_cast<int>(m_windowWidth / pixelScale),
             static_cast<int>(m_windowHeight / pixelScale)),
      m_updateGridTimer(0),
      m_gridUpdateRate(updateRateMs),
      m_InputManager() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
    return;
  }

  m_Window = SDL_CreateWindow("PhysicsBox", m_windowWidth, m_windowHeight, 0);

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
  m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING, m_grid.getWidth(),
                                m_grid.getHeight());
  SDL_SetTextureScaleMode(m_Texture, SDL_SCALEMODE_NEAREST);
}

PhysicsBoxApp::~PhysicsBoxApp() { SDL_Quit(); }

void PhysicsBoxApp::Update(Uint64 dt) {
  if (m_InputManager.isMouseButtonDown(1)) {
    std::pair<int, int> mousePos = m_InputManager.getMousePosition();

    int gridX = (mousePos.first / m_pixelScale);
    int gridY = (mousePos.second / m_pixelScale);
    m_grid.setCell(gridX, gridY, MaterialType::SAND);
  }

  m_updateGridTimer++;
  if (m_updateGridTimer >= m_gridUpdateRate) {
    m_grid.update();
    m_updateGridTimer = 0;
  }
  m_InputManager.updateFrameState();
}

void PhysicsBoxApp::Render() {
  std::vector<uint32_t> pixels(m_grid.getWidth() * m_grid.getHeight(),
                               0xff000000);
  int pitch = m_grid.getWidth() * sizeof(uint32_t);

  for (int i = 0; i < m_grid.getHeight(); i++) {
    for (int j = 0; j < m_grid.getWidth(); j++) {
      if (m_grid.getCell(j, i) == MaterialType::SAND) {
        pixels[i * m_grid.getWidth() + j] = 0xffff0000;
      }
    }
  }

  SDL_Rect dest = {0, 0, m_grid.getWidth() * m_pixelScale,
                   m_grid.getHeight() * m_pixelScale};

  SDL_UpdateTexture(m_Texture, &dest, pixels.data(),
                    m_grid.getWidth() * sizeof(uint32_t));

  SDL_RenderClear(m_Renderer);
  SDL_RenderTexture(m_Renderer, m_Texture, nullptr, nullptr);
  SDL_RenderPresent(m_Renderer);
}

void PhysicsBoxApp::HandleEvents() {
  SDL_Event ev;

  while (SDL_PollEvent(&ev)) {
    switch (ev.type) {
      case SDL_EVENT_QUIT: {
        m_Running = false;
        break;
      }
      case SDL_EVENT_KEY_DOWN: {
        if (ev.key.scancode == SDL_SCANCODE_ESCAPE) {
          m_Running = false;
        }
      }
      default:
        break;
    }
    m_InputManager.processKeyboardEvents(ev);
    m_InputManager.processMouseEvents(ev);
  }
}

void PhysicsBoxApp::Tick(Uint64 dt) {
  HandleEvents();
  Update(dt);
  Render();
}

void PhysicsBoxApp::MainLoop() {
  Uint64 lastTick = SDL_GetTicks();
  Uint64 lastTime = lastTick;
  Uint64 fps = 0;

  while (m_Running) {
    Uint64 currentTick = SDL_GetTicks();
    Uint64 deltaTime = SDL_GetTicks() - currentTick;
    lastTick = currentTick;

    Tick(deltaTime);

    fps++;

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
