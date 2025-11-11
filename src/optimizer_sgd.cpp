#include "surrogate/optimizer.hpp"

namespace sur {

template <class T>
class Tensor;

class SGD : public Optimizer {
 public:
  void step(const std::vector<Tensor<float>*>&, const std::vector<Tensor<float>*>&) override {}
  void zero_grad(const std::vector<Tensor<float>*>&) override {}
};

}  // namespace sur

