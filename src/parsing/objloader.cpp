#include "objloader.h"

#include "error.h"
#include "parsing_utils.h"
#include "rendering/scene.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <string>
#include <string_view>

namespace parsing {

namespace {

std::map<std::string, size_t, std::less<>> load_mtl_file(const char* file_name,
                                                         Scene& scene) {
  std::map<std::string, size_t, std::less<>> res;
  Material* cur_mat = nullptr;

  for_all_lines_with_error(file_name, [&](std::string_view line) {
    if (remove_prefix(line, "newmtl ")) {
      res.emplace(line, scene.add_material({}));
      cur_mat = &scene.materials.back();
    } else if (remove_prefix(line, "Kd ")) {
      if (!cur_mat) throw std::runtime_error{"Kd is used before newmtl"};
      cur_mat->diffuse = parse_color(line);
    } else if (remove_prefix(line, "Ke ")) {
      if (!cur_mat) throw std::runtime_error{"Ke is used before newmtl"};
      cur_mat->emmitance = parse_color(line);
    }
  });
  return res;
}

struct ObjFileState {
  std::map<std::string, size_t, std::less<>> material_indexes;
  size_t cur_mat_index = std::string::npos;
  std::vector<Point3d> points;

  Point3d get_point(int idx) {
    if (idx < 0) {
      idx += static_cast<int>(points.size());
    } else {
      idx -= 1;
    }
    return points.at(idx);
  }

  void on_mtllib(std::string_view s, const char* file_name, Scene& scene) {
    const auto p = std::filesystem::path(file_name).replace_filename(s);
    auto other = load_mtl_file(p.string().c_str(), scene);
    material_indexes.merge(other);
  }

  void on_usemtl(std::string_view s) {
    const auto it = material_indexes.find(s);
    if (it == material_indexes.end())
      throw std::runtime_error{"Material "s + s + "is not found"};
    cur_mat_index = it->second;
  }

  void on_vertex(std::string_view s) { points.emplace_back(parse_point(s)); }

  void on_polygon(std::string_view s, Scene& scene) {
    if (cur_mat_index == std::string::npos)
      throw std::runtime_error{"Current material is not set"};
    const auto start = get_point(parse_int(s));
    auto a = get_point(parse_int(s));
    while (!s.empty()) {
      auto b = get_point(parse_int(s));
      scene.add_triangle(Triangle{start, a, b}, cur_mat_index);
      a = b;
    }
  }
};

} // unnamed namespace

Scene load_obj_file(const char* file_name) {
  Scene scene;
  ObjFileState state;
  try {
    for_all_lines_with_error(file_name, [&](std::string_view line) {
      if (remove_prefix(line, "mtllib ")) {
        state.on_mtllib(line, file_name, scene);
      } else if (remove_prefix(line, "usemtl ")) {
        state.on_usemtl(line);
      } else if (remove_prefix(line, "v ")) {
        state.on_vertex(line);
      } else if (remove_prefix(line, "f ")) {
        state.on_polygon(line, scene);
      }
    });
  } catch (const std::exception& err) {
    // Cannot open root file
    throw ParsingError{file_name, err.what(), 0};
  }

  return scene;
}

} // namespace parsing
