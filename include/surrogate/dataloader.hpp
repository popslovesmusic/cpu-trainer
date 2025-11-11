#pragma once

#include <cstddef>

namespace sur {

template <class T>
class Tensor;

template <class T>
class TensorView;

class DataLoader {
 public:
  virtual ~DataLoader() = default;
  virtual bool next_batch(TensorView<float>& inputs, TensorView<float>& targets) = 0;
  virtual void reset() = 0;
  virtual std::size_t size() const noexcept = 0;
};

}  // namespace sur

