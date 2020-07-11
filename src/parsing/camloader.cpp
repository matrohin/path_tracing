#include "camloader.h"

#include "error.h"
#include "parsing_utils.h"
#include "rendering/camera.h"

namespace parsing {

// Loads .cam file with Camera parameters
Camera load_cam_file(const char* file_name, uint32_t width, uint32_t height) {
  Point3d pos{0., 0., 0.};
  Vec3d dir{0., 0., 1.};
  Vec3d up{0., 1., 0.};
  double fov = 0.5;
  double dist = 1.;

  try {
    for_all_lines_with_error(file_name, [&](std::string_view line) {
      if (remove_prefix(line, "pos ")) {
        pos = parse_point(line);
      } else if (remove_prefix(line, "dir ")) {
        dir = parse_vec(line);
      } else if (remove_prefix(line, "up ")) {
        up = parse_vec(line);
      } else if (remove_prefix(line, "dist ")) {
        dist = parse_double(line);
      } else if (remove_prefix(line, "fov ")) {
        fov = parse_double(line);
      }
    });
  } catch (const std::exception& err) {
    // Cannot open root file
    throw ParsingError{file_name, err.what(), 0};
  }
  return {pos, dir, up, fov, dist, width, height};
}

} // namespace parsing
