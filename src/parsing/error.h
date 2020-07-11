#pragma once

#include <string>

namespace parsing {

struct ParsingError {
  std::string file_name;
  std::string reason;
  size_t line;
};

} // namespace parsing
