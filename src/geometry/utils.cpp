#include "utils.h"

Vec3d generate_random_vec_on_hemisphere(const Vec3d& normal,
                                        std::minstd_rand& engine) {
  std::normal_distribution<> d;
  Vec3d res{d(engine), d(engine), d(engine)};
  const auto t = normal % res;
  if (t < 0) return -res;
  return res;
}

// http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
Vec3d reflect_vec(const Vec3d& vec, const Vec3d& normal) {
  return vec - 2 * (vec % normal) * normal;
}

Vec3d refract_vec(const Vec3d& from, const Vec3d& normal,
                  double refr_index_from, double refr_index_to) {
  const auto refr_ratio = refr_index_from / refr_index_to;
  const auto cos_from_angle = from % normal;
  const auto cos_from_angle_sqr = cos_from_angle * cos_from_angle;
  const auto sin_from_angle_sqr = 1 - cos_from_angle_sqr;

  // Total  internal  reflection
  if (sin_from_angle_sqr > 1. / refr_ratio) return reflect_vec(from, normal);

  const auto sin_to_angle_sqr = refr_ratio * sin_from_angle_sqr;
  return refr_ratio * from +
         (refr_ratio * cos_from_angle - sqrt(1 - sin_to_angle_sqr)) * normal;
}
