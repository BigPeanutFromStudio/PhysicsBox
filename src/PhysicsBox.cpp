// Copyright 2025 Piotr "Peanut" Bielański

#include <SDL3/SDL.h>

#include "PhysicsBox/PhysicsBoxApp.h"

int main() {
  PhysicsBoxApp app(720, 720, 12);
  app.MainLoop();

  return 0;
}
