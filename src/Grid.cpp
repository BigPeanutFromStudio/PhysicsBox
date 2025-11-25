#include "PhysicsBox/Grid.h"

#include <vector>

Grid::Grid(int widht, int height)
    : m_height(height),
      m_width(widht),
      m_grid(m_height, std::vector<MaterialType>(m_width, MaterialType::AIR)) {}

int Grid::getHeight() const { return m_height; }
int Grid::getWidth() const { return m_width; }
MaterialType Grid::getCell(int x, int y) const { return m_grid[y][x]; }
void Grid::setCell(int x, int y, MaterialType material) {
  m_grid[y][x] = material;
}
void Grid::update(Uint64 dt) {}
