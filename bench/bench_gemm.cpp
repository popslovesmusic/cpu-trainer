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
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "surrogate/mathkernels.hpp"

namespace {

using clock = std::chrono::high_resolution_clock;

template <class Fn>
double average_time(Fn&& fn, double minimum_seconds = 0.25) {
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

struct GemmConfig {
  int M;
  int N;
  int K;
};

struct ResultRow {
  std::string impl;
  int M;
  int N;
  int K;
  double time_ms;
  double gflops;
  double gbs;
};

std::vector<float> make_tensor(std::size_t size, std::uint32_t seed) {
  std::vector<float> data(size);
  std::mt19937 rng(seed);
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
  for (auto& v : data) {
    v = dist(rng);
  }
  return data;
}

ResultRow benchmark_impl(const GemmConfig& cfg, bool optimized) {
  const int lda = cfg.K;
  const int ldb = cfg.N;
  const int ldc = cfg.N;

  auto A = make_tensor(static_cast<std::size_t>(cfg.M) * lda, 1337u);
  auto B = make_tensor(static_cast<std::size_t>(cfg.K) * ldb, 7331u);
  std::vector<float> C(static_cast<std::size_t>(cfg.M) * ldc, 0.0f);

  const auto kernel = [&]() {
    if (optimized) {
      sur::kernels::gemm(cfg.M, cfg.N, cfg.K, A.data(), lda, B.data(), ldb, C.data(), ldc, 0.0f);
    } else {
      sur::kernels::gemm_ref(cfg.M, cfg.N, cfg.K, A.data(), lda, B.data(), ldb, C.data(), ldc, 0.0f);
    }
  };

  const double avg_seconds = average_time(kernel);
  const double flops = 2.0 * static_cast<double>(cfg.M) * static_cast<double>(cfg.N) *
                       static_cast<double>(cfg.K);
  const double gflops = (flops / avg_seconds) / 1e9;
  const double bytes = 4.0 * (static_cast<double>(cfg.M) * cfg.K +
                              static_cast<double>(cfg.K) * cfg.N +
                              static_cast<double>(cfg.M) * cfg.N);
  const double gbs = (bytes / avg_seconds) / 1e9;

  ResultRow row{};
  row.impl = optimized ? "avx2" : "ref";
  row.M = cfg.M;
  row.N = cfg.N;
  row.K = cfg.K;
  row.time_ms = avg_seconds * 1e3;
  row.gflops = gflops;
  row.gbs = gbs;
  return row;
}

void print_header() {
  std::cout << "GEMM Benchmark (beta=0)" << '\n';
  std::cout << "AVX2 available: " << (sur::kernels::has_avx2() ? "yes" : "no") << '\n';
  std::cout << std::left << std::setw(6) << "Impl" << std::right
            << std::setw(6) << "M" << std::setw(6) << "N" << std::setw(6) << "K"
            << std::setw(12) << "Time (ms)" << std::setw(12) << "GFLOP/s"
            << std::setw(12) << "GB/s" << '\n';
  std::cout << std::string(60, '-') << '\n';
}

void print_row(const ResultRow& row) {
  std::cout << std::left << std::setw(6) << row.impl << std::right << std::setw(6) << row.M
            << std::setw(6) << row.N << std::setw(6) << row.K << std::setw(12)
            << std::fixed << std::setprecision(3) << row.time_ms << std::setw(12)
            << std::setprecision(2) << row.gflops << std::setw(12) << row.gbs << '\n';
}

}  // namespace

int main() {
  const std::vector<GemmConfig> configs{{128, 128, 128}, {256, 256, 256}, {512, 512, 512},
                                        {1024, 1024, 1024}};

  print_header();
  for (const auto& cfg : configs) {
    const auto ref = benchmark_impl(cfg, false);
    print_row(ref);
    const auto opt = benchmark_impl(cfg, true);
    print_row(opt);
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

