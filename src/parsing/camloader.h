#pragma once

#include <cstdint>

struct Camera;

namespace parsing {

// Loads .cam file with Camera parameters
Camera load_cam_file(const char* file_name, uint32_t width, uint32_t height);

} // namespace parsing
