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

  void add_layer(std::unique_ptr<Layer> layer);
  std::vector<Tensor<float>*> parameters();
  std::vector<Tensor<float>*> gradients();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace sur

