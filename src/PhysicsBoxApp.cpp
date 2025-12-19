#include <PhysicsBox/PhysicsBoxApp.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "PhysicsBox/Colors.h"
#include "PhysicsBox/Grid.h"
#include "PhysicsBox/InputManager.h"
#include "PhysicsBox/windows/BrushSelectWindow.h"

PhysicsBoxApp::PhysicsBoxApp(int w, int h, int pixelScale, int updateRateMs)
    : m_windowHeight(h)
    , m_windowWidth(w)
    , m_pixelScale(pixelScale)
    , m_grid(static_cast<int>(m_windowWidth / pixelScale), static_cast<int>(m_windowHeight / pixelScale))
    , m_updateGridTimer(0)
    , m_gridUpdateRate(updateRateMs)
    , m_inputManager()
    , m_brushSize(1)
    , m_brushMaterial(MaterialType::SAND)
    , m_brushSelectWindow(ImVec2(m_windowWidth / 4, m_windowHeight / 5), &m_brushSize, &m_brushMaterial) {
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
    m_pixels = std::vector<uint32_t>(m_grid.getWidth() * m_grid.getHeight(), 0xff000000),
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_grid.getWidth(),
                                  m_grid.getHeight());
    SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer3_Init(m_renderer);
}

PhysicsBoxApp::~PhysicsBoxApp() {
    SDL_Quit();
}

void PhysicsBoxApp::update(Uint64 dt) {
    if (m_inputManager.isMouseButtonDown(MouseButton::LEFT)) {
        std::pair<int, int> mousePos = m_inputManager.getMousePosition();
        placeMaterial(mousePos, m_brushMaterial);
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
            m_pixels[i * m_grid.getWidth() + j] = colorForMaterial(m_grid.getCell(j, i));
        }
    }

    SDL_Rect dest = {0, 0, m_grid.getWidth() * m_pixelScale, m_grid.getHeight() * m_pixelScale};

    SDL_UpdateTexture(m_texture, &dest, m_pixels.data(), m_grid.getWidth() * sizeof(uint32_t));

    SDL_RenderClear(m_renderer);
    SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
    // SDL_RenderPresent(m_renderer);

    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);

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
        ImGui_ImplSDL3_ProcessEvent(&ev);
        if (!m_io->WantCaptureKeyboard) {
            m_inputManager.processKeyboardEvents(ev);
        }
        if (!m_io->WantCaptureMouse) {
            m_inputManager.processMouseEvents(ev);
        }
    }
}

void PhysicsBoxApp::tick(Uint64 dt) {
    handleEvents();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    m_brushSelectWindow.createWindow();

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

uint32_t PhysicsBoxApp::colorForMaterial(MaterialType material) {
    switch (material) {
    case MaterialType::AIR:
        return AIR_COLOR;
        break;
    case MaterialType::SAND:
        return SAND_COLOR;
        break;
    case MaterialType::WATER:
        return WATER_COLOR;
        break;
    case MaterialType::STONE:
        return STONE_COLOR;
        break;
    default:
        return 0xff000000;
        break;
    }
}

void PhysicsBoxApp::placeMaterial(std::pair<int, int> mousePos, MaterialType material) {
    int gridX = (mousePos.first / m_pixelScale);
    int gridY = (mousePos.second / m_pixelScale);
    for (int i = -(m_brushSize / 2); i <= (m_brushSize / 2); i++) {
        for (int j = -(m_brushSize / 2); j <= (m_brushSize / 2); j++) {
            m_grid.setCell(gridX + j, gridY + i, material);
        }
    }
}
