#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "surrogate/optimizer.hpp"
#include "surrogate/tensor.hpp"

namespace {

struct LinearDataset {
  std::vector<float> x{1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<float> y{3.0f, 5.0f, 7.0f, 9.0f};
};

float compute_loss(const sur::Tensor<float>& weights, const LinearDataset& data) {
  const float* w = weights.data();
  float total = 0.0f;
  for (std::size_t i = 0; i < data.x.size(); ++i) {
    const float pred = w[0] * data.x[i] + w[1];
    const float diff = pred - data.y[i];
    total += diff * diff;
  }
  return total / static_cast<float>(data.x.size());
}

void compute_gradients(const sur::Tensor<float>& weights, sur::Tensor<float>& grad,
                       const LinearDataset& data) {
  const float* w = weights.data();
  float* g = grad.data();
  g[0] = 0.0f;
  g[1] = 0.0f;
  for (std::size_t i = 0; i < data.x.size(); ++i) {
    const float pred = w[0] * data.x[i] + w[1];
    const float diff = pred - data.y[i];
    g[0] += diff * data.x[i];
    g[1] += diff;
  }
  const float scale = 2.0f / static_cast<float>(data.x.size());
  g[0] *= scale;
  g[1] *= scale;
}

sur::Tensor<float> make_param_tensor(float w, float b) {
  sur::Tensor<float> tensor({1, 2});
  float* data = tensor.data();
  data[0] = w;
  data[1] = b;
  return tensor;
}

void test_sgd_convergence() {
  LinearDataset data;
  sur::Tensor<float> weights = make_param_tensor(0.0f, 0.0f);
  sur::Tensor<float> grad({1, 2});

  sur::SGD optimizer(0.05f, 0.0f);

  float last_loss = compute_loss(weights, data);
  const float initial_loss = last_loss;
  for (int step = 0; step < 15; ++step) {
    compute_gradients(weights, grad, data);
    std::vector<sur::Tensor<float>*> params{&weights};
    std::vector<sur::Tensor<float>*> grads{&grad};
    optimizer.step(params, grads);
    const float new_loss = compute_loss(weights, data);
    assert(new_loss <= last_loss + 1e-6f);
    last_loss = new_loss;
  }
  assert(last_loss < initial_loss);
}

void test_sgd_weight_decay() {
  sur::Tensor<float> weights = make_param_tensor(1.0f, -1.0f);
  sur::Tensor<float> grad({1, 2});
  grad.zero_();

  sur::SGD optimizer(0.1f, 0.0f, 0.5f);
  std::vector<sur::Tensor<float>*> params{&weights};
  std::vector<sur::Tensor<float>*> grads{&grad};

  optimizer.step(params, grads);

  const float* w = weights.data();
  const float* g = grad.data();
  assert(w[0] < 1.0f);
  assert(w[1] > -1.0f);
  assert(g[0] == 0.0f && g[1] == 0.0f);
}

void test_sgd_momentum() {
  sur::Tensor<float> weights = make_param_tensor(0.0f, 0.0f);
  sur::Tensor<float> grad({1, 2});
  float* g = grad.data();
  g[0] = 1.0f;
  g[1] = 1.0f;

  sur::SGD optimizer(0.1f, 0.5f);
  std::vector<sur::Tensor<float>*> params{&weights};
  std::vector<sur::Tensor<float>*> grads{&grad};

  optimizer.step(params, grads);
  const float after_first = weights.data()[0];
  optimizer.step(params, grads);
  const float after_second = weights.data()[0];

  assert(std::fabs(after_first + 0.1f) <= 1e-6f);
  assert(std::fabs(after_second + 0.25f) <= 1e-6f);
}

void test_adam_convergence() {
  LinearDataset data;
  sur::Tensor<float> weights = make_param_tensor(0.5f, -0.5f);
  sur::Tensor<float> grad({1, 2});

  sur::Adam optimizer(0.05f);

  float last_loss = compute_loss(weights, data);
  const float initial_loss = last_loss;
  for (int step = 0; step < 20; ++step) {
    compute_gradients(weights, grad, data);
    std::vector<sur::Tensor<float>*> params{&weights};
    std::vector<sur::Tensor<float>*> grads{&grad};
    optimizer.step(params, grads);
    const float new_loss = compute_loss(weights, data);
    assert(new_loss <= last_loss + 1e-6f);
    last_loss = new_loss;
  }
  assert(last_loss < initial_loss);
}

}  // namespace

int main() {
  test_sgd_convergence();
  test_sgd_weight_decay();
  test_sgd_momentum();
  test_adam_convergence();
  std::cout << "Optimizer tests passed\n";
  return 0;
}

