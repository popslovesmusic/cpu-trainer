#pragma once

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
#include <filesystem>
>>>>>>> theirs
=======
#include <filesystem>
>>>>>>> theirs
=======
#include <filesystem>
>>>>>>> theirs
=======
#include <filesystem>
>>>>>>> theirs
#include <memory>
#include <vector>

namespace sur {

class Layer;

template <class T>
class Tensor;

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
class Model;

namespace detail {
std::vector<std::unique_ptr<Layer>>& access_layers(Model& model);
const std::vector<std::unique_ptr<Layer>>& access_layers(const Model& model);
}  // namespace detail

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
class Model {
 public:
  Model();
  ~Model();

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  void add_layer(std::unique_ptr<Layer> layer);
  std::vector<Tensor<float>*> parameters();
  std::vector<Tensor<float>*> gradients();

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
  void add(std::unique_ptr<Layer> layer);

  Tensor<float> forward(const Tensor<float>& input);
  Tensor<float> backward(const Tensor<float>& grad_output);

  std::vector<Tensor<float>*> parameters();
  std::vector<Tensor<float>*> gradients();

  void reserve_workspaces(int max_batch);

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
 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;

  friend void save(const Model& model, const std::filesystem::path& path);
  friend void load(Model& model, const std::filesystem::path& path);
  friend std::vector<std::unique_ptr<Layer>>& detail::access_layers(Model& model);
  friend const std::vector<std::unique_ptr<Layer>>& detail::access_layers(const Model& model);
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
};

}  // namespace sur

