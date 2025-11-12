#pragma once

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
=======
#include <cstddef>
#include <span>
>>>>>>> theirs
#include <vector>

namespace sur {

template <class T>
class Tensor;

class Optimizer {
 public:
  virtual ~Optimizer() = default;
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  virtual void step(const std::vector<Tensor<float>*>& params,
                    const std::vector<Tensor<float>*>& grads) = 0;
  virtual void zero_grad(const std::vector<Tensor<float>*>& grads) = 0;
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

  virtual void step(std::span<Tensor<float>* const> params,
                    std::span<Tensor<float>* const> grads) = 0;

  virtual void set_lr(float lr) = 0;
<<<<<<< ours
<<<<<<< ours
=======
  [[nodiscard]] virtual float current_lr() const noexcept = 0;
>>>>>>> theirs
=======
  [[nodiscard]] virtual float current_lr() const noexcept = 0;
>>>>>>> theirs
};

class SGD : public Optimizer {
 public:
  SGD(float lr, float momentum = 0.0f, float weight_decay = 0.0f);

  void step(std::span<Tensor<float>* const> params,
            std::span<Tensor<float>* const> grads) override;

  void set_lr(float lr) override;
<<<<<<< ours
<<<<<<< ours
=======
  [[nodiscard]] float current_lr() const noexcept override { return lr_; }
>>>>>>> theirs
=======
  [[nodiscard]] float current_lr() const noexcept override { return lr_; }
>>>>>>> theirs

  [[nodiscard]] float learning_rate() const noexcept { return lr_; }
  [[nodiscard]] float momentum() const noexcept { return momentum_; }
  [[nodiscard]] float weight_decay() const noexcept { return weight_decay_; }

 private:
  void ensure_state(std::span<Tensor<float>* const> params);

  float lr_ = 0.0f;
  float momentum_ = 0.0f;
  float weight_decay_ = 0.0f;

  std::vector<std::size_t> param_sizes_;
  std::vector<std::size_t> offsets_;
  std::vector<float> velocity_;
  std::vector<float> update_buffer_;
  std::size_t total_size_ = 0;
  bool state_ready_ = false;
};

class Adam : public Optimizer {
 public:
  Adam(float lr, float beta1 = 0.9f, float beta2 = 0.999f, float eps = 1e-8f);

  void step(std::span<Tensor<float>* const> params,
            std::span<Tensor<float>* const> grads) override;

  void set_lr(float lr) override;
<<<<<<< ours
<<<<<<< ours
=======
  [[nodiscard]] float current_lr() const noexcept override { return lr_; }
>>>>>>> theirs
=======
  [[nodiscard]] float current_lr() const noexcept override { return lr_; }
>>>>>>> theirs

  [[nodiscard]] float learning_rate() const noexcept { return lr_; }
  [[nodiscard]] float beta1() const noexcept { return beta1_; }
  [[nodiscard]] float beta2() const noexcept { return beta2_; }
  [[nodiscard]] float epsilon() const noexcept { return eps_; }

 private:
  void ensure_state(std::span<Tensor<float>* const> params);
  void reset_bias_correction();

  float lr_ = 0.0f;
  float beta1_ = 0.9f;
  float beta2_ = 0.999f;
  float eps_ = 1e-8f;

  std::vector<std::size_t> param_sizes_;
  std::vector<std::size_t> offsets_;
  std::vector<float> m_;
  std::vector<float> v_;
  std::vector<float> update_buffer_;
  std::size_t total_size_ = 0;
  bool state_ready_ = false;

  std::size_t step_ = 0;
  float beta1_pow_ = 1.0f;
  float beta2_pow_ = 1.0f;
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
};

}  // namespace sur

