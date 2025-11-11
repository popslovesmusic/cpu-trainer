#include "surrogate/model.hpp"

#include <memory>
#include <vector>

#include "surrogate/layers.hpp"

namespace sur {

template <class T>
class Tensor;

struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
};

Model::Model() : impl_(std::make_unique<Impl>()) {}
Model::~Model() = default;

void Model::add_layer(std::unique_ptr<Layer> layer) {
  impl_->layers.emplace_back(std::move(layer));
}

std::vector<Tensor<float>*> Model::parameters() {
  return {};
}

std::vector<Tensor<float>*> Model::gradients() {
  return {};
}

}  // namespace sur

