#pragma once

struct Scene;

namespace parsing {

// Loads wavefront .obj file with 3d scene
Scene load_obj_file(const char* file_name);

} // namespace parsing
