#pragma once

#include <cstddef>

namespace sur::kernels {

void gemm_ref(int M, int N, int K, const float* A, int lda, const float* B, int ldb, float* C, int ldc, float beta);
void gemv_ref(int M, int N, const float* A, int lda, const float* x, int incx, float* y, int incy, float beta);
float dot_ref(const float* x, const float* y, int n);
void axpy_ref(float a, const float* x, float* y, int n);

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
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
void relu_ref(float* y, const float* x, int n);
void relu_bw_ref(float* dx, const float* dy, const float* y, int n);
void tanh_ref(float* y, const float* x, int n);
void tanh_bw_ref(float* dx, const float* dy, const float* y, int n);
void sigmoid_ref(float* y, const float* x, int n);
void sigmoid_bw_ref(float* dx, const float* dy, const float* y, int n);

float reduce_sum_ref(const float* x, int n);

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
}  // namespace sur::kernels
>>>>>>> theirs
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
bool has_avx2();

void gemm(int M, int N, int K, const float* A, int lda, const float* B, int ldb, float* C, int ldc, float beta);
void gemv(int M, int N, const float* A, int lda, const float* x, int incx, float* y, int incy, float beta);
float dot(const float* x, const float* y, int n);
void axpy(float a, const float* x, float* y, int n);

void relu(float* y, const float* x, int n);
void relu_bw(float* dx, const float* dy, const float* y, int n);
void tanh(float* y, const float* x, int n);
void tanh_bw(float* dx, const float* dy, const float* y, int n);
void sigmoid(float* y, const float* x, int n);
void sigmoid_bw(float* dx, const float* dy, const float* y, int n);

float reduce_sum(const float* x, int n);

}  // namespace sur::kernels
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
