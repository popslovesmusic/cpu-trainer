#include "surrogate/dataloader.hpp"

<<<<<<< ours
<<<<<<< ours
namespace sur {
=======
=======
>>>>>>> theirs
#include <algorithm>
#include <array>
#include <numeric>
#include <stdexcept>

#include "surrogate/tensor.hpp"

namespace sur {
namespace {
int extract_dim(const std::array<int, 4>& shape, std::size_t axis) {
  if (axis >= shape.size()) {
    return 0;
  }
  return shape[axis];
}
}

TensorDataLoader::TensorDataLoader(Tensor<float> inputs,
                                   Tensor<float> targets,
                                   int batch_size,
                                   bool shuffle,
                                   unsigned int seed)
    : inputs_(std::move(inputs)),
      targets_(std::move(targets)),
      batch_size_(batch_size),
      shuffle_(shuffle),
      rng_(seed) {
  if (batch_size_ <= 0) {
    throw std::invalid_argument("TensorDataLoader expects positive batch size");
  }
  if (!inputs_.is_contiguous() || !targets_.is_contiguous()) {
    throw std::invalid_argument("TensorDataLoader requires contiguous tensors");
  }

  const DatasetShape shape = infer_shape();
  if (batch_size_ > shape.samples) {
    throw std::invalid_argument("TensorDataLoader batch size exceeds dataset size");
  }

  input_dim_ = shape.input_dim;
  output_dim_ = shape.output_dim;
  total_samples_ = shape.samples;

  prepare_indices();
}

bool TensorDataLoader::next_batch(Tensor<float>& inputs, Tensor<float>& targets) {
  if (cursor_ >= total_samples_) {
    return false;
  }

  const int remaining = total_samples_ - cursor_;
  const int count = remaining < batch_size_ ? remaining : batch_size_;

  Tensor<float> next_inputs({input_dim_, count});
  Tensor<float> next_targets({output_dim_, count});

  const float* source_inputs = inputs_.data();
  const float* source_targets = targets_.data();
  float* dst_inputs = next_inputs.data();
  float* dst_targets = next_targets.data();

  for (int row = 0; row < input_dim_; ++row) {
    const float* src_row = source_inputs + static_cast<std::size_t>(row) * static_cast<std::size_t>(total_samples_);
    float* dst_row = dst_inputs + static_cast<std::size_t>(row) * static_cast<std::size_t>(count);
    for (int i = 0; i < count; ++i) {
      const int sample_index = indices_[static_cast<std::size_t>(cursor_ + i)];
      dst_row[i] = src_row[sample_index];
    }
  }

  for (int row = 0; row < output_dim_; ++row) {
    const float* src_row = source_targets + static_cast<std::size_t>(row) * static_cast<std::size_t>(total_samples_);
    float* dst_row = dst_targets + static_cast<std::size_t>(row) * static_cast<std::size_t>(count);
    for (int i = 0; i < count; ++i) {
      const int sample_index = indices_[static_cast<std::size_t>(cursor_ + i)];
      dst_row[i] = src_row[sample_index];
    }
  }

  cursor_ += count;
  inputs = std::move(next_inputs);
  targets = std::move(next_targets);
  return true;
}

void TensorDataLoader::reset() {
  cursor_ = 0;
  if (shuffle_ && total_samples_ > 1) {
    std::shuffle(indices_.begin(), indices_.end(), rng_);
  }
}

std::size_t TensorDataLoader::size() const noexcept {
  return static_cast<std::size_t>(total_samples_);
}

TensorDataLoader::DatasetShape TensorDataLoader::infer_shape() const {
  const auto& input_shape = inputs_.shape();
  const auto& target_shape = targets_.shape();

  int inferred_input_dim = extract_dim(input_shape, 0);
  int inferred_input_samples = extract_dim(input_shape, 1);
  int inferred_output_dim = extract_dim(target_shape, 0);
  int inferred_output_samples = extract_dim(target_shape, 1);

  if (inferred_input_dim <= 0 || inferred_output_dim <= 0) {
    throw std::invalid_argument("TensorDataLoader expects non-zero feature dimensions");
  }
  if (inferred_input_samples <= 0 || inferred_output_samples <= 0) {
    throw std::invalid_argument("TensorDataLoader expects non-zero sample count");
  }
  if (inferred_input_samples != inferred_output_samples) {
    throw std::invalid_argument("TensorDataLoader input/target sample mismatch");
  }
  return DatasetShape{inferred_input_dim, inferred_output_dim, inferred_input_samples};
}

void TensorDataLoader::prepare_indices() {
  indices_.resize(static_cast<std::size_t>(total_samples_));
  std::iota(indices_.begin(), indices_.end(), 0);
  cursor_ = 0;
}
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs

}  // namespace sur

