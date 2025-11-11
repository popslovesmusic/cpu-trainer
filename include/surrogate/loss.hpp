#pragma once

namespace sur {

class Loss {
 public:
  virtual ~Loss() = default;
  virtual float forward() = 0;
  virtual void backward() = 0;
};

Loss* create_mse_loss();

}  // namespace sur

