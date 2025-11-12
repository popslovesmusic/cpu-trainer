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
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>

#include "surrogate/layers.hpp"
#include "surrogate/loss.hpp"
#include "surrogate/model.hpp"
#include "surrogate/tensor.hpp"

namespace {

void fill_inputs(sur::Tensor<float>& inputs) {
  const auto& shape = inputs.shape();
  const int features = shape[0];
  const int batch = shape[1];
  float value = -0.2f;
  float* data = inputs.data();
  for (int f = 0; f < features; ++f) {
    for (int n = 0; n < batch; ++n) {
      data[f * batch + n] = value;
      value += 0.15f;
    }
  }
}

void fill_targets(sur::Tensor<float>& targets) {
  const auto& shape = targets.shape();
  const int outputs = shape[0];
  const int batch = shape[1];
  float value = 0.1f;
  float* data = targets.data();
  for (int o = 0; o < outputs; ++o) {
    for (int n = 0; n < batch; ++n) {
      data[o * batch + n] = value;
      value -= 0.12f;
    }
  }
}

void fill_sequence(sur::Tensor<float>& tensor, float start, float step) {
  float value = start;
  float* data = tensor.data();
  const std::size_t count = tensor.size();
  for (std::size_t i = 0; i < count; ++i) {
    data[i] = value;
    value += step;
  }
}

void run_gradient_check() {
  constexpr int input_dim = 2;
  constexpr int hidden_dim = 2;
  constexpr int output_dim = 1;
  constexpr int batch_size = 2;

  sur::Tensor<float> inputs({input_dim, batch_size});
  sur::Tensor<float> targets({output_dim, batch_size});
  fill_inputs(inputs);
  fill_targets(targets);

  sur::Model model;
  auto dense0 = std::make_unique<sur::Dense>(input_dim, hidden_dim);
  auto activation = std::make_unique<sur::Tanh>();
  auto dense1 = std::make_unique<sur::Dense>(hidden_dim, output_dim);

  auto* dense0_ptr = dense0.get();
  auto* dense1_ptr = dense1.get();

  model.add(std::move(dense0));
  model.add(std::move(activation));
  model.add(std::move(dense1));

  fill_sequence(dense0_ptr->weights(), -0.35f, 0.05f);
  fill_sequence(dense0_ptr->bias(), 0.08f, -0.03f);
  fill_sequence(dense1_ptr->weights(), 0.12f, -0.04f);
  fill_sequence(dense1_ptr->bias(), -0.06f, 0.02f);

  model.reserve_workspaces(batch_size);

  sur::Tensor<float> predictions = model.forward(inputs);
  const std::size_t loss_grad_size = predictions.size();

  sur::MSE mse;
  mse.compute(predictions, targets);
  sur::Tensor<float> grad_predictions = mse.gradient(predictions, targets);
  model.backward(grad_predictions);

  auto params = model.parameters();
  auto grads = model.gradients();
  assert(params.size() == grads.size());

  std::vector<std::vector<float>> analytic(grads.size());
  for (std::size_t p = 0; p < grads.size(); ++p) {
    sur::Tensor<float>* grad = grads[p];
    assert(grad != nullptr);
    const float* data = grad->data();
    analytic[p].assign(data, data + grad->size());
  }

  const float epsilon = 1e-4f;
  const float abs_tol = 1e-3f;
  const float rel_tol = 1e-2f;

  float max_abs_err = 0.0f;
  float max_rel_err = 0.0f;

  for (std::size_t p = 0; p < params.size(); ++p) {
    sur::Tensor<float>* param = params[p];
    assert(param != nullptr);
    assert(param->size() == analytic[p].size());

    float* param_data = param->data();
    const std::size_t count = param->size();

    for (std::size_t i = 0; i < count; ++i) {
      const float original = param_data[i];

      param_data[i] = original + epsilon;
      sur::Tensor<float> pred_plus = model.forward(inputs);

      param_data[i] = original - epsilon;
      sur::Tensor<float> pred_minus = model.forward(inputs);

      param_data[i] = original;

      const float* plus_data = pred_plus.data();
      const float* minus_data = pred_minus.data();
      float numerical = 0.0f;
      for (std::size_t j = 0; j < loss_grad_size; ++j) {
        numerical += grad_predictions.data()[j] * (plus_data[j] - minus_data[j]);
      }
      numerical /= (2.0f * epsilon);
      const float analytical = analytic[p][i];
      const float abs_err = std::fabs(analytical - numerical);
      const float denom = std::max({1.0f, std::fabs(analytical), std::fabs(numerical)});
      const float rel_err = abs_err / denom;

      max_abs_err = std::max(max_abs_err, abs_err);
      max_rel_err = std::max(max_rel_err, rel_err);
    }
  }

  assert(max_abs_err < abs_tol);
  assert(max_rel_err < rel_tol);
}

}  // namespace

int main() {
  run_gradient_check();
  return 0;
}
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs

