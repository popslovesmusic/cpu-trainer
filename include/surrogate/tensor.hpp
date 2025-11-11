#pragma once

#include <array>
#include <cstddef>
#include <span>
#include <utility>

namespace sur {

template <class T = float>
class Tensor {
 public:
  using value_type = T;

  Tensor() = default;
  Tensor(T* data, std::size_t size, std::array<int, 4> shape, std::array<int, 4> strides, bool row_major = true)
      : data_(data), size_(size), shape_(shape), strides_(strides), offset_(0), row_major_(row_major) {}

  Tensor(const Tensor&) = delete;
  Tensor& operator=(const Tensor&) = delete;
  Tensor(Tensor&& other) noexcept
      : data_(std::exchange(other.data_, nullptr)),
        size_(std::exchange(other.size_, 0)),
        shape_(other.shape_),
        strides_(other.strides_),
        offset_(std::exchange(other.offset_, 0)),
        row_major_(other.row_major_) {}

  Tensor& operator=(Tensor&& other) noexcept {
    if (this != &other) {
      data_ = std::exchange(other.data_, nullptr);
      size_ = std::exchange(other.size_, 0);
      shape_ = other.shape_;
      strides_ = other.strides_;
      offset_ = std::exchange(other.offset_, 0);
      row_major_ = other.row_major_;
    }
    return *this;
  }

  ~Tensor() = default;

  [[nodiscard]] T* data() noexcept { return data_; }
  [[nodiscard]] const T* data() const noexcept { return data_; }
  [[nodiscard]] std::size_t size() const noexcept { return size_; }
  [[nodiscard]] const std::array<int, 4>& shape() const noexcept { return shape_; }
  [[nodiscard]] const std::array<int, 4>& strides() const noexcept { return strides_; }

  void fill(const T& value) {
    for (std::size_t i = 0; i < size_; ++i) {
      data_[i] = value;
    }
  }

  void zero() {
    fill(T{});
  }

 private:
  T* data_ = nullptr;
  std::size_t size_ = 0;
  std::array<int, 4> shape_{};
  std::array<int, 4> strides_{};
  std::size_t offset_ = 0;
  bool row_major_ = true;
};

}  // namespace sur

#include "surrogate/tensor_view.hpp"

namespace sur {

template <class T>
TensorView<T> make_view(Tensor<T>& tensor) {
  return TensorView<T>(tensor.data(), tensor.shape(), tensor.strides());
}

template <class T>
TensorView<const T> make_view(const Tensor<T>& tensor) {
  return TensorView<const T>(tensor.data(), tensor.shape(), tensor.strides());
}

}  // namespace sur

