#include "surrogate/layers.hpp"

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
namespace sur {

std::vector<LayerPtr> create_default_layers() {
  return {};
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
#include <stdexcept>

namespace sur {
namespace {
constexpr std::array<int, 4> make_shape(int dim0, int dim1) {
  return {dim0, dim1, 1, 1};
}
}  // namespace

Dense::Dense(int in_features, int out_features, bool use_bias)
    : in_features_(in_features),
      out_features_(out_features),
      use_bias_(use_bias),
      weight_(make_shape(out_features, in_features)),
      bias_(make_shape(out_features, 1)),
      grad_weight_(make_shape(out_features, in_features)),
      grad_bias_(make_shape(out_features, 1)) {
  if (in_features_ <= 0 || out_features_ <= 0) {
    throw std::invalid_argument("Dense layer dimensions must be positive");
  }

  param_ptrs_[0] = &weight_;
  param_ptrs_[1] = use_bias_ ? &bias_ : nullptr;
  grad_ptrs_[0] = &grad_weight_;
  grad_ptrs_[1] = use_bias_ ? &grad_bias_ : nullptr;

  grad_weight_.zero_();
  if (use_bias_) {
    grad_bias_.zero_();
  }
}

void Dense::ensure_workspaces(int batch_size) {
  if (batch_size <= 0) {
    throw std::invalid_argument("Batch size must be positive");
  }
  const auto current_batch = input_transposed_.shape()[0];
  if (current_batch < batch_size) {
    input_transposed_ = Tensor<float>(make_shape(batch_size, in_features_));
  }
  const auto dy_batch = dy_transposed_.shape()[0];
  if (dy_batch < batch_size) {
    dy_transposed_ = Tensor<float>(make_shape(batch_size, out_features_));
  }
  const auto dx_batch = dx_transposed_.shape()[0];
  if (dx_batch < batch_size) {
    dx_transposed_ = Tensor<float>(make_shape(batch_size, in_features_));
  }
}

void Dense::transpose_input(const Tensor<float>& x, int batch_size) {
  const float* src = x.data();
  float* dst = input_transposed_.data();
  const int stride_src = batch_size;
  for (int k = 0; k < in_features_; ++k) {
    for (int n = 0; n < batch_size; ++n) {
      dst[n * in_features_ + k] = src[k * stride_src + n];
    }
  }
}

void Dense::transpose_output_grad(const Tensor<float>& dy, int batch_size) {
  const float* src = dy.data();
  float* dst = dy_transposed_.data();
  const int stride_src = batch_size;
  for (int m = 0; m < out_features_; ++m) {
    for (int n = 0; n < batch_size; ++n) {
      dst[n * out_features_ + m] = src[m * stride_src + n];
    }
  }
}

Tensor<float> Dense::forward(const Tensor<float>& x) {
  const auto& shape = x.shape();
  if (shape[0] != in_features_) {
    throw std::invalid_argument("Dense::forward input feature mismatch");
  }
  const int batch_size = shape[1];
  ensure_workspaces(batch_size);
  transpose_input(x, batch_size);

  Tensor<float> y(make_shape(out_features_, batch_size));
  kernels::gemm(out_features_, batch_size, in_features_,
                weight_.data(), in_features_,
                x.data(), batch_size,
                y.data(), batch_size,
                0.0f);

  if (use_bias_) {
    const float* bias_ptr = bias_.data();
    float* y_ptr = y.data();
    for (int m = 0; m < out_features_; ++m) {
      const float b = bias_ptr[m];
      float* row = y_ptr + m * batch_size;
      for (int n = 0; n < batch_size; ++n) {
        row[n] += b;
      }
    }
  }

  last_batch_size_ = batch_size;
  has_cache_ = true;
  return y;
}

Tensor<float> Dense::backward(const Tensor<float>& dy) {
  if (!has_cache_) {
    throw std::logic_error("Dense::backward called before forward");
  }
  const auto& shape = dy.shape();
  if (shape[0] != out_features_ || shape[1] != last_batch_size_) {
    throw std::invalid_argument("Dense::backward gradient shape mismatch");
  }
  const int batch_size = last_batch_size_;
  transpose_output_grad(dy, batch_size);

  grad_weight_.zero_();
  kernels::gemm(out_features_, in_features_, batch_size,
                dy.data(), batch_size,
                input_transposed_.data(), in_features_,
                grad_weight_.data(), in_features_,
                0.0f);

  if (use_bias_) {
    grad_bias_.zero_();
    float* db_ptr = grad_bias_.data();
    const float* dy_ptr = dy.data();
    for (int m = 0; m < out_features_; ++m) {
      float acc = 0.0f;
      const float* row = dy_ptr + m * batch_size;
      for (int n = 0; n < batch_size; ++n) {
        acc += row[n];
      }
      db_ptr[m] = acc;
    }
  }

  Tensor<float> dx(make_shape(in_features_, batch_size));
  kernels::gemm(batch_size, in_features_, out_features_,
                dy_transposed_.data(), out_features_,
                weight_.data(), in_features_,
                dx_transposed_.data(), in_features_,
                0.0f);

  float* dst = dx.data();
  const float* src = dx_transposed_.data();
  for (int k = 0; k < in_features_; ++k) {
    for (int n = 0; n < batch_size; ++n) {
      dst[k * batch_size + n] = src[n * in_features_ + k];
    }
  }

  has_cache_ = false;
  return dx;
}

std::span<Tensor<float>*> Dense::params() noexcept {
  if (use_bias_) {
    return std::span<Tensor<float>*>(param_ptrs_.data(), 2);
  }
  return std::span<Tensor<float>*>(param_ptrs_.data(), 1);
}

std::span<Tensor<float>*> Dense::grads() noexcept {
  if (use_bias_) {
    return std::span<Tensor<float>*>(grad_ptrs_.data(), 2);
  }
  return std::span<Tensor<float>*>(grad_ptrs_.data(), 1);
}

void Dense::reserve_workspaces(int max_batch) {
  if (max_batch <= 0) {
    throw std::invalid_argument("reserve_workspaces expects positive batch");
  }
  ensure_workspaces(max_batch);
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
}

}  // namespace sur

