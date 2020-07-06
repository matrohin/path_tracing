#pragma once

#include "vec3d.h"

struct Matrix3d {
  Vec3d rows[3];

  Matrix3d(const Vec3d& i, const Vec3d& j, const Vec3d& k);
};

Vec3d operator*(const Matrix3d& m, const Vec3d& v);

inline Matrix3d::Matrix3d(const Vec3d& i, const Vec3d& j, const Vec3d& k)
    : rows{{i.x, j.x, k.x}, {i.y, j.y, k.y}, {i.z, j.z, k.z}} {}

inline Vec3d operator*(const Matrix3d& m, const Vec3d& v) {
  return {m.rows[0] % v, m.rows[1] % v, m.rows[2] % v};
}
