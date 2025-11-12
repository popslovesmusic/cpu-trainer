#include "surrogate/layers.hpp"

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
namespace sur {

=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
#include <stdexcept>

namespace sur {

Tensor<float> ReLU::forward(const Tensor<float>& x) {
  const auto& shape = x.shape();
  Tensor<float> y({shape[0], shape[1]});
  const int total = shape[0] * shape[1];
  kernels::relu(y.data(), x.data(), total);
  last_output_ = TensorView<const float>(y.data(), y.shape(), y.strides(), y.offset(), y.row_major());
  last_shape_ = shape;
  has_cache_ = true;
  return y;
}

Tensor<float> ReLU::backward(const Tensor<float>& dy) {
  if (!has_cache_) {
    throw std::logic_error("ReLU::backward called before forward");
  }
  const auto& shape = dy.shape();
  if (shape != last_shape_) {
    throw std::invalid_argument("ReLU::backward gradient shape mismatch");
  }
  Tensor<float> dx({shape[0], shape[1]});
  const int total = shape[0] * shape[1];
  kernels::relu_bw(dx.data(), dy.data(), last_output_.data(), total);
  has_cache_ = false;
  return dx;
}

Tensor<float> Tanh::forward(const Tensor<float>& x) {
  const auto& shape = x.shape();
  Tensor<float> y({shape[0], shape[1]});
  const int total = shape[0] * shape[1];
  kernels::tanh(y.data(), x.data(), total);
  last_output_ = TensorView<const float>(y.data(), y.shape(), y.strides(), y.offset(), y.row_major());
  last_shape_ = shape;
  has_cache_ = true;
  return y;
}

Tensor<float> Tanh::backward(const Tensor<float>& dy) {
  if (!has_cache_) {
    throw std::logic_error("Tanh::backward called before forward");
  }
  const auto& shape = dy.shape();
  if (shape != last_shape_) {
    throw std::invalid_argument("Tanh::backward gradient shape mismatch");
  }
  Tensor<float> dx({shape[0], shape[1]});
  const int total = shape[0] * shape[1];
  kernels::tanh_bw(dx.data(), dy.data(), last_output_.data(), total);
  has_cache_ = false;
  return dx;
}

Tensor<float> Sigmoid::forward(const Tensor<float>& x) {
  const auto& shape = x.shape();
  Tensor<float> y({shape[0], shape[1]});
  const int total = shape[0] * shape[1];
  kernels::sigmoid(y.data(), x.data(), total);
  last_output_ = TensorView<const float>(y.data(), y.shape(), y.strides(), y.offset(), y.row_major());
  last_shape_ = shape;
  has_cache_ = true;
  return y;
}

Tensor<float> Sigmoid::backward(const Tensor<float>& dy) {
  if (!has_cache_) {
    throw std::logic_error("Sigmoid::backward called before forward");
  }
  const auto& shape = dy.shape();
  if (shape != last_shape_) {
    throw std::invalid_argument("Sigmoid::backward gradient shape mismatch");
  }
  Tensor<float> dx({shape[0], shape[1]});
  const int total = shape[0] * shape[1];
  kernels::sigmoid_bw(dx.data(), dy.data(), last_output_.data(), total);
  has_cache_ = false;
  return dx;
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
}  // namespace sur

