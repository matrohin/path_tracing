#include "png_utils.h"

#include <png.h>
#include <cstdio>
#include <memory>
#include <optional>

namespace {

struct PngStruct {
  png_struct* png_ptr = nullptr;
  png_info* info_ptr = nullptr;

  PngStruct(FILE* file, uint32_t width, uint32_t height);
  PngStruct(const PngStruct&) = delete;
  PngStruct& operator=(const PngStruct&) = delete;
  PngStruct(PngStruct&&) = delete;
  PngStruct& operator=(PngStruct&&) = delete;
  ~PngStruct() { png_destroy_write_struct(&png_ptr, &info_ptr); }

  bool is_ok() const { return info_ptr && png_ptr; }
};

PngStruct::PngStruct(FILE* file, uint32_t width, uint32_t height) {
  png_ptr =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) return;

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, nullptr);
    return;
  }

  png_init_io(png_ptr, file);
  png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);
}

}  // unnamed namespace

namespace png_utils {

bool write_png(const char* file_name, const View& image) {
  std::unique_ptr<FILE, decltype(fclose)*> file{fopen(file_name, "wb"), fclose};
  if (!file) return false;

  PngStruct png{file.get(), image.width, image.height};
  if (!png.is_ok()) return false;

  std::vector<png_byte> row(image.width * 3);
  for (uint32_t y = 0; y < image.height; ++y) {
    for (uint32_t x = 0; x < image.width; ++x) {
      const auto i = x * 3;
      const auto& pixel = image(x, y);
      row[i + 0] = pixel.r;
      row[i + 1] = pixel.g;
      row[i + 2] = pixel.b;
    }
    auto p = row.data();
    png_write_rows(png.png_ptr, &p, 1);
  }

  png_write_end(png.png_ptr, png.info_ptr);
  return true;
}

}  // namespace png_utils
