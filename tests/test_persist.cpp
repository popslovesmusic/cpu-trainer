#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <vector>

#include "surrogate/layers.hpp"
#include "surrogate/model.hpp"
#include "surrogate/persist.hpp"
#include "surrogate/tensor.hpp"

namespace {

void expect(bool condition, const char* message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

std::filesystem::path make_temp_file() {
  const auto base = std::filesystem::temp_directory_path();
  const auto timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  return base / ("surrogate_persist-" + std::to_string(static_cast<long long>(timestamp)) + ".bin");
}

struct TempFileGuard {
  explicit TempFileGuard(std::filesystem::path path) : path_(std::move(path)) {}
  ~TempFileGuard() {
    if (!path_.empty()) {
      std::error_code ec;
      std::filesystem::remove(path_, ec);
    }
  }

  const std::filesystem::path& path() const { return path_; }

 private:
  std::filesystem::path path_;
};

void initialise_parameters(sur::Model& model, std::vector<std::vector<float>>& snapshot) {
  auto params = model.parameters();
  snapshot.clear();
  snapshot.reserve(params.size());
  float seed = 0.25f;
  for (auto* tensor : params) {
    const auto count = tensor->size();
    snapshot.emplace_back(count);
    auto& values = snapshot.back();
    for (std::size_t i = 0; i < count; ++i) {
      const float value = seed + static_cast<float>(i) * 0.5f;
      tensor->data()[i] = value;
      values[i] = value;
    }
    seed += 1.0f;
  }
}

void zero_parameters(sur::Model& model) {
  for (auto* tensor : model.parameters()) {
    tensor->zero_();
  }
}

void verify_parameters(sur::Model& model, const std::vector<std::vector<float>>& snapshot) {
  auto params = model.parameters();
  expect(params.size() == snapshot.size(), "Parameter count changed across persistence round-trip");
  for (std::size_t idx = 0; idx < params.size(); ++idx) {
    const auto* tensor = params[idx];
    const auto& expected = snapshot[idx];
    expect(tensor->size() == expected.size(), "Parameter size mismatch after load");
    for (std::size_t i = 0; i < expected.size(); ++i) {
      if (tensor->data()[i] != expected[i]) {
        throw std::runtime_error("Parameter value mismatch after load");
      }
    }
  }
}

void test_persistence_roundtrip() {
  sur::Model model;
  model.add(std::make_unique<sur::Dense>(3, 5));
  model.add(std::make_unique<sur::ReLU>());
  model.add(std::make_unique<sur::Dense>(5, 2));

  std::vector<std::vector<float>> snapshot;
  initialise_parameters(model, snapshot);

  sur::Tensor<float> input({3, 4});
  for (std::size_t i = 0; i < input.size(); ++i) {
    input.data()[i] = static_cast<float>(i) * 0.125f;
  }
  auto expected_output = model.forward(input).as_contiguous();

  TempFileGuard guard(make_temp_file());
  sur::save(model, guard.path());

  zero_parameters(model);
  auto degraded_output = model.forward(input).as_contiguous();
  bool differs = false;
  for (std::size_t i = 0; i < degraded_output.size(); ++i) {
    if (degraded_output.data()[i] != expected_output.data()[i]) {
      differs = true;
      break;
    }
  }
  expect(differs, "Model output unchanged after zeroing parameters");

  sur::load(model, guard.path());

  verify_parameters(model, snapshot);

  auto restored_output = model.forward(input).as_contiguous();
  expect(restored_output.size() == expected_output.size(), "Output size changed after load");
  for (std::size_t i = 0; i < restored_output.size(); ++i) {
    if (restored_output.data()[i] != expected_output.data()[i]) {
      throw std::runtime_error("Model output mismatch after persistence round-trip");
    }
  }
}

}  // namespace

int main() {
  try {
    test_persistence_roundtrip();
  } catch (const std::exception& e) {
    std::cerr << "Persistence tests failed: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
