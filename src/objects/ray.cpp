#include "ray.h"

Point3d Ray::at(double t) const {
    return start + (direction * t);
}
