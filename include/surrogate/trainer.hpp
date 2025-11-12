#pragma once

#include <cstddef>
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
#include <memory>
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

namespace sur {

class DataLoader;
class Loss;
class Model;
class Optimizer;
<<<<<<< ours
<<<<<<< ours

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
struct TrainerConfig {
  std::size_t epochs = 0;
  std::size_t batch_size = 0;
  int threads = 1;
  unsigned int seed = 42;
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
class Logger;

>>>>>>> theirs
=======
class Logger;

>>>>>>> theirs
struct TrainConfig {
  int epochs = 1;
  int batch_size = 1;
  int threads = 1;
  bool deterministic = false;
  int log_every = 0;
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
  Logger* logger = nullptr;
>>>>>>> theirs
=======
  Logger* logger = nullptr;
>>>>>>> theirs
};

class Trainer {
 public:
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  Trainer(Model& model, Loss& loss, Optimizer& optimizer, DataLoader& dataloader);
  ~Trainer();

  void train(const TrainerConfig& config);

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
=======
>>>>>>> theirs
  Trainer() = default;

  void train(Model& model,
             Optimizer& optimizer,
             Loss& loss,
             DataLoader& dataloader,
             const TrainConfig& config);
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
};

}  // namespace sur

