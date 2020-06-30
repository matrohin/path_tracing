#include "utils.h"

Vec3d generate_random_vec_on_hemisphere(const Vec3d& normal,
                                        std::minstd_rand& engine) {
  std::normal_distribution<> d;
  Vec3d res{d(engine), d(engine), d(engine)};
  const auto t = normal % res;
  if (t < 0) return -res;
  return res;
}
