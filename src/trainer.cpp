#include "surrogate/trainer.hpp"

#include <memory>

namespace sur {

struct Trainer::Impl {};

Trainer::Trainer(Model&, Loss&, Optimizer&, DataLoader&)
    : impl_(std::make_unique<Impl>()) {}

Trainer::~Trainer() = default;

void Trainer::train(const TrainerConfig&) {}

}  // namespace sur

