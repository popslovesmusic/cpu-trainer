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
<<<<<<< ours
namespace sur::kernels {

// AVX2 implementations will be provided in later phases.
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
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

#include "surrogate/aligned_alloc.hpp"
#include "surrogate/config.hpp"

#if defined(_MSC_VER)
#include <intrin.h>
#endif
#if defined(__AVX2__)
#include <immintrin.h>
#endif

namespace sur::kernels {

namespace detail {

inline constexpr float kExpHi = 88.3762626647949f;
inline constexpr float kExpLo = -88.3762626647949f;

#if defined(__AVX2__)

inline __m256 fmadd(__m256 a, __m256 b, __m256 c) {
#if defined(__FMA__)
  return _mm256_fmadd_ps(a, b, c);
#else
  return _mm256_add_ps(_mm256_mul_ps(a, b), c);
#endif
}

inline __m256 exp256_ps(__m256 x) {
  const __m256 max = _mm256_set1_ps(kExpHi);
  const __m256 min = _mm256_set1_ps(kExpLo);
  x = _mm256_max_ps(_mm256_min_ps(x, max), min);

  const __m256 log2e = _mm256_set1_ps(1.44269504088896341f);
  const __m256 c1 = _mm256_set1_ps(0.693359375f);
  const __m256 c2 = _mm256_set1_ps(-2.12194440e-4f);

  __m256 fx = fmadd(x, log2e, _mm256_set1_ps(0.5f));
  __m256 floor_fx = _mm256_floor_ps(fx);
  __m256i emm0 = _mm256_cvttps_epi32(floor_fx);

  __m256 tmp = _mm256_mul_ps(floor_fx, c1);
  __m256 z = _mm256_mul_ps(floor_fx, c2);
  x = _mm256_sub_ps(x, tmp);
  x = _mm256_sub_ps(x, z);

  const __m256 p0 = _mm256_set1_ps(1.9875691500e-4f);
  const __m256 p1 = _mm256_set1_ps(1.3981999507e-3f);
  const __m256 p2 = _mm256_set1_ps(8.3334519073e-3f);
  const __m256 p3 = _mm256_set1_ps(4.1665795894e-2f);
  const __m256 p4 = _mm256_set1_ps(1.6666665459e-1f);
  const __m256 p5 = _mm256_set1_ps(5.0000001201e-1f);

  __m256 y = fmadd(p0, x, p1);
  y = fmadd(y, x, p2);
  y = fmadd(y, x, p3);
  y = fmadd(y, x, p4);
  y = fmadd(y, x, p5);
  y = fmadd(y, x, _mm256_set1_ps(1.0f));

  emm0 = _mm256_add_epi32(emm0, _mm256_set1_epi32(0x7f));
  emm0 = _mm256_slli_epi32(emm0, 23);
  __m256 pow2n = _mm256_castsi256_ps(emm0);
  return _mm256_mul_ps(y, pow2n);
}

inline float horizontal_sum(__m256 v) {
  __m128 vlow = _mm256_castps256_ps128(v);
  __m128 vhigh = _mm256_extractf128_ps(v, 1);
  vlow = _mm_add_ps(vlow, vhigh);
  __m128 shuf = _mm_movehdup_ps(vlow);
  vlow = _mm_add_ps(vlow, shuf);
  shuf = _mm_movehl_ps(shuf, vlow);
  vlow = _mm_add_ss(vlow, shuf);
  return _mm_cvtss_f32(vlow);
}

inline void scale_matrix(float* C, int M, int N, int ldc, float beta) {
  if (beta == 1.0f) {
    return;
  }
  if (beta == 0.0f) {
    for (int m = 0; m < M; ++m) {
      std::memset(C + m * ldc, 0, sizeof(float) * N);
    }
    return;
  }
  for (int m = 0; m < M; ++m) {
    float* row = C + m * ldc;
    for (int n = 0; n < N; ++n) {
      row[n] *= beta;
    }
  }
}

inline void pack_a_panel(float* dst, const float* src, int lda, int rows, int cols) {
  for (int k = 0; k < cols; ++k) {
    const float* col = src + k;
    float* out = dst + k * rows;
    for (int r = 0; r < rows; ++r) {
      out[r] = col[r * lda];
    }
  }
}

inline void pack_b_panel(float* dst, const float* src, int ldb, int rows, int cols) {
  for (int k = 0; k < rows; ++k) {
    const float* row = src + k * ldb;
    std::memcpy(dst + k * cols, row, sizeof(float) * cols);
  }
}

inline void micro_kernel_8x4(const float* packed_a, const float* packed_b, float* C, int ldc,
                             int kc, int stride_a, int stride_b) {
  __m256 c0 = _mm256_setzero_ps();
  __m256 c1 = _mm256_setzero_ps();
  __m256 c2 = _mm256_setzero_ps();
  __m256 c3 = _mm256_setzero_ps();

  for (int p = 0; p < kc; ++p) {
    const float* a_panel = packed_a + p * stride_a;
    const float* b_panel = packed_b + p * stride_b;
    __m256 a_vec = _mm256_loadu_ps(a_panel);
    __m256 b0 = _mm256_set1_ps(b_panel[0]);
    __m256 b1 = _mm256_set1_ps(b_panel[1]);
    __m256 b2 = _mm256_set1_ps(b_panel[2]);
    __m256 b3 = _mm256_set1_ps(b_panel[3]);
    c0 = fmadd(a_vec, b0, c0);
    c1 = fmadd(a_vec, b1, c1);
    c2 = fmadd(a_vec, b2, c2);
    c3 = fmadd(a_vec, b3, c3);
  }

  alignas(32) float buf0[8];
  alignas(32) float buf1[8];
  alignas(32) float buf2[8];
  alignas(32) float buf3[8];
  _mm256_store_ps(buf0, c0);
  _mm256_store_ps(buf1, c1);
  _mm256_store_ps(buf2, c2);
  _mm256_store_ps(buf3, c3);

  float* crow = C;
  for (int r = 0; r < 8; ++r) {
    crow[0] += buf0[r];
    crow[1] += buf1[r];
    crow[2] += buf2[r];
    crow[3] += buf3[r];
    crow += ldc;
  }
}

inline void accumulate_scalar_row(const float* packed_a, const float* packed_b, float* C_block, int ldc,
                                  int row, int col_start, int col_count, int kc, int stride_a, int stride_b) {
  float* crow = C_block + row * ldc + col_start;
  for (int c = 0; c < col_count; ++c) {
    float acc = 0.0f;
    for (int p = 0; p < kc; ++p) {
      acc += packed_a[p * stride_a + row] * packed_b[p * stride_b + col_start + c];
    }
    crow[c] += acc;
  }
}

inline void compute_block(int mb, int nb, int kb, const float* packed_a, const float* packed_b,
                          float* C_block, int ldc) {
  const int stride_a = mb;
  const int stride_b = nb;

  const int m8 = (mb / 8) * 8;
  const int n4 = (nb / 4) * 4;

  for (int j = 0; j < n4; j += 4) {
    for (int i = 0; i < m8; i += 8) {
      micro_kernel_8x4(packed_a + i, packed_b + j, C_block + i * ldc + j, ldc, kb, stride_a, stride_b);
    }
    for (int i = m8; i < mb; ++i) {
      accumulate_scalar_row(packed_a, packed_b, C_block, ldc, i, j, 4, kb, stride_a, stride_b);
    }
  }

  if (n4 < nb) {
    const int rem_cols = nb - n4;
    for (int i = 0; i < mb; ++i) {
      accumulate_scalar_row(packed_a, packed_b, C_block, ldc, i, n4, rem_cols, kb, stride_a, stride_b);
    }
  }
}

inline void gemm_avx2_impl(int M, int N, int K, const float* A, int lda, const float* B, int ldb,
                           float* C, int ldc, float beta) {
  scale_matrix(C, M, N, ldc, beta);

  if (M == 0 || N == 0 || K == 0) {
    return;
  }

  const int MC = sur::config::kGemmBlockMC;
  const int NC = sur::config::kGemmBlockNC;
  const int KC = sur::config::kGemmBlockKC;

  const std::size_t pack_a_size = static_cast<std::size_t>(MC) * KC;
  const std::size_t pack_b_size = static_cast<std::size_t>(KC) * NC;

  auto deleter = [](float* ptr) { sur::aligned_free(ptr); };
  std::unique_ptr<float, decltype(deleter)> pack_a_buf(static_cast<float*>(sur::aligned_malloc(sizeof(float) * pack_a_size)), deleter);
  std::unique_ptr<float, decltype(deleter)> pack_b_buf(static_cast<float*>(sur::aligned_malloc(sizeof(float) * pack_b_size)), deleter);

  for (int jc = 0; jc < N; jc += NC) {
    const int nb = std::min(NC, N - jc);
    for (int kc = 0; kc < K; kc += KC) {
      const int kb = std::min(KC, K - kc);
      pack_b_panel(pack_b_buf.get(), B + kc * ldb + jc, ldb, kb, nb);
      for (int ic = 0; ic < M; ic += MC) {
        const int mb = std::min(MC, M - ic);
        pack_a_panel(pack_a_buf.get(), A + ic * lda + kc, lda, mb, kb);
        compute_block(mb, nb, kb, pack_a_buf.get(), pack_b_buf.get(), C + ic * ldc + jc, ldc);
      }
    }
  }
}

inline void gemv_avx2_impl(int M, int N, const float* A, int lda, const float* x, float* y, float beta) {
  if (beta == 0.0f) {
    std::fill_n(y, M, 0.0f);
  } else if (beta != 1.0f) {
    for (int i = 0; i < M; ++i) {
      y[i] *= beta;
    }
  }

  for (int m = 0; m < M; ++m) {
    const float* a_row = A + m * lda;
    __m256 acc = _mm256_setzero_ps();
    int k = 0;
    for (; k + 7 < N; k += 8) {
      __m256 va = _mm256_loadu_ps(a_row + k);
      __m256 vx = _mm256_loadu_ps(x + k);
      acc = fmadd(va, vx, acc);
    }
    float sum = horizontal_sum(acc);
    for (; k < N; ++k) {
      sum += a_row[k] * x[k];
    }
    y[m] += sum;
  }
}

inline float dot_avx2_impl(const float* x, const float* y, int n) {
  __m256 acc = _mm256_setzero_ps();
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vx = _mm256_loadu_ps(x + i);
    __m256 vy = _mm256_loadu_ps(y + i);
    acc = fmadd(vx, vy, acc);
  }
  float result = horizontal_sum(acc);
  for (; i < n; ++i) {
    result += x[i] * y[i];
  }
  return result;
}

inline void axpy_avx2_impl(float a, const float* x, float* y, int n) {
  __m256 alpha = _mm256_set1_ps(a);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vx = _mm256_loadu_ps(x + i);
    __m256 vy = _mm256_loadu_ps(y + i);
    vy = fmadd(alpha, vx, vy);
    _mm256_storeu_ps(y + i, vy);
  }
  for (; i < n; ++i) {
    y[i] += a * x[i];
  }
}

inline void relu_avx2_impl(float* y, const float* x, int n) {
  const __m256 zero = _mm256_set1_ps(0.0f);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vx = _mm256_loadu_ps(x + i);
    __m256 vy = _mm256_max_ps(vx, zero);
    _mm256_storeu_ps(y + i, vy);
  }
  for (; i < n; ++i) {
    y[i] = std::max(x[i], 0.0f);
  }
}

inline void relu_bw_avx2_impl(float* dx, const float* dy, const float* y, int n) {
  const __m256 zero = _mm256_set1_ps(0.0f);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vy = _mm256_loadu_ps(y + i);
    __m256 vdy = _mm256_loadu_ps(dy + i);
    __m256 mask = _mm256_cmp_ps(vy, zero, _CMP_GT_OQ);
    __m256 vdx = _mm256_and_ps(vdy, mask);
    _mm256_storeu_ps(dx + i, vdx);
  }
  for (; i < n; ++i) {
    dx[i] = y[i] > 0.0f ? dy[i] : 0.0f;
  }
}

inline void tanh_avx2_impl(float* y, const float* x, int n) {
  const __m256 two = _mm256_set1_ps(2.0f);
  const __m256 one = _mm256_set1_ps(1.0f);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vx = _mm256_loadu_ps(x + i);
    __m256 v2x = _mm256_mul_ps(vx, two);
    __m256 e = exp256_ps(v2x);
    __m256 num = _mm256_sub_ps(e, one);
    __m256 den = _mm256_add_ps(e, one);
    __m256 vt = _mm256_div_ps(num, den);
    _mm256_storeu_ps(y + i, vt);
  }
  for (; i < n; ++i) {
    y[i] = std::tanh(x[i]);
  }
}

inline void tanh_bw_avx2_impl(float* dx, const float* dy, const float* y, int n) {
  const __m256 one = _mm256_set1_ps(1.0f);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vy = _mm256_loadu_ps(y + i);
    __m256 vdy = _mm256_loadu_ps(dy + i);
    __m256 vy2 = _mm256_mul_ps(vy, vy);
    __m256 grad = _mm256_sub_ps(one, vy2);
    __m256 vdx = _mm256_mul_ps(vdy, grad);
    _mm256_storeu_ps(dx + i, vdx);
  }
  for (; i < n; ++i) {
    dx[i] = dy[i] * (1.0f - y[i] * y[i]);
  }
}

inline void sigmoid_avx2_impl(float* y, const float* x, int n) {
  const __m256 one = _mm256_set1_ps(1.0f);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vx = _mm256_loadu_ps(x + i);
    __m256 neg = _mm256_sub_ps(_mm256_setzero_ps(), vx);
    __m256 exp_neg = exp256_ps(neg);
    __m256 denom = _mm256_add_ps(one, exp_neg);
    __m256 vs = _mm256_div_ps(one, denom);
    _mm256_storeu_ps(y + i, vs);
  }
  for (; i < n; ++i) {
    y[i] = 1.0f / (1.0f + std::exp(-x[i]));
  }
}

inline void sigmoid_bw_avx2_impl(float* dx, const float* dy, const float* y, int n) {
  const __m256 one = _mm256_set1_ps(1.0f);
  int i = 0;
  for (; i + 7 < n; i += 8) {
    __m256 vy = _mm256_loadu_ps(y + i);
    __m256 vdy = _mm256_loadu_ps(dy + i);
    __m256 one_minus = _mm256_sub_ps(one, vy);
    __m256 grad = _mm256_mul_ps(vy, one_minus);
    __m256 vdx = _mm256_mul_ps(vdy, grad);
    _mm256_storeu_ps(dx + i, vdx);
  }
  for (; i < n; ++i) {
    dx[i] = dy[i] * y[i] * (1.0f - y[i]);
  }
}

inline float reduce_sum_avx2_impl(const float* x, int n) {
  __m256 acc = _mm256_setzero_ps();
  int i = 0;
  for (; i + 7 < n; i += 8) {
    acc = _mm256_add_ps(acc, _mm256_loadu_ps(x + i));
  }
  float sum = horizontal_sum(acc);
  for (; i < n; ++i) {
    sum += x[i];
  }
  return sum;
}

#endif  // defined(__AVX2__)

inline bool runtime_has_avx2() {
#if defined(__AVX2__)
  return true;
#elif defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#if defined(__GNUC__)
  return __builtin_cpu_supports("avx2");
#elif defined(_MSC_VER)
  int info[4];
  __cpuid(info, 0);
  if (info[0] < 7) {
    return false;
  }
  __cpuidex(info, 7, 0);
  return (info[1] & (1 << 5)) != 0;
#else
  return false;
#endif
#else
  return false;
#endif
}

}  // namespace detail

bool has_avx2() { return detail::runtime_has_avx2(); }

void gemm(int M, int N, int K, const float* A, int lda, const float* B, int ldb, float* C, int ldc, float beta) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::gemm_avx2_impl(M, N, K, A, lda, B, ldb, C, ldc, beta);
    return;
  }
#endif
  gemm_ref(M, N, K, A, lda, B, ldb, C, ldc, beta);
}

void gemv(int M, int N, const float* A, int lda, const float* x, int incx, float* y, int incy, float beta) {
  if (incx != 1 || incy != 1) {
    gemv_ref(M, N, A, lda, x, incx, y, incy, beta);
    return;
  }
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::gemv_avx2_impl(M, N, A, lda, x, y, beta);
    return;
  }
#endif
  gemv_ref(M, N, A, lda, x, incx, y, incy, beta);
}

float dot(const float* x, const float* y, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    return detail::dot_avx2_impl(x, y, n);
  }
#endif
  return dot_ref(x, y, n);
}

void axpy(float a, const float* x, float* y, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::axpy_avx2_impl(a, x, y, n);
    return;
  }
#endif
  axpy_ref(a, x, y, n);
}

void relu(float* y, const float* x, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::relu_avx2_impl(y, x, n);
    return;
  }
#endif
  relu_ref(y, x, n);
}

void relu_bw(float* dx, const float* dy, const float* y, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::relu_bw_avx2_impl(dx, dy, y, n);
    return;
  }
#endif
  relu_bw_ref(dx, dy, y, n);
}

void tanh(float* y, const float* x, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::tanh_avx2_impl(y, x, n);
    return;
  }
#endif
  tanh_ref(y, x, n);
}

void tanh_bw(float* dx, const float* dy, const float* y, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::tanh_bw_avx2_impl(dx, dy, y, n);
    return;
  }
#endif
  tanh_bw_ref(dx, dy, y, n);
}

void sigmoid(float* y, const float* x, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::sigmoid_avx2_impl(y, x, n);
    return;
  }
#endif
  sigmoid_ref(y, x, n);
}

void sigmoid_bw(float* dx, const float* dy, const float* y, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    detail::sigmoid_bw_avx2_impl(dx, dy, y, n);
    return;
  }
#endif
  sigmoid_bw_ref(dx, dy, y, n);
}

float reduce_sum(const float* x, int n) {
#if defined(__AVX2__)
  if (detail::runtime_has_avx2()) {
    return detail::reduce_sum_avx2_impl(x, n);
  }
#endif
  return reduce_sum_ref(x, n);
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

}  // namespace sur::kernels

