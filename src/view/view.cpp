#include "view.h"

View::View(uint32_t w, uint32_t h)
    : width(w), height(h), pixels(w * h) {}

Color& View::operator()(uint32_t x, uint32_t y) { return pixels[y * width + x]; }
const Color& View::operator()(uint32_t x, uint32_t y) const {
  return pixels[y * width + x];
}

