<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
#include <cstdlib>

int main() { return EXIT_SUCCESS; }
=======
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs
=======
#include <algorithm>
#include <cassert>
#include <cmath>
>>>>>>> theirs

#include "surrogate/dataloader.hpp"
#include "surrogate/layers.hpp"
#include "surrogate/loss.hpp"
#include "surrogate/model.hpp"
#include "surrogate/optimizer.hpp"
#include "surrogate/tensor.hpp"
#include "surrogate/trainer.hpp"

namespace {

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
class SyntheticLoader final : public sur::DataLoader {
 public:
  SyntheticLoader(int input_dim,
                  int output_dim,
                  int total_samples,
                  int batch_size,
                  float weight,
                  float bias)
      : input_dim_(input_dim),
        output_dim_(output_dim),
        total_samples_(total_samples),
        batch_size_(batch_size),
        inputs_(static_cast<std::size_t>(input_dim) * static_cast<std::size_t>(total_samples)),
        targets_(static_cast<std::size_t>(output_dim) * static_cast<std::size_t>(total_samples)) {
    for (int n = 0; n < total_samples_; ++n) {
      const float x = static_cast<float>(n);
      for (int d = 0; d < input_dim_; ++d) {
        inputs_[static_cast<std::size_t>(d) * total_samples_ + n] = x;
      }
      const float y = weight * x + bias;
      for (int d = 0; d < output_dim_; ++d) {
        targets_[static_cast<std::size_t>(d) * total_samples_ + n] = y;
      }
    }
  }

  bool next_batch(sur::Tensor<float>& inputs, sur::Tensor<float>& targets) override {
    if (cursor_ >= total_samples_) {
      return false;
    }
    const int remaining = total_samples_ - cursor_;
    const int count = std::min(batch_size_, remaining);
    inputs = sur::Tensor<float>({input_dim_, count});
    targets = sur::Tensor<float>({output_dim_, count});

    for (int d = 0; d < input_dim_; ++d) {
      float* dst = inputs.data() + d * count;
      const float* src = inputs_.data() + static_cast<std::size_t>(d) * total_samples_ + cursor_;
      for (int n = 0; n < count; ++n) {
        dst[n] = src[n];
      }
    }

    for (int d = 0; d < output_dim_; ++d) {
      float* dst = targets.data() + d * count;
      const float* src = targets_.data() + static_cast<std::size_t>(d) * total_samples_ + cursor_;
      for (int n = 0; n < count; ++n) {
        dst[n] = src[n];
      }
    }

    cursor_ += count;
    return true;
  }

  void reset() override { cursor_ = 0; }

  std::size_t size() const noexcept override { return static_cast<std::size_t>(total_samples_); }

  sur::Tensor<float> full_inputs() const {
    sur::Tensor<float> tensor({input_dim_, total_samples_});
    for (int d = 0; d < input_dim_; ++d) {
      float* dst = tensor.data() + d * total_samples_;
      const float* src = inputs_.data() + static_cast<std::size_t>(d) * total_samples_;
      for (int n = 0; n < total_samples_; ++n) {
        dst[n] = src[n];
      }
    }
    return tensor;
  }

  sur::Tensor<float> full_targets() const {
    sur::Tensor<float> tensor({output_dim_, total_samples_});
    for (int d = 0; d < output_dim_; ++d) {
      float* dst = tensor.data() + d * total_samples_;
      const float* src = targets_.data() + static_cast<std::size_t>(d) * total_samples_;
      for (int n = 0; n < total_samples_; ++n) {
        dst[n] = src[n];
      }
    }
    return tensor;
  }

 private:
  int input_dim_ = 0;
  int output_dim_ = 0;
  int total_samples_ = 0;
  int batch_size_ = 0;
  int cursor_ = 0;
  std::vector<float> inputs_;
  std::vector<float> targets_;
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
void fill_linear_dataset(sur::Tensor<float>& inputs,
                         sur::Tensor<float>& targets,
                         float weight,
                         float bias) {
  const int input_dim = inputs.shape()[0];
  const int output_dim = targets.shape()[0];
  const int total_samples = inputs.shape()[1];
  for (int n = 0; n < total_samples; ++n) {
    const float x = static_cast<float>(n);
    const float y = weight * x + bias;
    for (int d = 0; d < input_dim; ++d) {
      inputs.data()[static_cast<std::size_t>(d) * static_cast<std::size_t>(total_samples) + n] = x;
    }
    for (int d = 0; d < output_dim; ++d) {
      targets.data()[static_cast<std::size_t>(d) * static_cast<std::size_t>(total_samples) + n] = y;
    }
  }
}
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

float compute_loss(sur::Model& model,
                   const sur::Loss& loss,
                   const sur::Tensor<float>& inputs,
                   const sur::Tensor<float>& targets) {
  sur::Tensor<float> predictions = model.forward(inputs);
  return loss.compute(predictions, targets);
}

void test_trainer_reduces_loss() {
  constexpr int input_dim = 1;
  constexpr int output_dim = 1;
  constexpr int total_samples = 16;
  constexpr int batch_size = 4;

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  SyntheticLoader loader(input_dim, output_dim, total_samples, batch_size, 2.0f, -1.0f);
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
  sur::Tensor<float> loader_inputs({input_dim, total_samples});
  sur::Tensor<float> loader_targets({output_dim, total_samples});
  fill_linear_dataset(loader_inputs, loader_targets, 2.0f, -1.0f);

  sur::Tensor<float> eval_inputs({input_dim, total_samples});
  sur::Tensor<float> eval_targets({output_dim, total_samples});
  fill_linear_dataset(eval_inputs, eval_targets, 2.0f, -1.0f);

  sur::TensorDataLoader loader(std::move(loader_inputs), std::move(loader_targets), batch_size, true, 1337u);
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

  sur::Model model;
  auto dense = std::make_unique<sur::Dense>(input_dim, output_dim);
  dense->weights().zero_();
  dense->bias().zero_();
  model.add(std::move(dense));

  sur::SGD optimizer(0.01f, 0.0f, 0.0f);
  sur::MSE loss;
  sur::Trainer trainer;

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  sur::Tensor<float> full_inputs = loader.full_inputs();
  sur::Tensor<float> full_targets = loader.full_targets();
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, full_inputs, full_targets);
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  model.reserve_workspaces(total_samples);

  const float initial_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs

  sur::TrainConfig config;
  config.epochs = 600;
  config.batch_size = batch_size;
  config.threads = 2;
  config.deterministic = true;
  config.log_every = 0;
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours

  trainer.train(model, optimizer, loss, loader, config);

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  const float final_loss = compute_loss(model, loss, full_inputs, full_targets);
=======
  const float final_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  const float final_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  const float final_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
  const float final_loss = compute_loss(model, loss, eval_inputs, eval_targets);
>>>>>>> theirs
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
  config.seed = 1337u;

  trainer.train(model, optimizer, loss, loader, config);

  const float final_loss = compute_loss(model, loss, eval_inputs, eval_targets);
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
  assert(final_loss < initial_loss);
  assert(std::fabs(final_loss) < 1e-2f);
}

}  // namespace

int main() {
  test_trainer_reduces_loss();
  return 0;
}
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

