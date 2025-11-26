#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

#include <vector>

#include "PhysicsBox/MaterialTypeEnum.h"

class Grid {
 private:
  int m_width;
  int m_height;
  std::vector<std::vector<MaterialType>> m_grid;

 public:
  Grid(int width, int height);

  void update();
  MaterialType getCell(int x, int y) const;
  void setCell(int x, int y, MaterialType material);
  int getWidth() const;
  int getHeight() const;
};
