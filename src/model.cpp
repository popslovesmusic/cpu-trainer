#include "surrogate/model.hpp"

#include <memory>
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
#include <vector>

#include "surrogate/layers.hpp"

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
#include <stdexcept>
#include <utility>
#include <vector>

#include "surrogate/layers.hpp"
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
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
=======
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
  std::vector<Tensor<float>> activations;
>>>>>>> theirs
=======
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
  std::vector<Tensor<float>> activations;
>>>>>>> theirs
=======
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
  std::vector<Tensor<float>> activations;
>>>>>>> theirs
=======
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
  std::vector<Tensor<float>> activations;
>>>>>>> theirs
=======
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
  std::vector<Tensor<float>> activations;
>>>>>>> theirs
=======
struct Model::Impl {
  std::vector<std::unique_ptr<Layer>> layers;
  std::vector<Tensor<float>> activations;
>>>>>>> theirs
};

Model::Model() : impl_(std::make_unique<Impl>()) {}
Model::~Model() = default;

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
void Model::add_layer(std::unique_ptr<Layer> layer) {
  impl_->layers.emplace_back(std::move(layer));
}

std::vector<Tensor<float>*> Model::parameters() {
  return {};
}

std::vector<Tensor<float>*> Model::gradients() {
  return {};
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
void Model::add(std::unique_ptr<Layer> layer) {
  impl_->layers.emplace_back(std::move(layer));
}

Tensor<float> Model::forward(const Tensor<float>& input) {
  if (impl_->layers.empty()) {
    return input.as_contiguous();
  }

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  const Tensor<float>* current = &input;
  Tensor<float> activation;
  for (auto& layer : impl_->layers) {
    Tensor<float> next = layer->forward(*current);
    activation = std::move(next);
    current = &activation;
  }

  return activation;
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
  impl_->activations.resize(impl_->layers.size());
  const Tensor<float>* current = &input;
  for (std::size_t i = 0; i < impl_->layers.size(); ++i) {
    Tensor<float> next = impl_->layers[i]->forward(*current);
    impl_->activations[i] = std::move(next);
    current = &impl_->activations[i];
  }

  return impl_->activations.back().as_contiguous();
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
}

Tensor<float> Model::backward(const Tensor<float>& grad_output) {
  if (impl_->layers.empty()) {
    return grad_output.as_contiguous();
  }

  const Tensor<float>* current = &grad_output;
  Tensor<float> grad;
  for (auto it = impl_->layers.rbegin(); it != impl_->layers.rend(); ++it) {
    Tensor<float> next = (*it)->backward(*current);
    grad = std::move(next);
    current = &grad;
  }

  return grad;
}

std::vector<Tensor<float>*> Model::parameters() {
  std::vector<Tensor<float>*> params;
  for (auto& layer : impl_->layers) {
    auto layer_params = layer->params();
    for (auto* param : layer_params) {
      if (param) {
        params.push_back(param);
      }
    }
  }
  return params;
}

std::vector<Tensor<float>*> Model::gradients() {
  std::vector<Tensor<float>*> grads;
  for (auto& layer : impl_->layers) {
    auto layer_grads = layer->grads();
    for (auto* grad : layer_grads) {
      if (grad) {
        grads.push_back(grad);
      }
    }
  }
  return grads;
}

void Model::reserve_workspaces(int max_batch) {
  if (max_batch <= 0) {
    throw std::invalid_argument("Model::reserve_workspaces expects positive batch size");
  }
  for (auto& layer : impl_->layers) {
    layer->reserve_workspaces(max_batch);
  }
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
}

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
}

namespace detail {

std::vector<std::unique_ptr<Layer>>& access_layers(Model& model) {
  return model.impl_->layers;
}

const std::vector<std::unique_ptr<Layer>>& access_layers(const Model& model) {
  return model.impl_->layers;
}

}  // namespace detail

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
}  // namespace sur

