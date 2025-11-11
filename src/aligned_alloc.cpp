#include "surrogate/aligned_alloc.hpp"

#include <cstdlib>

#if defined(_MSC_VER)
#include <malloc.h>
#endif

namespace sur {

void* aligned_malloc(std::size_t bytes, std::size_t alignment) {
#if defined(_MSC_VER)
  return _aligned_malloc(bytes, alignment);
#else
  (void)alignment;
  return std::malloc(bytes);
#endif
}

void aligned_free(void* ptr) noexcept {
#if defined(_MSC_VER)
  if (ptr) {
    _aligned_free(ptr);
  }
#else
  std::free(ptr);
#endif
}

}  // namespace sur

