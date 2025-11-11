#include "surrogate/mathkernels.hpp"

namespace sur::kernels {

void gemm_ref(int, int, int, const float*, int, const float*, int, float*, int, float) {}

void gemv_ref(int, int, const float*, int, const float*, int, float*, int, float) {}

float dot_ref(const float*, const float*, int) { return 0.0f; }

void axpy_ref(float, const float*, float*, int) {}

}  // namespace sur::kernels

