#pragma once

#include <array>
#include <cstddef>

namespace sur {

template <class T>
class TensorView {
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours

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
  using reference = T&;

  TensorView() = default;
  TensorView(pointer data,
             std::array<int, 4> shape,
             std::array<int, 4> strides,
             std::size_t offset = 0,
             bool row_major = true) noexcept
      : data_(data), shape_(shape), strides_(strides), offset_(offset), row_major_(row_major) {}

  [[nodiscard]] pointer data() noexcept { return data_; }
  [[nodiscard]] pointer data() const noexcept { return data_; }
  [[nodiscard]] const std::array<int, 4>& shape() const noexcept { return shape_; }
  [[nodiscard]] const std::array<int, 4>& strides() const noexcept { return strides_; }
  [[nodiscard]] std::size_t offset() const noexcept { return offset_; }
  [[nodiscard]] bool row_major() const noexcept { return row_major_; }

  [[nodiscard]] std::size_t size() const noexcept {
    std::size_t total = 1;
    for (int dim : shape_) {
      total *= static_cast<std::size_t>(dim);
    }
    return total;
  }

  template <class... Index>
  [[nodiscard]] reference operator()(Index... indices) const noexcept {
    static_assert(sizeof...(Index) <= 4, "TensorView supports up to 4 indices");
    const std::array<int, 4> idx = pack_indices(indices...);
    std::size_t offset = offset_;
    for (std::size_t i = 0; i < 4; ++i) {
      offset += static_cast<std::size_t>(idx[i]) * static_cast<std::size_t>(strides_[i]);
    }
    return data_[offset];
  }

 private:
  template <class... Index>
  [[nodiscard]] static std::array<int, 4> pack_indices(Index... indices) noexcept {
    std::array<int, 4> result{0, 0, 0, 0};
    int values[] = {static_cast<int>(indices)...};
    for (std::size_t i = 0; i < sizeof...(Index); ++i) {
      result[i] = values[i];
    }
    return result;
  }

  pointer data_ = nullptr;
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
};

}  // namespace sur

