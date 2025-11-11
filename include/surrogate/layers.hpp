#pragma once

#include <memory>
#include <vector>

namespace sur {

template <class T>
class Tensor;

class Layer {
 public:
  virtual ~Layer() = default;
  virtual void forward() = 0;
  virtual void backward() = 0;
};

using LayerPtr = std::unique_ptr<Layer>;

std::vector<LayerPtr> create_default_layers();

}  // namespace sur

