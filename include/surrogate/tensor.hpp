#pragma once

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
#include <array>
#include <cstddef>
#include <span>
#include <utility>

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
#include <algorithm>
#include <array>
#include <cstddef>
#include <initializer_list>
#include <span>
#include <stdexcept>
#include <utility>

#include "surrogate/aligned_alloc.hpp"
#include "surrogate/tensor_view.hpp"

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
namespace sur {

template <class T = float>
class Tensor {
 public:
  using value_type = T;
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

  Tensor() = default;
  Tensor(T* data, std::size_t size, std::array<int, 4> shape, std::array<int, 4> strides, bool row_major = true)
      : data_(data), size_(size), shape_(shape), strides_(strides), offset_(0), row_major_(row_major) {}
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
  using pointer = T*;
  using const_pointer = const T*;

  Tensor() = default;
  explicit Tensor(std::span<const int> dims, bool row_major = true) : row_major_(row_major) {
    if (dims.empty() || dims.size() > 4) {
      throw std::invalid_argument("Tensor expects 1-4 dimensions");
    }
    shape_ = normalise_dims(dims);
    compute_strides();
    size_ = compute_size();
    allocate();
  }

  explicit Tensor(const std::array<int, 4>& dims, bool row_major = true)
      : Tensor(std::span<const int>(dims.data(), dims.size()), row_major) {}

  Tensor(std::initializer_list<int> dims, bool row_major = true)
      : Tensor(std::span<const int>(dims.begin(), dims.size()), row_major) {}
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

  Tensor(const Tensor&) = delete;
  Tensor& operator=(const Tensor&) = delete;
  Tensor(Tensor&& other) noexcept
      : data_(std::exchange(other.data_, nullptr)),
        size_(std::exchange(other.size_, 0)),
        shape_(other.shape_),
        strides_(other.strides_),
        offset_(std::exchange(other.offset_, 0)),
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
        row_major_(other.row_major_) {}

  Tensor& operator=(Tensor&& other) noexcept {
    if (this != &other) {
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
        row_major_(other.row_major_),
        owning_(std::exchange(other.owning_, false)) {}

  Tensor& operator=(Tensor&& other) noexcept {
    if (this != &other) {
      release();
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
      data_ = std::exchange(other.data_, nullptr);
      size_ = std::exchange(other.size_, 0);
      shape_ = other.shape_;
      strides_ = other.strides_;
      offset_ = std::exchange(other.offset_, 0);
      row_major_ = other.row_major_;
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
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
=======
      owning_ = std::exchange(other.owning_, false);
>>>>>>> theirs
    }
    return *this;
  }

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
  ~Tensor() { release(); }

  [[nodiscard]] pointer data() noexcept { return data_; }
  [[nodiscard]] const_pointer data() const noexcept { return data_; }
  [[nodiscard]] std::size_t size() const noexcept { return size_; }
  [[nodiscard]] const std::array<int, 4>& shape() const noexcept { return shape_; }
  [[nodiscard]] const std::array<int, 4>& strides() const noexcept { return strides_; }
  [[nodiscard]] std::size_t offset() const noexcept { return offset_; }
  [[nodiscard]] bool row_major() const noexcept { return row_major_; }

  void fill(const T& value) {
    if (!data_) {
      return;
    }
    if (is_contiguous()) {
      std::fill_n(data_ + offset_, size_, value);
    } else {
      iterate_elements([&](pointer elem) { *elem = value; });
    }
  }

  void zero_() { fill(T{}); }

  Tensor as_contiguous() const {
    Tensor result(shape_, true);
    if (!data_) {
      return result;
    }
    if (is_contiguous()) {
      std::copy_n(data_ + offset_, size_, result.data_);
    } else {
      auto dst = result.data_;
      std::size_t index = 0;
      iterate_elements([&](const_pointer elem) mutable {
        dst[index++] = *elem;
      });
    }
    return result;
  }

  void reshape(std::span<const int> dims) {
    if (!is_contiguous()) {
      throw std::logic_error("Reshape requires contiguous tensor");
    }
    if (dims.empty() || dims.size() > 4) {
      throw std::invalid_argument("Tensor expects 1-4 dimensions");
    }
    auto new_shape = normalise_dims(dims);
    std::size_t new_size = 1;
    for (int dim : new_shape) {
      new_size *= static_cast<std::size_t>(dim);
    }
    if (new_size != size_) {
      throw std::invalid_argument("Reshape must preserve total element count");
    }
    shape_ = new_shape;
    compute_strides();
  }

  void reshape(std::initializer_list<int> dims) { reshape(std::span<const int>(dims.begin(), dims.size())); }

  TensorView<T> slice(const std::array<int, 4>& begin, const std::array<int, 4>& end) {
    validate_slice(begin, end);
    return make_slice_view<T>(begin, end);
  }

  TensorView<const T> slice(const std::array<int, 4>& begin, const std::array<int, 4>& end) const {
    validate_slice(begin, end);
    return make_slice_view<const T>(begin, end);
  }

  TensorView<T> view() noexcept { return TensorView<T>(data_, shape_, strides_, offset_, row_major_); }
  TensorView<const T> view() const noexcept { return TensorView<const T>(data_, shape_, strides_, offset_, row_major_); }

  [[nodiscard]] bool is_contiguous() const noexcept {
    if (!row_major_ || offset_ != 0) {
      return false;
    }
    int expected = 1;
    for (int i = 3; i >= 0; --i) {
      if (shape_[i] == 0) {
        return size_ == 0;
      }
      if (shape_[i] == 1) {
        continue;
      }
      if (strides_[i] != expected) {
        return false;
      }
      expected *= shape_[i];
    }
    return true;
  }

 private:
  template <class Func>
  void iterate_elements(Func&& func) const {
    std::array<int, 4> idx{0, 0, 0, 0};
    recurse_iterate<0>(idx, std::forward<Func>(func));
  }

  template <std::size_t Dim, class Func>
  void recurse_iterate(std::array<int, 4>& idx, Func&& func) const {
    if constexpr (Dim == 4) {
      std::size_t offset = offset_;
      for (std::size_t i = 0; i < 4; ++i) {
        offset += static_cast<std::size_t>(idx[i]) * static_cast<std::size_t>(strides_[i]);
      }
      func(data_ + offset);
    } else {
      for (int i = 0; i < shape_[Dim]; ++i) {
        idx[Dim] = i;
        recurse_iterate<Dim + 1>(idx, std::forward<Func>(func));
      }
    }
  }

  static std::array<int, 4> normalise_dims(std::span<const int> dims) {
    std::array<int, 4> shape{1, 1, 1, 1};
    for (std::size_t i = 0; i < dims.size(); ++i) {
      if (dims[i] < 0) {
        throw std::invalid_argument("Tensor dimensions must be non-negative");
      }
      shape[i] = dims[i];
    }
    return shape;
  }

  void compute_strides() {
    if (!row_major_) {
      throw std::logic_error("Only row-major tensors are supported");
    }
    strides_.fill(0);
    int stride = 1;
    for (int i = 3; i >= 0; --i) {
      strides_[i] = stride;
      stride *= shape_[i];
    }
  }

  std::size_t compute_size() const noexcept {
    std::size_t total = 1;
    for (int dim : shape_) {
      total *= static_cast<std::size_t>(dim);
    }
    return total;
  }

  void allocate() {
    if (size_ == 0) {
      data_ = nullptr;
      owning_ = false;
      return;
    }
    data_ = static_cast<pointer>(aligned_malloc(sizeof(T) * size_));
    owning_ = true;
  }

  void release() noexcept {
    if (owning_ && data_) {
      aligned_free(data_);
    }
    data_ = nullptr;
    size_ = 0;
    offset_ = 0;
    owning_ = false;
  }

  void validate_slice(const std::array<int, 4>& begin, const std::array<int, 4>& end) const {
    for (std::size_t i = 0; i < 4; ++i) {
      if (begin[i] < 0 || end[i] < begin[i] || end[i] > shape_[i]) {
        throw std::out_of_range("Invalid slice bounds");
      }
    }
  }

  template <class ViewT>
  TensorView<ViewT> make_slice_view(const std::array<int, 4>& begin, const std::array<int, 4>& end) const {
    std::array<int, 4> new_shape{};
    std::size_t new_offset = offset_;
    for (std::size_t i = 0; i < 4; ++i) {
      new_shape[i] = end[i] - begin[i];
      new_offset += static_cast<std::size_t>(begin[i]) * static_cast<std::size_t>(strides_[i]);
    }
    return TensorView<ViewT>(data_, new_shape, strides_, new_offset, row_major_);
  }

  pointer data_ = nullptr;
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
  std::size_t size_ = 0;
  std::array<int, 4> shape_{};
  std::array<int, 4> strides_{};
  std::size_t offset_ = 0;
  bool row_major_ = true;
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
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
=======
  bool owning_ = false;
>>>>>>> theirs
};

}  // namespace sur

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
