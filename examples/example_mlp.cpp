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
#include <iostream>
#include <memory>

#include "surrogate/layers.hpp"
#include "surrogate/model.hpp"
#include "surrogate/tensor.hpp"

int main() {
  constexpr int input_dim = 4;
  constexpr int hidden_dim = 8;
  constexpr int output_dim = 2;
  constexpr int batch_size = 3;

  sur::Model model;
  model.add(std::make_unique<sur::Dense>(input_dim, hidden_dim));
  model.add(std::make_unique<sur::ReLU>());
  model.add(std::make_unique<sur::Dense>(hidden_dim, output_dim));

  model.reserve_workspaces(batch_size);

  sur::Tensor<float> input({input_dim, batch_size});
  float* data = input.data();
  for (int i = 0; i < input_dim * batch_size; ++i) {
    data[i] = 0.1f * static_cast<float>(i + 1);
  }

  {
    sur::Tensor<float> warmup_output = model.forward(input);
    sur::Tensor<float> warmup_grad({output_dim, batch_size});
    warmup_grad.fill(0.05f);
    model.backward(warmup_grad);
    (void)warmup_output;
  }

  sur::Tensor<float> output = model.forward(input);
  std::cout << "Output activations:\n";
  for (int row = 0; row < output_dim; ++row) {
    for (int col = 0; col < batch_size; ++col) {
      std::cout << output.data()[row * batch_size + col] << ' ';
    }
    std::cout << '\n';
  }

  sur::Tensor<float> grad_output({output_dim, batch_size});
  grad_output.fill(0.1f);
  sur::Tensor<float> grad_input = model.backward(grad_output);

  std::cout << "Input gradients:\n";
  for (int row = 0; row < input_dim; ++row) {
    for (int col = 0; col < batch_size; ++col) {
      std::cout << grad_input.data()[row * batch_size + col] << ' ';
    }
    std::cout << '\n';
  }

  return 0;
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
