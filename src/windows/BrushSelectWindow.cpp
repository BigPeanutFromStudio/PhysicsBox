#include "PhysicsBox/windows/BrushSelectWindow.h"
#include <imgui.h>

BrushSelectWindow::BrushSelectWindow(ImVec2 windowSize, int *brushSize, MaterialType *brushMaterial)
    : Window(windowSize)
    , m_brushSize(brushSize)
    , m_brushMaterial(brushMaterial) {}

void BrushSelectWindow::createWindow() {
    ImGui::SetNextWindowSize(m_windowSize);
    // This makes a window unmovable, if needed it should only happen once
    // ImGui::SetNextWindowPos(ImVec2(30, 30));
    //
    // Begin a window
    if (ImGui::Begin("Brush Select", &m_opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav)) {

        // Slider
        ImGui::SliderInt("Brush size", m_brushSize, 1, 10);

        const char *items[] = {"Air", "Sand", "Water", "Stone"};

        const char *combo_preview_value = items[int(*m_brushMaterial)];
        if (ImGui::BeginCombo("Brush material", combo_preview_value)) {
            for (int n = 0; n < 4; n++) {
                const bool is_selected = ((MaterialType)n == *m_brushMaterial);

                if (ImGui::Selectable(items[n], is_selected)) {
                    *m_brushMaterial = (MaterialType)n;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
    // End a window
    ImGui::End();
}
