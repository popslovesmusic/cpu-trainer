#include "surrogate/mathkernels.hpp"

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
namespace sur::kernels {

void gemm_ref(int, int, int, const float*, int, const float*, int, float*, int, float) {}

void gemv_ref(int, int, const float*, int, const float*, int, float*, int, float) {}

float dot_ref(const float*, const float*, int) { return 0.0f; }

void axpy_ref(float, const float*, float*, int) {}

}  // namespace sur::kernels

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
#include <algorithm>
#include <cmath>
#include <limits>

namespace sur::kernels {

void gemm_ref(int M, int N, int K, const float* A, int lda, const float* B, int ldb, float* C, int ldc, float beta) {
  for (int m = 0; m < M; ++m) {
    for (int n = 0; n < N; ++n) {
      const int c_idx = m * ldc + n;
      float acc = 0.0f;
      for (int k = 0; k < K; ++k) {
        acc += A[m * lda + k] * B[k * ldb + n];
      }
      if (beta == 0.0f) {
        C[c_idx] = acc;
      } else {
        C[c_idx] = acc + beta * C[c_idx];
      }
    }
  }
}

void gemv_ref(int M, int N, const float* A, int lda, const float* x, int incx, float* y, int incy, float beta) {
  for (int m = 0; m < M; ++m) {
    const int y_idx = m * incy;
    float acc = 0.0f;
    for (int n = 0; n < N; ++n) {
      acc += A[m * lda + n] * x[n * incx];
    }
    if (beta == 0.0f) {
      y[y_idx] = acc;
    } else {
      y[y_idx] = acc + beta * y[y_idx];
    }
  }
}

float dot_ref(const float* x, const float* y, int n) {
  double acc = 0.0;
  for (int i = 0; i < n; ++i) {
    acc += static_cast<double>(x[i]) * static_cast<double>(y[i]);
  }
  return static_cast<float>(acc);
}

void axpy_ref(float a, const float* x, float* y, int n) {
  for (int i = 0; i < n; ++i) {
    y[i] += a * x[i];
  }
}

void relu_ref(float* y, const float* x, int n) {
  for (int i = 0; i < n; ++i) {
    y[i] = std::max(0.0f, x[i]);
  }
}

void relu_bw_ref(float* dx, const float* dy, const float* y, int n) {
  for (int i = 0; i < n; ++i) {
    dx[i] = y[i] > 0.0f ? dy[i] : 0.0f;
  }
}

void tanh_ref(float* y, const float* x, int n) {
  for (int i = 0; i < n; ++i) {
    y[i] = std::tanh(x[i]);
  }
}

void tanh_bw_ref(float* dx, const float* dy, const float* y, int n) {
  for (int i = 0; i < n; ++i) {
    dx[i] = dy[i] * (1.0f - y[i] * y[i]);
  }
}

void sigmoid_ref(float* y, const float* x, int n) {
  for (int i = 0; i < n; ++i) {
    const float v = x[i];
    if (v >= 0.0f) {
      const float exp_neg = std::exp(-v);
      y[i] = 1.0f / (1.0f + exp_neg);
    } else {
      const float exp_pos = std::exp(v);
      y[i] = exp_pos / (1.0f + exp_pos);
    }
  }
}

void sigmoid_bw_ref(float* dx, const float* dy, const float* y, int n) {
  for (int i = 0; i < n; ++i) {
    dx[i] = dy[i] * y[i] * (1.0f - y[i]);
  }
}

float reduce_sum_ref(const float* x, int n) {
  double sum = 0.0;
  double c = 0.0;
  for (int i = 0; i < n; ++i) {
    double y_val = static_cast<double>(x[i]) - c;
    double t = sum + y_val;
    c = (t - sum) - y_val;
    sum = t;
  }
  return static_cast<float>(sum);
}

}  // namespace sur::kernels
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
