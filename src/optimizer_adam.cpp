#include "surrogate/optimizer.hpp"

<<<<<<< ours
namespace sur {

template <class T>
class Tensor;

class Adam : public Optimizer {
 public:
  void step(const std::vector<Tensor<float>*>&, const std::vector<Tensor<float>*>&) override {}
  void zero_grad(const std::vector<Tensor<float>*>&) override {}
};
=======
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>

#include "surrogate/mathkernels.hpp"
#include "surrogate/tensor.hpp"

namespace sur {

namespace {

int narrow_size(std::size_t size) {
  if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
    throw std::overflow_error("Tensor size exceeds supported range");
  }
  return static_cast<int>(size);
}

}  // namespace

Adam::Adam(float lr, float beta1, float beta2, float eps)
    : lr_(lr), beta1_(beta1), beta2_(beta2), eps_(eps) {
  if (!(lr_ > 0.0f)) {
    throw std::invalid_argument("Learning rate must be positive");
  }
  if (beta1_ <= 0.0f || beta1_ >= 1.0f) {
    throw std::invalid_argument("Beta1 must be in (0, 1)");
  }
  if (beta2_ <= 0.0f || beta2_ >= 1.0f) {
    throw std::invalid_argument("Beta2 must be in (0, 1)");
  }
  if (!(eps_ > 0.0f)) {
    throw std::invalid_argument("Epsilon must be positive");
  }
}

void Adam::set_lr(float lr) {
  if (!(lr > 0.0f)) {
    throw std::invalid_argument("Learning rate must be positive");
  }
  lr_ = lr;
}

void Adam::reset_bias_correction() {
  step_ = 0;
  beta1_pow_ = 1.0f;
  beta2_pow_ = 1.0f;
}

void Adam::ensure_state(std::span<Tensor<float>* const> params) {
  bool rebuild = !state_ready_;
  if (!rebuild && params.size() != param_sizes_.size()) {
    rebuild = true;
  }

  if (!rebuild) {
    for (std::size_t i = 0; i < params.size(); ++i) {
      if (!params[i]) {
        throw std::invalid_argument("Null parameter tensor passed to Adam");
      }
      if (params[i]->size() != param_sizes_[i]) {
        rebuild = true;
        break;
      }
    }
  }

  if (!rebuild) {
    return;
  }

  param_sizes_.clear();
  offsets_.assign(params.size(), 0);
  total_size_ = 0;

  for (std::size_t i = 0; i < params.size(); ++i) {
    Tensor<float>* tensor = params[i];
    if (!tensor) {
      throw std::invalid_argument("Null parameter tensor passed to Adam");
    }
    if (!tensor->is_contiguous()) {
      throw std::invalid_argument("Adam requires contiguous parameter tensors");
    }
    param_sizes_.push_back(tensor->size());
    offsets_[i] = total_size_;
    total_size_ += tensor->size();
  }

  m_.assign(total_size_, 0.0f);
  v_.assign(total_size_, 0.0f);
  update_buffer_.assign(total_size_, 0.0f);
  reset_bias_correction();
  state_ready_ = true;
}

void Adam::step(std::span<Tensor<float>* const> params,
                std::span<Tensor<float>* const> grads) {
  if (params.size() != grads.size()) {
    throw std::invalid_argument("Adam expects the same number of params and grads");
  }

  ensure_state(params);

  ++step_;
  beta1_pow_ *= beta1_;
  beta2_pow_ *= beta2_;
  const float bias_correction1 = 1.0f - beta1_pow_;
  const float bias_correction2 = 1.0f - beta2_pow_;

  for (std::size_t i = 0; i < params.size(); ++i) {
    Tensor<float>* param = params[i];
    Tensor<float>* grad = grads[i];
    if (!param || !grad) {
      throw std::invalid_argument("Null tensor provided to Adam");
    }
    if (!grad->is_contiguous()) {
      throw std::invalid_argument("Adam requires contiguous gradient tensors");
    }
    if (param->size() != grad->size()) {
      throw std::invalid_argument("Parameter and gradient size mismatch");
    }

    float* param_data = param->data();
    const float* grad_data = grad->data();
    const std::size_t tensor_size = param->size();
    float* m_state = m_.data() + offsets_[i];
    float* v_state = v_.data() + offsets_[i];
    float* update = update_buffer_.data() + offsets_[i];

    for (std::size_t j = 0; j < tensor_size; ++j) {
      const float g = grad_data[j];
      m_state[j] = beta1_ * m_state[j] + (1.0f - beta1_) * g;
      v_state[j] = beta2_ * v_state[j] + (1.0f - beta2_) * g * g;

      const float m_hat = m_state[j] / bias_correction1;
      const float v_hat = v_state[j] / bias_correction2;
      update[j] = m_hat / (std::sqrt(v_hat) + eps_);
    }

    kernels::axpy(-lr_, update, param_data, narrow_size(tensor_size));
  }
}
>>>>>>> theirs

}  // namespace sur

