#include "PhysicsBox/Grid.h"

#include <random>
#include <utility>
#include <vector>

Grid::Grid(int widht, int height)
    : m_height(height),
      m_width(widht),
      m_grid(m_height, std::vector<MaterialType>(m_width, MaterialType::AIR)),
      rng(std::random_device{}()) {
  m_materialProperties[MaterialType::SAND] = {2};
  m_materialProperties[MaterialType::WATER] = {1};
  m_materialProperties[MaterialType::AIR] = {0};
  m_materialProperties[MaterialType::STONE] = {0};
}

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
      if (isValid(j, i - 1)) {
        int targetCellDensity = m_materialProperties[getCell(j, i)].density;
        int higherCellDensity = m_materialProperties[getCell(j, i - 1)].density;
        if (higherCellDensity > targetCellDensity && targetCellDensity != 0 &&
            higherCellDensity != 0) {
          MaterialType higherCell = getCell(j, i - 1);
          setCell(j, i - 1, getCell(j, i));
          setCell(j, i, higherCell);
        }
      }
      switch (getCell(j, i)) {
        case (MaterialType::SAND):
          updateSand(j, i);
          break;
        case (MaterialType::WATER):
          updateWater(j, i);
          break;
        default:
          break;
      }
    }
  }
}

bool Grid::isValid(int x, int y) const {
  return x >= 0 && x < getWidth() && y >= 0 && y < getHeight();
}

void Grid::updateSand(int x, int y) {
  // Check below
  if (isValid(x, y + 1) && getCell(x, y + 1) == MaterialType::AIR) {
    setCell(x, y + 1, getCell(x, y));
    setCell(x, y, MaterialType::AIR);
    return;
  }

  std::vector<std::pair<int, int>> options;

  // Check below right
  if (isValid(x + 1, y + 1)) {
    if (getCell(x + 1, y + 1) == MaterialType::AIR &&
        getCell(x + 1, y) == MaterialType::AIR) {
      options.emplace_back(std::make_pair(x + 1, y + 1));
    }
  }

  // Check below left
  if (isValid(x - 1, y + 1)) {
    if (getCell(x - 1, y + 1) == MaterialType::AIR &&
        getCell(x - 1, y) == MaterialType::AIR) {
      options.emplace_back(std::make_pair(x - 1, y + 1));
    }
  }

  if (!options.empty()) {
    int randomIndex =
        std::uniform_int_distribution<int>(0, options.size() - 1)(rng);
    auto [nx, ny] = options[randomIndex];
    setCell(nx, ny, MaterialType::SAND);
    setCell(x, y, MaterialType::AIR);
    return;
  }
}

void Grid::updateWater(int x, int y) {
  // Check below
  if (isValid(x, y + 1) && getCell(x, y + 1) == MaterialType::AIR) {
    setCell(x, y + 1, getCell(x, y));
    setCell(x, y, MaterialType::AIR);
    return;
  }

  std::vector<std::pair<int, int>> options;

  // Check below right
  if (isValid(x + 1, y + 1)) {
    if (getCell(x + 1, y + 1) == MaterialType::AIR &&
        getCell(x + 1, y) == MaterialType::AIR) {
      options.emplace_back(std::make_pair(x + 1, y + 1));
    }
  }

  // Check below left
  if (isValid(x - 1, y + 1)) {
    if (getCell(x - 1, y + 1) == MaterialType::AIR &&
        getCell(x - 1, y) == MaterialType::AIR) {
      options.emplace_back(std::make_pair(x - 1, y + 1));
    }
  }

  if (!options.empty()) {
    int randomIndex =
        std::uniform_int_distribution<int>(0, options.size() - 1)(rng);
    auto [nx, ny] = options[randomIndex];
    setCell(nx, ny, MaterialType::WATER);
    setCell(x, y, MaterialType::AIR);
    return;
  }

  // If below, below left or below right are taken, then try to move right or
  // left
  if ((!isValid(x, y + 1) || getCell(x, y + 1) != AIR) &&
      (!isValid(x - 1, y + 1) || getCell(x - 1, y + 1) != AIR) &&
      (!isValid(x + 1, y + 1) || getCell(x + 1, y + 1) != AIR)) {
    std::vector<std::pair<int, int>> options;

    if (isValid(x + 1, y)) {
      if (getCell(x + 1, y) == AIR) {
        options.emplace_back(x + 1, y);
      }
    }

    if (isValid(x - 1, y)) {
      if (getCell(x - 1, y) == AIR) {
        options.emplace_back(x - 1, y);
      }
    }

    if (!options.empty()) {
      int randomIndex =
          std::uniform_int_distribution<int>(0, options.size() - 1)(rng);
      auto [nx, ny] = options[randomIndex];
      setCell(nx, ny, MaterialType::WATER);
      setCell(x, y, MaterialType::AIR);
    }
  }
}
