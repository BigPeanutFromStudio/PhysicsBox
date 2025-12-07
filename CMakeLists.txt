cmake_minimum_required(VERSION 3.20)
project(PhysicsBox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory (safe for Linux + Windows)
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Collect all cpp files from src/
file(GLOB_RECURSE SOURCES src/*.cpp)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendored/imgui")

# 2. Define ImGui Sources (Core + Backends)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    
    # Platform Backend (SDL3)
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp 
    
    # Renderer Backend (Specific to SDL Renderer)
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp 

    # (Optional) std::string support
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_executable(PhysicsBox ${SOURCES} ${IMGUI_DIR})

# Include the header directory
target_include_directories(PhysicsBox PRIVATE ${PROJECT_SOURCE_DIR}/include ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Link to the actual SDL3 library.
target_link_libraries(PhysicsBox PRIVATE SDL3)
