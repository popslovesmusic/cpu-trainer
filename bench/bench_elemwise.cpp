<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
#include <cstdlib>

int main() { return EXIT_SUCCESS; }
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "surrogate/mathkernels.hpp"

namespace {

using clock = std::chrono::high_resolution_clock;

template <class Fn>
double average_time(Fn&& fn, double minimum_seconds = 0.2) {
  for (int i = 0; i < 2; ++i) {
    fn();
  }

  double elapsed = 0.0;
  int iterations = 0;
  do {
    const auto start = clock::now();
    fn();
    const auto end = clock::now();
    elapsed += std::chrono::duration<double>(end - start).count();
    ++iterations;
  } while (elapsed < minimum_seconds);

  return elapsed / static_cast<double>(iterations);
}

std::vector<float> make_input(std::size_t size) {
  std::vector<float> data(size);
  std::mt19937 rng(2023);
  std::uniform_real_distribution<float> dist(-3.0f, 3.0f);
  for (auto& v : data) {
    v = dist(rng);
  }
  return data;
}

struct ResultRow {
  std::string op;
  std::string impl;
  int n;
  double time_ms;
  double gbs;
};

ResultRow benchmark_activation(const char* name, int n, bool optimized,
                               void (*fast)(float*, const float*, int),
                               void (*ref)(float*, const float*, int)) {
  auto x = make_input(static_cast<std::size_t>(n));
  std::vector<float> y(static_cast<std::size_t>(n));

  const auto kernel = [&]() {
    if (optimized) {
      fast(y.data(), x.data(), n);
    } else {
      ref(y.data(), x.data(), n);
    }
  };

  const double avg_seconds = average_time(kernel);
  const double bytes = 2.0 * static_cast<double>(n) * sizeof(float);

  ResultRow row{};
  row.op = name;
  row.impl = optimized ? "avx2" : "ref";
  row.n = n;
  row.time_ms = avg_seconds * 1e3;
  row.gbs = (bytes / avg_seconds) / 1e9;
  return row;
}

void print_header() {
  std::cout << "Elementwise Activation Benchmark" << '\n';
  std::cout << "AVX2 available: " << (sur::kernels::has_avx2() ? "yes" : "no") << '\n';
  std::cout << std::left << std::setw(8) << "Op" << std::setw(6) << "Impl" << std::right
            << std::setw(12) << "N" << std::setw(12) << "Time (ms)" << std::setw(12)
            << "GB/s" << '\n';
  std::cout << std::string(50, '-') << '\n';
}

void print_row(const ResultRow& row) {
  std::cout << std::left << std::setw(8) << row.op << std::setw(6) << row.impl << std::right
            << std::setw(12) << row.n << std::setw(12) << std::fixed << std::setprecision(3)
            << row.time_ms << std::setw(12) << std::setprecision(2) << row.gbs << '\n';
}

}  // namespace

int main() {
  const std::vector<int> sizes{1 << 16, 1 << 20, 1 << 22};

  print_header();
  for (const int n : sizes) {
    print_row(benchmark_activation("relu", n, false, sur::kernels::relu, sur::kernels::relu_ref));
    print_row(benchmark_activation("relu", n, true, sur::kernels::relu, sur::kernels::relu_ref));
    print_row(benchmark_activation("tanh", n, false, sur::kernels::tanh, sur::kernels::tanh_ref));
    print_row(benchmark_activation("tanh", n, true, sur::kernels::tanh, sur::kernels::tanh_ref));
    print_row(benchmark_activation("sigmoid", n, false, sur::kernels::sigmoid, sur::kernels::sigmoid_ref));
    print_row(benchmark_activation("sigmoid", n, true, sur::kernels::sigmoid, sur::kernels::sigmoid_ref));
  }

  return 0;
}
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs

