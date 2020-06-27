#pragma once

#include "view/color.h"
#include "view/view.h"

namespace png_utils {

bool write_png(const char* file_name, const View& image);

}  // namespace png_utils
