#pragma once

#include "color.h"

#include <cstdint>
#include <vector>

using ViewRow = std::vector<Color>;

struct View {
  uint32_t width = 0;
  uint32_t height = 0;
  std::vector<std::vector<Color>> pixels;

  View(uint32_t w, uint32_t h);
  Color& operator()(uint32_t x, uint32_t y);
  const Color& operator()(uint32_t x, uint32_t y) const;
  void apply_row(ViewRow&& i_row, uint32_t y);
};

inline View::View(uint32_t w, uint32_t h) : width(w), height(h), pixels(h) {}

inline Color& View::operator()(uint32_t x, uint32_t y) {
  return pixels[y][x];
}
inline const Color& View::operator()(uint32_t x, uint32_t y) const {
  return pixels[y][x];
}

inline void View::apply_row(ViewRow&& i_row, uint32_t y) {
  pixels[y] = std::move(i_row);
}
