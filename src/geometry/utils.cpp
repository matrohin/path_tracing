#include "utils.h"

#include <cmath>

namespace {
const double pi = acos(0.) * 2.;

Matrix3d matrix_from_normal(const Vec3d& normal) {
  const auto h = abs(normal.x) < 0.99 ? Vec3d{1., 0., 0.} : Vec3d{0., 1., 0.};
  const auto tangent = (normal * h).normalized();
  const auto binormal = (normal * tangent).normalized();
  return Matrix3d{tangent, binormal, normal};
}

} // unnamed namespace

Vec3d generate_random_vec_on_hemisphere(const Vec3d& normal, double alpha,
                                        std::minstd_rand& engine) {
  std::uniform_real_distribution<> d;
  double cos_theta = pow(d(engine), 1. / (alpha + 1.));
  double sin_theta = sqrt(1. - cos_theta * cos_theta);
  double phi = 2 * pi * d(engine);
  Vec3d res{cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta};

  return matrix_from_normal(normal) * res;
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
