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

add_executable(PhysicsBox ${SOURCES})

# Include the header directory
target_include_directories(PhysicsBox PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Link to the actual SDL3 library.
target_link_libraries(PhysicsBox PRIVATE SDL3)
