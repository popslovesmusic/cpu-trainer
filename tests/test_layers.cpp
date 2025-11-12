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
#include <cstdlib>

int main() { return EXIT_SUCCESS; }
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
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

#include "surrogate/layers.hpp"
#include "surrogate/tensor.hpp"

using sur::Dense;
using sur::ReLU;
using sur::Sigmoid;
using sur::Tanh;
using sur::Tensor;

namespace {
Tensor<float> make_tensor(int dim0, int dim1, std::initializer_list<float> values) {
  Tensor<float> t({dim0, dim1});
  assert(static_cast<int>(values.size()) == dim0 * dim1);
  auto* data = t.data();
  int idx = 0;
  for (float v : values) {
    data[idx++] = v;
  }
  return t;
}

bool almost_equal(float a, float b, float eps = 1e-5f) {
  return std::fabs(a - b) <= eps;
}

void test_dense_forward_backward() {
  Dense dense(3, 2);
  dense.reserve_workspaces(2);

  auto& W = dense.weights();
  float* w = W.data();
  w[0] = 1.0f; w[1] = 2.0f; w[2] = 3.0f;
  w[3] = 4.0f; w[4] = 5.0f; w[5] = 6.0f;

  auto& b = dense.bias();
  float* bptr = b.data();
  bptr[0] = 0.5f;
  bptr[1] = -0.5f;

  Tensor<float> x = make_tensor(3, 2, {1.0f, 2.0f, -1.0f, 0.0f, 0.5f, -0.5f});
  Tensor<float> y = dense.forward(x);

  const float expected_y[] = {1.0f, 1.0f, 1.5f, 4.5f};
  for (int i = 0; i < 4; ++i) {
    assert(almost_equal(y.data()[i], expected_y[i]));
  }

  Tensor<float> dy = make_tensor(2, 2, {0.1f, -0.2f, 0.05f, 0.3f});
  Tensor<float> dx = dense.backward(dy);

  const float expected_dx[] = {0.3f, 1.0f, 0.45f, 1.1f, 0.6f, 1.2f};
  for (int i = 0; i < 6; ++i) {
    assert(almost_equal(dx.data()[i], expected_dx[i]));
  }

  auto& dW = dense.grad_weights();
  const float expected_dW[] = {-0.3f, -0.1f, 0.15f, 0.65f, -0.05f, -0.125f};
  for (int i = 0; i < 6; ++i) {
    assert(almost_equal(dW.data()[i], expected_dW[i]));
  }

  auto& db = dense.grad_bias();
  const float expected_db[] = {-0.1f, 0.35f};
  for (int i = 0; i < 2; ++i) {
    assert(almost_equal(db.data()[i], expected_db[i]));
  }
}

void test_relu_layer() {
  ReLU relu;
  Tensor<float> x = make_tensor(2, 3, {-1.0f, 0.0f, 1.0f, 2.0f, -0.5f, 0.5f});
  Tensor<float> y = relu.forward(x);
  const float expected_y[] = {0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.5f};
  for (int i = 0; i < 6; ++i) {
    assert(almost_equal(y.data()[i], expected_y[i]));
  }

  Tensor<float> dy = make_tensor(2, 3, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f});
  Tensor<float> dx = relu.backward(dy);
  const float expected_dx[] = {0.0f, 0.0f, 0.3f, 0.4f, 0.0f, 0.6f};
  for (int i = 0; i < 6; ++i) {
    assert(almost_equal(dx.data()[i], expected_dx[i]));
  }
}

void test_tanh_layer() {
  Tanh layer;
  Tensor<float> x = make_tensor(1, 3, {-1.0f, 0.0f, 1.0f});
  Tensor<float> y = layer.forward(x);
  const float expected_y[] = {std::tanh(-1.0f), std::tanh(0.0f), std::tanh(1.0f)};
  for (int i = 0; i < 3; ++i) {
    assert(almost_equal(y.data()[i], expected_y[i]));
  }

  Tensor<float> dy = make_tensor(1, 3, {0.2f, 0.3f, 0.4f});
  Tensor<float> dx = layer.backward(dy);
  for (int i = 0; i < 3; ++i) {
    float expected = dy.data()[i] * (1.0f - expected_y[i] * expected_y[i]);
    assert(almost_equal(dx.data()[i], expected));
  }
}

void test_sigmoid_layer() {
  Sigmoid layer;
  Tensor<float> x = make_tensor(1, 3, {-2.0f, 0.0f, 2.0f});
  Tensor<float> y = layer.forward(x);
  auto sigmoid = [](float v) {
    if (v >= 0.0f) {
      float exp_neg = std::exp(-v);
      return 1.0f / (1.0f + exp_neg);
    }
    float exp_pos = std::exp(v);
    return exp_pos / (1.0f + exp_pos);
  };
  const float expected_y[] = {sigmoid(-2.0f), sigmoid(0.0f), sigmoid(2.0f)};
  for (int i = 0; i < 3; ++i) {
    assert(almost_equal(y.data()[i], expected_y[i]));
  }

  Tensor<float> dy = make_tensor(1, 3, {0.3f, 0.4f, 0.5f});
  Tensor<float> dx = layer.backward(dy);
  for (int i = 0; i < 3; ++i) {
    float expected = dy.data()[i] * expected_y[i] * (1.0f - expected_y[i]);
    assert(almost_equal(dx.data()[i], expected));
  }
}
}  // namespace

int main() {
  test_dense_forward_backward();
  test_relu_layer();
  test_tanh_layer();
  test_sigmoid_layer();
  std::cout << "Layers tests passed\n";
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

