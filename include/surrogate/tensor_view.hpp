#pragma once

#include <array>
#include <cstddef>

namespace sur {

template <class T>
class TensorView {
 public:
  using value_type = T;

  TensorView() = default;
  TensorView(T* data, std::array<int, 4> shape, std::array<int, 4> strides, std::size_t offset = 0) noexcept
      : data_(data), shape_(shape), strides_(strides), offset_(offset) {}

  [[nodiscard]] T* data() noexcept { return data_; }
  [[nodiscard]] const T* data() const noexcept { return data_; }
  [[nodiscard]] const std::array<int, 4>& shape() const noexcept { return shape_; }
  [[nodiscard]] const std::array<int, 4>& strides() const noexcept { return strides_; }

 private:
  T* data_ = nullptr;
  std::array<int, 4> shape_{};
  std::array<int, 4> strides_{};
  std::size_t offset_ = 0;
};

}  // namespace sur

