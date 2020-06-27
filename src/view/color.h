#pragma once

#include <cstdint>

struct Color {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
};

inline constexpr Color BLACK = {};
inline constexpr Color GREEN = {0, 0xff, 0};
inline constexpr Color RED = {0xff, 0, 0};
