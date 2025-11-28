#include "PhysicsBox/Grid.h"

#include <cstdlib>
#include <utility>
#include <vector>

Grid::Grid(int widht, int height)
    : m_height(height),
      m_width(widht),
      m_grid(m_height, std::vector<MaterialType>(m_width, MaterialType::AIR)) {}

int Grid::getHeight() const { return m_height; }
int Grid::getWidth() const { return m_width; }
MaterialType Grid::getCell(int x, int y) const { return m_grid[y][x]; }
void Grid::setCell(int x, int y, MaterialType material) {
  if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) {
    return;
  }
  m_grid[y][x] = material;
}
void Grid::update() {
  for (int i = getHeight() - 1; i >= 0; --i) {
    for (int j = 0; j < getWidth(); j++) {
      switch (getCell(j, i)) {
        case (MaterialType::SAND): {
          if (i + 1 >= getHeight()) {
            continue;
          }

          // Check below
          if (getCell(j, i + 1) == MaterialType::AIR) {
            setCell(j, i + 1, getCell(j, i));
            setCell(j, i, MaterialType::AIR);
            continue;
          }

          std::vector<std::pair<int, int>> options;

          // Check below right
          if (j + 1 < getWidth()) {
            if (getCell(j + 1, i + 1) == MaterialType::AIR) {
              options.emplace_back(std::make_pair(j + 1, i + 1));
            }
          }

          // Check below left
          if (j - 1 >= 0) {
            if (getCell(j - 1, i + 1) == MaterialType::AIR) {
              options.emplace_back(std::make_pair(j - 1, i + 1));
            }
          }

          if (!options.empty()) {
            auto [x, y] = options[rand() % options.size()];
            setCell(x, y, MaterialType::SAND);
            setCell(j, i, MaterialType::AIR);
            continue;
          }

          break;
        }
        default:
          break;
      }
    }
  }
}
