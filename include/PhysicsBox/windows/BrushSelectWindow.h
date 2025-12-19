#pragma once
#include "PhysicsBox/Grid.h"
#include "PhysicsBox/windows/Window.h"

class BrushSelectWindow : public Window {
  private:
    int *m_brushSize;
    MaterialType *m_brushMaterial;

  public:
    BrushSelectWindow(ImVec2 windowSize, int *brushSize, MaterialType *brushMaterial);
    virtual void createWindow() override;
};
