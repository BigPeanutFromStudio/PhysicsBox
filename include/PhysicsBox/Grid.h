#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

#include <random>
#include <unordered_map>
#include <vector>

#include "PhysicsBox/MaterialProperty.h"
#include "PhysicsBox/MaterialTypeEnum.h"

class Grid {
 private:
  int m_width;
  int m_height;
  std::vector<MaterialType> m_grid;
  std::mt19937 rng;
  std::unordered_map<MaterialType, MaterialProperty> m_materialProperties;

 public:
  Grid(int width, int height);

  void update();
  void updateSand(int x, int y);
  void updateWater(int x, int y);
  MaterialType getCell(int x, int y) const;
  void setCell(int x, int y, MaterialType material);
  int getWidth() const;
  int getHeight() const;
  bool isValid(int x, int y) const;
};
