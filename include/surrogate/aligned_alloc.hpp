#pragma once

#include <cstddef>

namespace sur {

[[nodiscard]] void* aligned_malloc(std::size_t bytes, std::size_t alignment = 32);
void aligned_free(void* ptr) noexcept;

}  // namespace sur

