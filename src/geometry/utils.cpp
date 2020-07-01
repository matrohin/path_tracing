#include "utils.h"

Vec3d generate_random_vec_on_hemisphere(const Vec3d& normal,
                                        std::minstd_rand& engine) {
  std::normal_distribution<> d;
  Vec3d res{d(engine), d(engine), d(engine)};
  const auto t = normal % res;
  if (t < 0) return -res;
  return res;
}

Vec3d reflect_vec(const Vec3d& vec, const Vec3d& normal) {
  // http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
  return vec - 2 * (vec % normal) * normal;
}
