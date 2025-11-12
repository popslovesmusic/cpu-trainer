#include "surrogate/aligned_alloc.hpp"

#include <cstdlib>
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs
=======
#include <new>
>>>>>>> theirs

#if defined(_MSC_VER)
#include <malloc.h>
#endif

namespace sur {

void* aligned_malloc(std::size_t bytes, std::size_t alignment) {
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
#if defined(_MSC_VER)
  return _aligned_malloc(bytes, alignment);
#else
  (void)alignment;
  return std::malloc(bytes);
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
  if (bytes == 0) {
    return nullptr;
  }

#if defined(_MSC_VER)
  void* ptr = _aligned_malloc(bytes, alignment);
  if (!ptr) {
    throw std::bad_alloc();
  }
  return ptr;
#else
  if (alignment < alignof(void*)) {
    alignment = alignof(void*);
  }

  void* ptr = nullptr;
#if defined(_POSIX_VERSION)
  int rc = ::posix_memalign(&ptr, alignment, bytes);
  if (rc != 0) {
    ptr = nullptr;
  }
#else
  std::size_t size = bytes;
  const std::size_t remainder = size % alignment;
  if (remainder != 0) {
    size += alignment - remainder;
  }
  ptr = std::aligned_alloc(alignment, size);
#endif
  if (!ptr) {
    throw std::bad_alloc();
  }
  return ptr;
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
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

