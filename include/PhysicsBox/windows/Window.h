#pragma once
#include "imgui.h"

class Window {
  protected:
    bool m_opened;
    ImVec2 m_windowSize;

  public:
    Window(ImVec2 windowSize);
    virtual void createWindow();
    void closeWindow();
    void openWindow();
};
