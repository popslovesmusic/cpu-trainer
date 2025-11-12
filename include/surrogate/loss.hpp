#pragma once

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
namespace sur {

class Loss {
 public:
  virtual ~Loss() = default;
  virtual float forward() = 0;
  virtual void backward() = 0;
};

Loss* create_mse_loss();
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
#include <memory>

namespace sur {

template <class T>
class Tensor;

class Loss {
 public:
  virtual ~Loss() = default;
  virtual float compute(const Tensor<float>& prediction, const Tensor<float>& target) const = 0;
  virtual Tensor<float> gradient(const Tensor<float>& prediction, const Tensor<float>& target) const = 0;
};

class MSE final : public Loss {
 public:
  float compute(const Tensor<float>& prediction, const Tensor<float>& target) const override;
  Tensor<float> gradient(const Tensor<float>& prediction, const Tensor<float>& target) const override;
};

class MAE final : public Loss {
 public:
  float compute(const Tensor<float>& prediction, const Tensor<float>& target) const override;
  Tensor<float> gradient(const Tensor<float>& prediction, const Tensor<float>& target) const override;
};

std::unique_ptr<Loss> make_mse();
std::unique_ptr<Loss> make_mae();
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

}  // namespace sur

