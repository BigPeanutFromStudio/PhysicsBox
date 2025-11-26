// Copyright 2025 Piotr "Peanut" Bielański

#include <SDL3/SDL.h>

#include "PhysicsBox/PhysicsBoxApp.h"

int main() {
  PhysicsBoxApp app(1280, 720, 16, 100);
  app.MainLoop();

  return 0;
}
