#include <cassert>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

#include "surrogate/dataloader.hpp"
#include "surrogate/tensor.hpp"

namespace {

void fill_dataset(sur::Tensor<float>& inputs,
                  sur::Tensor<float>& targets,
                  float scale_inputs,
                  float scale_targets) {
  const int features = inputs.shape()[0];
  const int outputs = targets.shape()[0];
  const int samples = inputs.shape()[1];
  for (int sample = 0; sample < samples; ++sample) {
    const float value_in = scale_inputs * static_cast<float>(sample);
    const float value_out = scale_targets * static_cast<float>(sample);
    for (int f = 0; f < features; ++f) {
      inputs.data()[static_cast<std::size_t>(f) * static_cast<std::size_t>(samples) + sample] =
          value_in + static_cast<float>(f) * 0.1f;
    }
    for (int o = 0; o < outputs; ++o) {
      targets.data()[static_cast<std::size_t>(o) * static_cast<std::size_t>(samples) + sample] =
          value_out + static_cast<float>(o) * 0.2f;
    }
  }
}

void test_aligned_batches_and_sequential_iteration() {
  constexpr int input_dim = 3;
  constexpr int output_dim = 2;
  constexpr int total_samples = 8;
  constexpr int batch_size = 4;

  sur::Tensor<float> loader_inputs({input_dim, total_samples});
  sur::Tensor<float> loader_targets({output_dim, total_samples});
  fill_dataset(loader_inputs, loader_targets, 1.0f, 2.0f);

  sur::TensorDataLoader loader(std::move(loader_inputs), std::move(loader_targets), batch_size, false);

  sur::Tensor<float> batch_inputs;
  sur::Tensor<float> batch_targets;

  loader.reset();

  bool has_batch = loader.next_batch(batch_inputs, batch_targets);
  assert(has_batch);
  assert(reinterpret_cast<std::uintptr_t>(batch_inputs.data()) % 32 == 0);
  assert(reinterpret_cast<std::uintptr_t>(batch_targets.data()) % 32 == 0);
  assert(batch_inputs.shape()[1] == batch_size);
  assert(batch_targets.shape()[1] == batch_size);

  for (int sample = 0; sample < batch_size; ++sample) {
    const float expected_input = 1.0f * static_cast<float>(sample);
    const float expected_target = 2.0f * static_cast<float>(sample);
    for (int f = 0; f < input_dim; ++f) {
      const float value = batch_inputs.data()[static_cast<std::size_t>(f) * batch_size + sample];
      assert(value == expected_input + static_cast<float>(f) * 0.1f);
    }
    for (int o = 0; o < output_dim; ++o) {
      const float value = batch_targets.data()[static_cast<std::size_t>(o) * batch_size + sample];
      assert(value == expected_target + static_cast<float>(o) * 0.2f);
    }
  }

  has_batch = loader.next_batch(batch_inputs, batch_targets);
  assert(has_batch);
  assert(batch_inputs.shape()[1] == total_samples - batch_size);
  assert(batch_targets.shape()[1] == total_samples - batch_size);

  for (int sample = 0; sample < batch_inputs.shape()[1]; ++sample) {
    const int absolute_index = batch_size + sample;
    const float expected_input = 1.0f * static_cast<float>(absolute_index);
    const float expected_target = 2.0f * static_cast<float>(absolute_index);
    for (int f = 0; f < input_dim; ++f) {
      const float value = batch_inputs.data()[static_cast<std::size_t>(f) * batch_inputs.shape()[1] + sample];
      assert(value == expected_input + static_cast<float>(f) * 0.1f);
    }
    for (int o = 0; o < output_dim; ++o) {
      const float value = batch_targets.data()[static_cast<std::size_t>(o) * batch_targets.shape()[1] + sample];
      assert(value == expected_target + static_cast<float>(o) * 0.2f);
    }
  }

  has_batch = loader.next_batch(batch_inputs, batch_targets);
  assert(!has_batch);
}

void test_shuffle_changes_order_across_epochs() {
  constexpr int input_dim = 1;
  constexpr int output_dim = 1;
  constexpr int total_samples = 10;
  constexpr int batch_size = 4;
  constexpr unsigned int seed = 1234u;

  sur::Tensor<float> loader_inputs({input_dim, total_samples});
  sur::Tensor<float> loader_targets({output_dim, total_samples});
  fill_dataset(loader_inputs, loader_targets, 1.0f, 1.0f);

  sur::TensorDataLoader loader(std::move(loader_inputs), std::move(loader_targets), batch_size, true, seed);

  std::vector<int> epoch_one_order;
  std::vector<int> epoch_two_order;

  sur::Tensor<float> batch_inputs;
  sur::Tensor<float> batch_targets;

  loader.reset();
  while (loader.next_batch(batch_inputs, batch_targets)) {
    const int count = batch_inputs.shape()[1];
    for (int i = 0; i < count; ++i) {
      epoch_one_order.push_back(static_cast<int>(batch_inputs.data()[i]));
    }
  }

  loader.reset();
  while (loader.next_batch(batch_inputs, batch_targets)) {
    const int count = batch_inputs.shape()[1];
    for (int i = 0; i < count; ++i) {
      epoch_two_order.push_back(static_cast<int>(batch_inputs.data()[i]));
    }
  }

  assert(epoch_one_order.size() == static_cast<std::size_t>(total_samples));
  assert(epoch_two_order.size() == static_cast<std::size_t>(total_samples));

  std::vector<int> expected(total_samples);
  std::iota(expected.begin(), expected.end(), 0);
  std::mt19937 rng(seed);
  std::shuffle(expected.begin(), expected.end(), rng);
  assert(epoch_one_order == expected);
  std::shuffle(expected.begin(), expected.end(), rng);
  assert(epoch_two_order == expected);
}

}  // namespace

int main() {
  test_aligned_batches_and_sequential_iteration();
  test_shuffle_changes_order_across_epochs();
  return 0;
}

