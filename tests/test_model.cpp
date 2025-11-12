#include <cassert>
#include <iostream>
#include <memory>

#include "surrogate/layers.hpp"
#include "surrogate/model.hpp"
#include "surrogate/tensor.hpp"

using sur::Dense;
using sur::Model;
using sur::ReLU;
using sur::Tensor;

namespace {
Tensor<float> make_tensor(int rows, int cols, float base) {
  Tensor<float> t({rows, cols});
  float* data = t.data();
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      data[r * cols + c] = base + static_cast<float>(r * cols + c) * 0.1f;
    }
  }
  return t;
}

void test_sequential_forward_backward() {
  Model model;
  model.add(std::make_unique<Dense>(3, 4));
  model.add(std::make_unique<ReLU>());
  model.add(std::make_unique<Dense>(4, 2));

  const int batch = 2;
  model.reserve_workspaces(batch);

  Tensor<float> input = make_tensor(3, batch, 0.5f);
  Tensor<float> output = model.forward(input);
  assert(output.shape()[0] == 2);
  assert(output.shape()[1] == batch);

  Tensor<float> grad_out({2, batch});
  grad_out.fill(0.25f);
  Tensor<float> grad_in = model.backward(grad_out);
  assert(grad_in.shape()[0] == 3);
  assert(grad_in.shape()[1] == batch);
}

void test_parameter_collection() {
  Model model;
  auto dense1 = std::make_unique<Dense>(2, 3);
  auto dense2 = std::make_unique<Dense>(3, 1);
  model.add(std::move(dense1));
  model.add(std::move(dense2));

  auto params = model.parameters();
  auto grads = model.gradients();

  assert(!params.empty());
  assert(params.size() == grads.size());
  for (auto* p : params) {
    assert(p != nullptr);
    assert(p->data() != nullptr);
  }
  for (auto* g : grads) {
    assert(g != nullptr);
    assert(g->data() != nullptr);
  }
}
}  // namespace

int main() {
  test_sequential_forward_backward();
  test_parameter_collection();
  std::cout << "Model tests passed\n";
  return 0;
}
