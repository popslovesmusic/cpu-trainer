#include "surrogate/trainer.hpp"

<<<<<<< ours
#include <memory>

namespace sur {

struct Trainer::Impl {};

Trainer::Trainer(Model&, Loss&, Optimizer&, DataLoader&)
    : impl_(std::make_unique<Impl>()) {}

Trainer::~Trainer() = default;

void Trainer::train(const TrainerConfig&) {}
=======
#include <cstddef>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "surrogate/dataloader.hpp"
#include "surrogate/loss.hpp"
#include "surrogate/model.hpp"
#include "surrogate/optimizer.hpp"
#include "surrogate/tensor.hpp"

namespace sur {

namespace {

int normalise_threads(int requested) {
  return requested > 0 ? requested : 1;
}

void validate_config(const TrainConfig& config) {
  if (config.epochs <= 0) {
    throw std::invalid_argument("Trainer expects epochs > 0");
  }
  if (config.batch_size <= 0) {
    throw std::invalid_argument("Trainer expects batch_size > 0");
  }
  if (config.threads == 0) {
    throw std::invalid_argument("Trainer expects threads != 0");
  }
  if (config.log_every < 0) {
    throw std::invalid_argument("Trainer expects non-negative log interval");
  }
}

void scale_gradients(std::span<Tensor<float>* const> grads,
                     float scale,
                     int threads) {
  if (grads.empty() || scale == 1.0f) {
    return;
  }
#pragma omp parallel for schedule(static) num_threads(threads) if (threads > 1)
  for (int i = 0; i < static_cast<int>(grads.size()); ++i) {
    Tensor<float>* grad = grads[static_cast<std::size_t>(i)];
    if (!grad) {
      continue;
    }
    float* data = grad->data();
    const std::size_t size = grad->size();
    for (std::size_t idx = 0; idx < size; ++idx) {
      data[idx] *= scale;
    }
  }
}

}  // namespace

void Trainer::train(Model& model,
                    Optimizer& optimizer,
                    Loss& loss,
                    DataLoader& dataloader,
                    const TrainConfig& config) {
  validate_config(config);

  const int threads = normalise_threads(config.threads);
#ifdef _OPENMP
  if (config.deterministic) {
    omp_set_dynamic(0);
  }
#endif

  auto params_vec = model.parameters();
  auto grads_vec = model.gradients();
  if (params_vec.size() != grads_vec.size()) {
    throw std::runtime_error("Model parameters and gradients mismatch");
  }

  for (Tensor<float>* param : params_vec) {
    if (!param) {
      throw std::invalid_argument("Model returned null parameter");
    }
    if (!param->is_contiguous()) {
      throw std::invalid_argument("Trainer expects contiguous parameters");
    }
  }
  for (Tensor<float>* grad : grads_vec) {
    if (!grad) {
      throw std::invalid_argument("Model returned null gradient");
    }
    if (!grad->is_contiguous()) {
      throw std::invalid_argument("Trainer expects contiguous gradients");
    }
  }

  std::span<Tensor<float>* const> params(params_vec.data(), params_vec.size());
  std::span<Tensor<float>* const> grads(grads_vec.data(), grads_vec.size());

  Tensor<float> batch_inputs;
  Tensor<float> batch_targets;

  for (int epoch = 0; epoch < config.epochs; ++epoch) {
    dataloader.reset();
    while (dataloader.next_batch(batch_inputs, batch_targets)) {
      const auto& input_shape = batch_inputs.shape();
      const auto& target_shape = batch_targets.shape();
      if (input_shape[1] <= 0) {
        throw std::invalid_argument("Batch must contain at least one sample");
      }
      if (target_shape[1] != input_shape[1]) {
        throw std::invalid_argument("Target batch size mismatch");
      }
      if (input_shape[1] > config.batch_size) {
        throw std::invalid_argument("DataLoader returned batch larger than configuration");
      }

      model.reserve_workspaces(input_shape[1]);

      Tensor<float> predictions = model.forward(batch_inputs);
      [[maybe_unused]] const float loss_value = loss.compute(predictions, batch_targets);
      Tensor<float> loss_grad = loss.gradient(predictions, batch_targets);
      model.backward(loss_grad);

      const float scale = 1.0f / static_cast<float>(input_shape[1]);
      scale_gradients(grads, scale, threads);

      optimizer.step(params, grads);
    }
  }
}
>>>>>>> theirs

}  // namespace sur

