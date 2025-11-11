#pragma once

#include <cstddef>
#include <memory>

namespace sur {

class DataLoader;
class Loss;
class Model;
class Optimizer;

struct TrainerConfig {
  std::size_t epochs = 0;
  std::size_t batch_size = 0;
  int threads = 1;
  unsigned int seed = 42;
};

class Trainer {
 public:
  Trainer(Model& model, Loss& loss, Optimizer& optimizer, DataLoader& dataloader);
  ~Trainer();

  void train(const TrainerConfig& config);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace sur

