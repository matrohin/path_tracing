#pragma once

#include "point3d.h"

#include <random>

Vec3d generate_random_vec_on_hemisphere(const Vec3d& normal,
                                        std::mt19937& engine);
