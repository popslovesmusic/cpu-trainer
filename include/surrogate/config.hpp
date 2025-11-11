#pragma once

#include <cstddef>
#include <string>

namespace sur {

struct RuntimeConfig {
  std::string preset;
  int threads = 1;
  bool enable_avx2 = false;
};

}  // namespace sur

