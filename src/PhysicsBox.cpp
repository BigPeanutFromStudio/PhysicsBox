// Copyright 2025 Piotr "Peanut" Bielański

#include <SDL3/SDL.h>

#include "PhysicsBox/PhysicsBoxApp.h"

int main() {
    PhysicsBoxApp app(1280, 720, 8, 50);
    app.mainLoop();

    return 0;
}
