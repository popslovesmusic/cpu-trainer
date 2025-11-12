#pragma once

#include <cstddef>
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

namespace sur {

template <class T>
class Tensor;

<<<<<<< ours
template <class T>
class TensorView;
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
#include <random>
#include <vector>

#include "surrogate/tensor.hpp"

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

class DataLoader {
 public:
  virtual ~DataLoader() = default;
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
  virtual bool next_batch(TensorView<float>& inputs, TensorView<float>& targets) = 0;
=======
class DataLoader {
 public:
  virtual ~DataLoader() = default;
  virtual bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) = 0;
>>>>>>> theirs
=======
  virtual bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) = 0;
>>>>>>> theirs
=======
  virtual bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) = 0;
>>>>>>> theirs
=======
  virtual bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) = 0;
>>>>>>> theirs
=======
  virtual bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) = 0;
>>>>>>> theirs
  virtual void reset() = 0;
  virtual std::size_t size() const noexcept = 0;
};

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
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
  virtual bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) = 0;
  virtual void reset() = 0;
  virtual std::size_t size() const noexcept = 0;
  virtual void set_seed(unsigned int seed) = 0;
};

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
class TensorDataLoader final : public DataLoader {
 public:
  TensorDataLoader(Tensor<float> inputs,
                   Tensor<float> targets,
                   int batch_size,
                   bool shuffle = true,
                   unsigned int seed = 5489u);

  bool next_batch(Tensor<float>& inputs, Tensor<float>& targets) override;
  void reset() override;
  std::size_t size() const noexcept override;
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs
=======
  void set_seed(unsigned int seed) override;
>>>>>>> theirs

  [[nodiscard]] int input_dim() const noexcept { return input_dim_; }
  [[nodiscard]] int output_dim() const noexcept { return output_dim_; }
  [[nodiscard]] int batch_size() const noexcept { return batch_size_; }

 private:
  struct DatasetShape {
    int input_dim;
    int output_dim;
    int samples;
  };

  [[nodiscard]] DatasetShape infer_shape() const;
  void prepare_indices();

  Tensor<float> inputs_;
  Tensor<float> targets_;
  int input_dim_ = 0;
  int output_dim_ = 0;
  int total_samples_ = 0;
  int batch_size_ = 0;
  int cursor_ = 0;
  bool shuffle_ = true;
  std::mt19937 rng_;
  std::vector<int> indices_;
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

