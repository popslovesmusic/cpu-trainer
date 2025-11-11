#pragma once

#include <vector>

namespace sur {

template <class T>
class Tensor;

class Optimizer {
 public:
  virtual ~Optimizer() = default;
  virtual void step(const std::vector<Tensor<float>*>& params,
                    const std::vector<Tensor<float>*>& grads) = 0;
  virtual void zero_grad(const std::vector<Tensor<float>*>& grads) = 0;
};

}  // namespace sur

