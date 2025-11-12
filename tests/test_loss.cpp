#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

#include "surrogate/loss.hpp"
#include "surrogate/tensor.hpp"

namespace {

sur::Tensor<float> make_tensor(std::initializer_list<float> values) {
  sur::Tensor<float> tensor({1, static_cast<int>(values.size())});
  float* data = tensor.data();
  int idx = 0;
  for (float v : values) {
    data[idx++] = v;
  }
  return tensor;
}

bool almost_equal(float a, float b, float eps = 1e-6f) {
  return std::fabs(a - b) <= eps;
}

void test_mse_loss() {
  sur::Tensor<float> prediction = make_tensor({1.0f, 2.0f, 3.0f, 4.0f});
  sur::Tensor<float> target = make_tensor({0.0f, 2.0f, 2.0f, 5.0f});

  sur::MSE loss;
  float value = loss.compute(prediction, target);
  assert(almost_equal(value, 0.75f));

  sur::Tensor<float> grad = loss.gradient(prediction, target);
  const float expected[] = {0.5f, 0.0f, 0.5f, -0.5f};
  for (int i = 0; i < 4; ++i) {
    assert(almost_equal(grad.data()[i], expected[i]));
  }
}

void test_mae_loss() {
  sur::Tensor<float> prediction = make_tensor({-1.0f, 0.0f, 2.0f, 4.0f});
  sur::Tensor<float> target = make_tensor({0.0f, 0.0f, 1.0f, 6.0f});

  sur::MAE loss;
  float value = loss.compute(prediction, target);
  assert(almost_equal(value, 1.0f));

  sur::Tensor<float> grad = loss.gradient(prediction, target);
  const float expected[] = {-0.25f, 0.0f, 0.25f, -0.25f};
  for (int i = 0; i < 4; ++i) {
    assert(almost_equal(grad.data()[i], expected[i]));
  }
}

}  // namespace

int main() {
  test_mse_loss();
  test_mae_loss();
  std::cout << "Loss tests passed\n";
  return 0;
}

