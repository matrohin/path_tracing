#pragma once

#include "color.h"

#include <cstdint>
#include <vector>

struct View {
  uint32_t width = 0;
  uint32_t height = 0;
  std::vector<Color> pixels;

  View(uint32_t w, uint32_t h);
  Color& operator()(uint32_t i, uint32_t j);
  const Color& operator()(uint32_t i, uint32_t j) const;
};
