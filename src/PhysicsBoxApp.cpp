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
      m_inputManager() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
    return;
  }

  m_window = SDL_CreateWindow("PhysicsBox", m_windowWidth, m_windowHeight, 0);

  if (!m_window) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
    SDL_Quit();
  }

  m_renderer = SDL_CreateRenderer(m_window, nullptr);
  if (!m_renderer) {
    std::cerr << "SDL_Renderer failed: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(m_window);
    SDL_Quit();
  }
  m_pixels =
      std::vector<uint32_t>(m_grid.getWidth() * m_grid.getHeight(), 0xff000000),
  m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING, m_grid.getWidth(),
                                m_grid.getHeight());
  SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
}

PhysicsBoxApp::~PhysicsBoxApp() { SDL_Quit(); }

void PhysicsBoxApp::update(Uint64 dt) {
  if (m_inputManager.isMouseButtonDown(MouseButton::LEFT)) {
    std::pair<int, int> mousePos = m_inputManager.getMousePosition();

    int gridX = (mousePos.first / m_pixelScale);
    int gridY = (mousePos.second / m_pixelScale);
    m_grid.setCell(gridX, gridY, MaterialType::SAND);
  }

  m_updateGridTimer++;
  if (m_updateGridTimer >= m_gridUpdateRate) {
    m_grid.update();
    m_updateGridTimer = 0;
  }
  m_inputManager.updateFrameState();
}

void PhysicsBoxApp::render() {
  std::fill(m_pixels.begin(), m_pixels.end(), 0xff000000);

  int pitch = m_grid.getWidth() * sizeof(uint32_t);

  for (int i = 0; i < m_grid.getHeight(); i++) {
    for (int j = 0; j < m_grid.getWidth(); j++) {
      if (m_grid.getCell(j, i) == MaterialType::SAND) {
        m_pixels[i * m_grid.getWidth() + j] = 0xffff0000;
      }
    }
  }

  SDL_Rect dest = {0, 0, m_grid.getWidth() * m_pixelScale,
                   m_grid.getHeight() * m_pixelScale};

  SDL_UpdateTexture(m_texture, &dest, m_pixels.data(),
                    m_grid.getWidth() * sizeof(uint32_t));

  SDL_RenderClear(m_renderer);
  SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
  SDL_RenderPresent(m_renderer);
}

void PhysicsBoxApp::handleEvents() {
  SDL_Event ev;

  while (SDL_PollEvent(&ev)) {
    switch (ev.type) {
      case SDL_EVENT_QUIT: {
        m_running = false;
        break;
      }
      case SDL_EVENT_KEY_DOWN: {
        if (ev.key.scancode == SDL_SCANCODE_ESCAPE) {
          m_running = false;
        }
      }
      default:
        break;
    }
    m_inputManager.processKeyboardEvents(ev);
    m_inputManager.processMouseEvents(ev);
  }
}

void PhysicsBoxApp::tick(Uint64 dt) {
  handleEvents();
  update(dt);
  render();
}

void PhysicsBoxApp::mainLoop() {
  Uint64 lastTick = SDL_GetTicks();
  Uint64 lastTime = lastTick;
  Uint64 fps = 0;

  while (m_running) {
    Uint64 currentTick = SDL_GetTicks();
    Uint64 deltaTime = SDL_GetTicks() - currentTick;
    lastTick = currentTick;

    tick(deltaTime);

    fps++;

    if (currentTick > lastTime + 1000) {
      lastTime = currentTick;
      std::string title = "PhysicsBox " + std::to_string(fps);
      SDL_SetWindowTitle(m_window, title.c_str());
      fps = 0;
    }
  }

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}
