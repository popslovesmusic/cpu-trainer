<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
#include <cstdlib>

int main() { return EXIT_SUCCESS; }
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "surrogate/tensor.hpp"

namespace {

void expect(bool condition, const char* message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void test_alignment() {
  sur::Tensor<float> tensor({2, 3, 4, 5});
  const auto address = reinterpret_cast<std::uintptr_t>(tensor.data());
  expect(address % 32 == 0, "Tensor data is not 32-byte aligned");
  expect(tensor.size() == 2 * 3 * 4 * 5, "Tensor size mismatch");
  expect(tensor.strides()[0] == 60 && tensor.strides()[1] == 20 && tensor.strides()[2] == 5 && tensor.strides()[3] == 1,
         "Unexpected row-major strides");
}

void test_fill_and_zero() {
  sur::Tensor<float> tensor({4, 4});
  tensor.fill(3.5f);
  for (std::size_t i = 0; i < tensor.size(); ++i) {
    expect(tensor.data()[i] == 3.5f, "Fill() failed to assign value");
  }

  tensor.zero_();
  for (std::size_t i = 0; i < tensor.size(); ++i) {
    expect(tensor.data()[i] == 0.0f, "zero_() failed to reset element");
  }
}

void test_reshape() {
  sur::Tensor<float> tensor({2, 3, 4});
  tensor.reshape({6, 4});
  const auto& shape = tensor.shape();
  expect(shape[0] == 6 && shape[1] == 4 && shape[2] == 1 && shape[3] == 1, "reshape() produced incorrect shape");
}

void test_slice_and_view() {
  sur::Tensor<float> tensor({2, 3, 4, 5});
  for (std::size_t i = 0; i < tensor.size(); ++i) {
    tensor.data()[i] = static_cast<float>(i);
  }

  auto view = tensor.slice({1, 1, 0, 0}, {2, 3, 4, 5});
  expect(view.shape()[0] == 1 && view.shape()[1] == 2 && view.shape()[2] == 4 && view.shape()[3] == 5,
         "Slice shape mismatch");
  expect(view(0, 0, 0, 0) == tensor.view()(1, 1, 0, 0), "Slice origin mismatch");
  expect(view(0, 1, 3, 4) == tensor.view()(1, 2, 3, 4), "Slice indexing mismatch");
}

void test_as_contiguous() {
  sur::Tensor<float> tensor({2, 3, 4, 5});
  for (std::size_t i = 0; i < tensor.size(); ++i) {
    tensor.data()[i] = static_cast<float>(i * 2);
  }

  auto copy = tensor.as_contiguous();
  expect(copy.size() == tensor.size(), "as_contiguous() size mismatch");
  for (std::size_t i = 0; i < tensor.size(); ++i) {
    expect(copy.data()[i] == tensor.data()[i], "as_contiguous() failed to copy data");
  }
}

}  // namespace

int main() {
  try {
    test_alignment();
    test_fill_and_zero();
    test_reshape();
    test_slice_and_view();
    test_as_contiguous();
  } catch (const std::exception& e) {
    std::cerr << "Tensor tests failed: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
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

