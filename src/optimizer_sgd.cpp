#include "surrogate/optimizer.hpp"

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
namespace sur {

template <class T>
class Tensor;

class SGD : public Optimizer {
 public:
  void step(const std::vector<Tensor<float>*>&, const std::vector<Tensor<float>*>&) override {}
  void zero_grad(const std::vector<Tensor<float>*>&) override {}
};
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

SGD::SGD(float lr, float momentum, float weight_decay)
    : lr_(lr), momentum_(momentum), weight_decay_(weight_decay) {
  if (!(lr_ > 0.0f)) {
    throw std::invalid_argument("Learning rate must be positive");
  }
  if (momentum_ < 0.0f || momentum_ >= 1.0f) {
    throw std::invalid_argument("Momentum must be in [0, 1)");
  }
  if (weight_decay_ < 0.0f) {
    throw std::invalid_argument("Weight decay must be non-negative");
  }
}

void SGD::set_lr(float lr) {
  if (!(lr > 0.0f)) {
    throw std::invalid_argument("Learning rate must be positive");
  }
  lr_ = lr;
}

void SGD::ensure_state(std::span<Tensor<float>* const> params) {
  bool rebuild = !state_ready_;
  if (!rebuild && params.size() != param_sizes_.size()) {
    rebuild = true;
  }

  if (!rebuild) {
    for (std::size_t i = 0; i < params.size(); ++i) {
      if (!params[i]) {
        throw std::invalid_argument("Null parameter tensor passed to SGD");
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
      throw std::invalid_argument("Null parameter tensor passed to SGD");
    }
    if (!tensor->is_contiguous()) {
      throw std::invalid_argument("SGD requires contiguous parameter tensors");
    }
    param_sizes_.push_back(tensor->size());
    offsets_[i] = total_size_;
    total_size_ += tensor->size();
  }

  velocity_.assign(total_size_, 0.0f);
  update_buffer_.assign(total_size_, 0.0f);
  state_ready_ = true;
}

void SGD::step(std::span<Tensor<float>* const> params,
               std::span<Tensor<float>* const> grads) {
  if (params.size() != grads.size()) {
    throw std::invalid_argument("SGD expects the same number of params and grads");
  }
  ensure_state(params);

  for (std::size_t i = 0; i < params.size(); ++i) {
    Tensor<float>* param = params[i];
    Tensor<float>* grad = grads[i];
    if (!param || !grad) {
      throw std::invalid_argument("Null tensor provided to SGD");
    }
    if (!grad->is_contiguous()) {
      throw std::invalid_argument("SGD requires contiguous gradient tensors");
    }
    if (param->size() != grad->size()) {
      throw std::invalid_argument("Parameter and gradient size mismatch");
    }

    float* param_data = param->data();
    const float* grad_data = grad->data();
    const std::size_t tensor_size = param->size();
    float* update = update_buffer_.data() + offsets_[i];

    for (std::size_t j = 0; j < tensor_size; ++j) {
      float value = grad_data[j];
      if (weight_decay_ != 0.0f) {
        value += weight_decay_ * param_data[j];
      }
      update[j] = value;
    }

    if (momentum_ != 0.0f) {
      float* velocity = velocity_.data() + offsets_[i];
      for (std::size_t j = 0; j < tensor_size; ++j) {
        velocity[j] = momentum_ * velocity[j] + update[j];
        update[j] = velocity[j];
      }
    }

    kernels::axpy(-lr_, update, param_data, narrow_size(tensor_size));
  }
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

}  // namespace sur

