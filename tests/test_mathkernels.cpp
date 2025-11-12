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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
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
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "surrogate/mathkernels.hpp"

namespace {

bool nearly_equal(float a, float b, float eps = 1e-5f) {
  const float diff = std::fabs(a - b);
  const float scale = std::max(1.0f, std::max(std::fabs(a), std::fabs(b)));
  return diff <= eps * scale;
}

void expect_close(float a, float b, float eps = 1e-5f) {
  assert(nearly_equal(a, b, eps));
}

void test_gemm() {
  const int M = 3;
  const int N = 2;
  const int K = 4;
  const int lda = K;
  const int ldb = N;
  const int ldc = N;

  std::vector<float> A(M * lda);
  std::vector<float> B(K * ldb);
  std::vector<float> C(M * ldc, 1.0f);
  std::vector<float> expected = C;

  for (int m = 0; m < M; ++m) {
    for (int k = 0; k < K; ++k) {
      A[m * lda + k] = static_cast<float>((m + 1) * (k + 1));
    }
  }
  for (int k = 0; k < K; ++k) {
    for (int n = 0; n < N; ++n) {
      B[k * ldb + n] = static_cast<float>((k + 1) * (n + 2));
    }
  }

  const float beta = 0.5f;
  for (int m = 0; m < M; ++m) {
    for (int n = 0; n < N; ++n) {
      float acc = 0.0f;
      for (int k = 0; k < K; ++k) {
        acc += A[m * lda + k] * B[k * ldb + n];
      }
      expected[m * ldc + n] = acc + beta * expected[m * ldc + n];
    }
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  sur::kernels::gemm_ref(M, N, K, A.data(), lda, B.data(), ldb, C.data(), ldc, beta);

  for (int i = 0; i < M * ldc; ++i) {
    expect_close(C[i], expected[i]);
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
  {
    std::vector<float> C_ref = C;
    sur::kernels::gemm_ref(M, N, K, A.data(), lda, B.data(), ldb, C_ref.data(), ldc, beta);
    for (int i = 0; i < M * ldc; ++i) {
      expect_close(C_ref[i], expected[i]);
    }
  }

  {
    std::vector<float> C_fast = C;
    sur::kernels::gemm(M, N, K, A.data(), lda, B.data(), ldb, C_fast.data(), ldc, beta);
    for (int i = 0; i < M * ldc; ++i) {
      expect_close(C_fast[i], expected[i]);
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
  }
}

void test_gemv() {
  const int M = 4;
  const int N = 3;
  const int lda = N;
  const int incx = 1;
  const int incy = 1;

  std::vector<float> A(M * lda);
  std::vector<float> x(N * incx);
  std::vector<float> y(M * incy, 2.0f);
  std::vector<float> expected = y;

  for (int m = 0; m < M; ++m) {
    for (int n = 0; n < N; ++n) {
      A[m * lda + n] = static_cast<float>(m + n + 1);
    }
  }
  for (int n = 0; n < N; ++n) {
    x[n * incx] = static_cast<float>(n + 1);
  }

  const float beta = 0.25f;
  for (int m = 0; m < M; ++m) {
    float acc = 0.0f;
    for (int n = 0; n < N; ++n) {
      acc += A[m * lda + n] * x[n * incx];
    }
    expected[m * incy] = acc + beta * expected[m * incy];
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  sur::kernels::gemv_ref(M, N, A.data(), lda, x.data(), incx, y.data(), incy, beta);

  for (int i = 0; i < M * incy; ++i) {
    expect_close(y[i], expected[i]);
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
  {
    std::vector<float> y_ref = y;
    sur::kernels::gemv_ref(M, N, A.data(), lda, x.data(), incx, y_ref.data(), incy, beta);
    for (int i = 0; i < M * incy; ++i) {
      expect_close(y_ref[i], expected[i]);
    }
  }

  {
    std::vector<float> y_fast = y;
    sur::kernels::gemv(M, N, A.data(), lda, x.data(), incx, y_fast.data(), incy, beta);
    for (int i = 0; i < M * incy; ++i) {
      expect_close(y_fast[i], expected[i]);
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
  }
}

void test_dot_axpy() {
  const int n = 6;
  std::vector<float> x(n);
  std::vector<float> y(n);
  for (int i = 0; i < n; ++i) {
    x[i] = static_cast<float>(i + 1);
    y[i] = static_cast<float>(2 * (i + 1));
  }

  float expected_dot = 0.0f;
  for (int i = 0; i < n; ++i) {
    expected_dot += x[i] * y[i];
  }
  expect_close(sur::kernels::dot_ref(x.data(), y.data(), n), expected_dot);
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs
=======
  expect_close(sur::kernels::dot(x.data(), y.data(), n), expected_dot);
>>>>>>> theirs

  const float a = 0.5f;
  std::vector<float> expected = y;
  for (int i = 0; i < n; ++i) {
    expected[i] += a * x[i];
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  sur::kernels::axpy_ref(a, x.data(), y.data(), n);
  for (int i = 0; i < n; ++i) {
    expect_close(y[i], expected[i]);
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
  {
    std::vector<float> y_ref = y;
    sur::kernels::axpy_ref(a, x.data(), y_ref.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(y_ref[i], expected[i]);
    }
  }

  {
    std::vector<float> y_fast = y;
    sur::kernels::axpy(a, x.data(), y_fast.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(y_fast[i], expected[i]);
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
  }
}

void test_activations() {
  std::vector<float> x{-2.0f, -0.1f, 0.0f, 0.2f, 3.5f};
  const int n = static_cast<int>(x.size());
  std::vector<float> y(n);
  std::vector<float> dy{1.0f, -1.0f, 0.5f, -0.25f, 2.0f};
  std::vector<float> dx(n);

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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  sur::kernels::relu_ref(y.data(), x.data(), n);
  std::vector<float> expected_relu{0.0f, 0.0f, 0.0f, 0.2f, 3.5f};
  for (int i = 0; i < n; ++i) {
    expect_close(y[i], expected_relu[i]);
  }

  sur::kernels::relu_bw_ref(dx.data(), dy.data(), y.data(), n);
  std::vector<float> expected_relu_bw{0.0f, 0.0f, 0.0f, -0.25f, 2.0f};
  for (int i = 0; i < n; ++i) {
    expect_close(dx[i], expected_relu_bw[i]);
  }

  sur::kernels::tanh_ref(y.data(), x.data(), n);
  for (int i = 0; i < n; ++i) {
    expect_close(y[i], std::tanh(x[i]));
  }

  sur::kernels::tanh_bw_ref(dx.data(), dy.data(), y.data(), n);
  for (int i = 0; i < n; ++i) {
    float expected = dy[i] * (1.0f - y[i] * y[i]);
    expect_close(dx[i], expected);
  }

  sur::kernels::sigmoid_ref(y.data(), x.data(), n);
  for (int i = 0; i < n; ++i) {
    float expected = 1.0f / (1.0f + std::exp(-x[i]));
    expect_close(y[i], expected);
  }

  sur::kernels::sigmoid_bw_ref(dx.data(), dy.data(), y.data(), n);
  for (int i = 0; i < n; ++i) {
    float expected = dy[i] * y[i] * (1.0f - y[i]);
    expect_close(dx[i], expected);
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
  std::vector<float> expected_relu{0.0f, 0.0f, 0.0f, 0.2f, 3.5f};
  {
    std::vector<float> y_ref(n);
    sur::kernels::relu_ref(y_ref.data(), x.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(y_ref[i], expected_relu[i]);
    }

    std::vector<float> y_fast(n);
    sur::kernels::relu(y_fast.data(), x.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(y_fast[i], expected_relu[i]);
    }

    std::vector<float> dx_ref(n);
    sur::kernels::relu_bw_ref(dx_ref.data(), dy.data(), y_ref.data(), n);
    std::vector<float> expected_relu_bw{0.0f, 0.0f, 0.0f, -0.25f, 2.0f};
    for (int i = 0; i < n; ++i) {
      expect_close(dx_ref[i], expected_relu_bw[i]);
    }

    std::vector<float> dx_fast(n);
    sur::kernels::relu_bw(dx_fast.data(), dy.data(), y_fast.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(dx_fast[i], expected_relu_bw[i]);
    }
  }

  {
    std::vector<float> y_ref(n);
    sur::kernels::tanh_ref(y_ref.data(), x.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(y_ref[i], std::tanh(x[i]));
    }

    std::vector<float> y_fast(n);
    sur::kernels::tanh(y_fast.data(), x.data(), n);
    for (int i = 0; i < n; ++i) {
      expect_close(y_fast[i], std::tanh(x[i]));
    }

    std::vector<float> dx_ref(n);
    sur::kernels::tanh_bw_ref(dx_ref.data(), dy.data(), y_ref.data(), n);
    for (int i = 0; i < n; ++i) {
      float expected = dy[i] * (1.0f - y_ref[i] * y_ref[i]);
      expect_close(dx_ref[i], expected);
    }

    std::vector<float> dx_fast(n);
    sur::kernels::tanh_bw(dx_fast.data(), dy.data(), y_fast.data(), n);
    for (int i = 0; i < n; ++i) {
      float expected = dy[i] * (1.0f - y_fast[i] * y_fast[i]);
      expect_close(dx_fast[i], expected);
    }
  }

  {
    std::vector<float> y_ref(n);
    sur::kernels::sigmoid_ref(y_ref.data(), x.data(), n);
    for (int i = 0; i < n; ++i) {
      float expected = 1.0f / (1.0f + std::exp(-x[i]));
      expect_close(y_ref[i], expected);
    }

    std::vector<float> y_fast(n);
    sur::kernels::sigmoid(y_fast.data(), x.data(), n);
    for (int i = 0; i < n; ++i) {
      float expected = 1.0f / (1.0f + std::exp(-x[i]));
      expect_close(y_fast[i], expected);
    }

    std::vector<float> dx_ref(n);
    sur::kernels::sigmoid_bw_ref(dx_ref.data(), dy.data(), y_ref.data(), n);
    for (int i = 0; i < n; ++i) {
      float expected = dy[i] * y_ref[i] * (1.0f - y_ref[i]);
      expect_close(dx_ref[i], expected);
    }

    std::vector<float> dx_fast(n);
    sur::kernels::sigmoid_bw(dx_fast.data(), dy.data(), y_fast.data(), n);
    for (int i = 0; i < n; ++i) {
      float expected = dy[i] * y_fast[i] * (1.0f - y_fast[i]);
      expect_close(dx_fast[i], expected);
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
  }
}

void test_reduce_sum() {
  std::vector<float> values{1.0f, -2.5f, 3.0f, 4.25f, -1.75f};
  float expected = 0.0f;
  for (float v : values) {
    expected += v;
  }
  expect_close(sur::kernels::reduce_sum_ref(values.data(), static_cast<int>(values.size())), expected);
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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
=======
  expect_close(sur::kernels::reduce_sum(values.data(), static_cast<int>(values.size())), expected);
>>>>>>> theirs
}

}  // namespace

int main() {
  test_gemm();
  test_gemv();
  test_dot_axpy();
  test_activations();
  test_reduce_sum();
  return 0;
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
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
