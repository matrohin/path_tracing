#pragma once

struct Vec3d {
  double x = 0;
  double y = 0;
  double z = 0;

  double length() const;
  Vec3d normalized() const;
};

double operator%(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, double b);
Vec3d operator*(double a, const Vec3d& b);
Vec3d operator/(const Vec3d& a, double b);
Vec3d operator-(const Vec3d& a);
Vec3d operator+(const Vec3d& a, const Vec3d& b);
Vec3d operator-(const Vec3d& a, const Vec3d& b);
