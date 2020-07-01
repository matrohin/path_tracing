#pragma once

#include "point3d.h"

#include <random>

Vec3d generate_random_vec_on_hemisphere(const Vec3d& normal,
                                        std::minstd_rand& engine);

Vec3d reflect_vec(const Vec3d& vec, const Vec3d& normal);

Vec3d refract_vec(const Vec3d& from, const Vec3d& normal,
                  double refr_index_from, double refr_index_to);
