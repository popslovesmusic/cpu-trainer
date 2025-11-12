#pragma once

#include <memory>
#include <vector>

namespace sur {

class Layer;

template <class T>
class Tensor;

class Model {
 public:
  Model();
  ~Model();

<<<<<<< ours
  void add_layer(std::unique_ptr<Layer> layer);
  std::vector<Tensor<float>*> parameters();
  std::vector<Tensor<float>*> gradients();

=======
  void add(std::unique_ptr<Layer> layer);

  Tensor<float> forward(const Tensor<float>& input);
  Tensor<float> backward(const Tensor<float>& grad_output);

  std::vector<Tensor<float>*> parameters();
  std::vector<Tensor<float>*> gradients();

  void reserve_workspaces(int max_batch);

>>>>>>> theirs
 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace sur

