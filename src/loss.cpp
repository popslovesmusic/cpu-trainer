#include "surrogate/loss.hpp"

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
namespace sur {

Loss* create_mse_loss() {
  return nullptr;
}

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
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "surrogate/mathkernels.hpp"
#include "surrogate/tensor.hpp"

namespace sur {
namespace {

void ensure_compatible(const Tensor<float>& prediction, const Tensor<float>& target) {
  if (prediction.shape() != target.shape()) {
    throw std::invalid_argument("Loss tensors must share the same shape");
  }
  if (prediction.size() != target.size()) {
    throw std::invalid_argument("Loss tensors must share the same element count");
  }
}

int checked_size(std::size_t size) {
  if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
    throw std::overflow_error("Tensor too large for loss kernel dispatch");
  }
  return static_cast<int>(size);
}

struct ContiguousTensor {
  Tensor<float> owned;
  const float* data = nullptr;
};

ContiguousTensor make_contiguous(const Tensor<float>& tensor) {
  if (tensor.is_contiguous()) {
    return {Tensor<float>(), tensor.data() + tensor.offset()};
  }
  Tensor<float> copy = tensor.as_contiguous();
  return {std::move(copy), copy.data()};
}

Tensor<float> difference(const float* lhs, const float* rhs, int count, const std::array<int, 4>& shape) {
  Tensor<float> diff(shape);
  std::copy_n(lhs, count, diff.data());
  kernels::axpy(-1.0f, rhs, diff.data(), count);
  return diff;
}

}  // namespace

float MSE::compute(const Tensor<float>& prediction, const Tensor<float>& target) const {
  ensure_compatible(prediction, target);
  const int count = checked_size(prediction.size());

  auto pred_contig = make_contiguous(prediction);
  auto tgt_contig = make_contiguous(target);

  Tensor<float> diff = difference(pred_contig.data, tgt_contig.data, count, prediction.shape());
  const float sum_sq = kernels::dot(diff.data(), diff.data(), count);
  return count == 0 ? 0.0f : sum_sq / static_cast<float>(count);
}

Tensor<float> MSE::gradient(const Tensor<float>& prediction, const Tensor<float>& target) const {
  ensure_compatible(prediction, target);
  const int count = checked_size(prediction.size());

  auto pred_contig = make_contiguous(prediction);
  auto tgt_contig = make_contiguous(target);

  Tensor<float> grad = difference(pred_contig.data, tgt_contig.data, count, prediction.shape());
  if (count == 0) {
    return grad;
  }
  const float scale = 2.0f / static_cast<float>(count);
  float* data = grad.data();
  for (int i = 0; i < count; ++i) {
    data[i] *= scale;
  }
  return grad;
}

float MAE::compute(const Tensor<float>& prediction, const Tensor<float>& target) const {
  ensure_compatible(prediction, target);
  const int count = checked_size(prediction.size());

  auto pred_contig = make_contiguous(prediction);
  auto tgt_contig = make_contiguous(target);

  Tensor<float> diff = difference(pred_contig.data, tgt_contig.data, count, prediction.shape());
  double sum = 0.0;
  for (int i = 0; i < count; ++i) {
    sum += std::fabs(diff.data()[i]);
  }
  return count == 0 ? 0.0f : static_cast<float>(sum / static_cast<double>(count));
}

Tensor<float> MAE::gradient(const Tensor<float>& prediction, const Tensor<float>& target) const {
  ensure_compatible(prediction, target);
  const int count = checked_size(prediction.size());

  auto pred_contig = make_contiguous(prediction);
  auto tgt_contig = make_contiguous(target);

  Tensor<float> grad = difference(pred_contig.data, tgt_contig.data, count, prediction.shape());
  if (count == 0) {
    return grad;
  }
  const float inv_count = 1.0f / static_cast<float>(count);
  float* data = grad.data();
  for (int i = 0; i < count; ++i) {
    const float v = data[i];
    if (v > 0.0f) {
      data[i] = inv_count;
    } else if (v < 0.0f) {
      data[i] = -inv_count;
    } else {
      data[i] = 0.0f;
    }
  }
  return grad;
}

std::unique_ptr<Loss> make_mse() { return std::make_unique<MSE>(); }

std::unique_ptr<Loss> make_mae() { return std::make_unique<MAE>(); }

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
}  // namespace sur

