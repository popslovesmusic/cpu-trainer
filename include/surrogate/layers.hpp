#pragma once

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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
#include <memory>
#include <vector>

namespace sur {

template <class T>
class Tensor;
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
#include <array>
#include <memory>
#include <span>
#include <vector>

#include "surrogate/mathkernels.hpp"
#include "surrogate/tensor.hpp"
#include "surrogate/tensor_view.hpp"

namespace sur {
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

class Layer {
 public:
  virtual ~Layer() = default;
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  virtual void forward() = 0;
  virtual void backward() = 0;
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
  virtual Tensor<float> forward(const Tensor<float>& x) = 0;
  virtual Tensor<float> backward(const Tensor<float>& dy) = 0;
  virtual std::span<Tensor<float>*> params() noexcept { return {}; }
  virtual std::span<Tensor<float>*> grads() noexcept { return {}; }
  virtual void reserve_workspaces(int /*max_batch*/) {}
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
};

using LayerPtr = std::unique_ptr<Layer>;

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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
std::vector<LayerPtr> create_default_layers();
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
class Dense final : public Layer {
 public:
  Dense(int in_features, int out_features, bool use_bias = true);

  Tensor<float> forward(const Tensor<float>& x) override;
  Tensor<float> backward(const Tensor<float>& dy) override;

  std::span<Tensor<float>*> params() noexcept override;
  std::span<Tensor<float>*> grads() noexcept override;
  void reserve_workspaces(int max_batch) override;

  Tensor<float>& weights() noexcept { return weight_; }
  Tensor<float>& bias() noexcept { return bias_; }
  Tensor<float>& grad_weights() noexcept { return grad_weight_; }
  Tensor<float>& grad_bias() noexcept { return grad_bias_; }

 private:
  void ensure_workspaces(int batch_size);
  void transpose_input(const Tensor<float>& x, int batch_size);
  void transpose_output_grad(const Tensor<float>& dy, int batch_size);

  int in_features_ = 0;
  int out_features_ = 0;
  bool use_bias_ = true;

  Tensor<float> weight_;
  Tensor<float> bias_;
  Tensor<float> grad_weight_;
  Tensor<float> grad_bias_;

  Tensor<float> input_transposed_;
  Tensor<float> dy_transposed_;
  Tensor<float> dx_transposed_;

  int last_batch_size_ = 0;
  bool has_cache_ = false;

  std::array<Tensor<float>*, 2> param_ptrs_{};
  std::array<Tensor<float>*, 2> grad_ptrs_{};
};

class ReLU final : public Layer {
 public:
  Tensor<float> forward(const Tensor<float>& x) override;
  Tensor<float> backward(const Tensor<float>& dy) override;

 private:
  TensorView<const float> last_output_;
  std::array<int, 4> last_shape_{};
  bool has_cache_ = false;
};

class Tanh final : public Layer {
 public:
  Tensor<float> forward(const Tensor<float>& x) override;
  Tensor<float> backward(const Tensor<float>& dy) override;

 private:
  TensorView<const float> last_output_;
  std::array<int, 4> last_shape_{};
  bool has_cache_ = false;
};

class Sigmoid final : public Layer {
 public:
  Tensor<float> forward(const Tensor<float>& x) override;
  Tensor<float> backward(const Tensor<float>& dy) override;

 private:
  TensorView<const float> last_output_;
  std::array<int, 4> last_shape_{};
  bool has_cache_ = false;
};
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

