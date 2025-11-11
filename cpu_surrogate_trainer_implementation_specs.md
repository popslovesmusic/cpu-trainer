# CPU Surrogate Trainer — Implementation Specs

A step‑by‑step, test‑first plan to build a **pure C++ (C++20)** surrogate‑model training framework optimized for **high‑core CPUs with OpenMP + AVX2**. Follow phases in order; each phase has *deliverables*, *acceptance tests*, and *notes*.

---

## 0) Project Skeleton & Conventions

**Goals**
- Establish directory layout, coding standards, toolchain flags, and CI/test scaffolding.

**Directory Tree**
```
surrogate-cpu/
  CMakeLists.txt
  cmake/
    toolchains.cmake
    presets.cmake
  include/
    surrogate/
      tensor.hpp
      tensor_view.hpp
      aligned_alloc.hpp
      mathkernels.hpp
      layers.hpp
      loss.hpp
      optimizer.hpp
      model.hpp
      trainer.hpp
      dataloader.hpp
      logging.hpp
      persist.hpp
      config.hpp
  src/
    mathkernels_ref.cpp
    mathkernels_avx2.cpp
    layers_dense.cpp
    layers_activation.cpp
    loss.cpp
    optimizer_sgd.cpp
    optimizer_adam.cpp
    model.cpp
    trainer.cpp
    dataloader.cpp
    logging.cpp
    persist.cpp
  tests/
    test_tensor.cpp
    test_mathkernels.cpp
    test_layers.cpp
    test_gradcheck.cpp
    test_trainer.cpp
  bench/
    bench_gemm.cpp
    bench_elemwise.cpp
  examples/
    example_mlp.cpp
  tools/
    gen_synthetic.cpp
  .github/workflows/ci.yml
```

**Coding Standards**
- C++20, `-Wall -Wextra -Wpedantic`, no RTTI in hot path, `noexcept` where possible.
- Public headers under `include/surrogate`. All AVX2 code isolated behind `#if defined(__AVX2__)` guards.
- Namespaces: `sur::` for library, `sur::detail` for internals.

**Build Presets** (CMake presets names)
- `Debug-Sanitize`: `-fsanitize=address,undefined` (no AVX2)
- `Release-AVX2`: `-O3 -mavx2 -mfma -fopenmp`
- `Release-Portable`: `-O3 -fopenmp` (no AVX2)

**Acceptance**
- `cmake --list-presets` shows the three presets and the CI workflow builds them.

---

## 1) Aligned Memory & Tensor Core

**Goal**
- Provide cache‑friendly, 32‑byte aligned contiguous buffers with views and strides.

**Spec**
- `aligned_alloc.hpp`
  - `void* aligned_malloc(std::size_t bytes, std::size_t alignment=32)`
  - `void  aligned_free(void* p)`
- `tensor.hpp`
  - `template<class T=float> class Tensor` with fields: `T* data_`, `std::size_t size_`, `std::array<int,4> shape`, `std::array<int,4> strides`, `std::size_t offset_`, `bool row_major=true`.
  - Constructors: own/allocate; from external buffer (non‑owning flag false prohibited – use view); move‑only.
  - Methods: `data()`, `size()`, `shape()`, `strides()`, `fill`, `zero_`, `as_contiguous()`, `reshape`, `slice(beg, end) -> TensorView<T>`.
- `tensor_view.hpp`
  - Non‑owning view: pointer + shape + strides + offset; no allocation.

**Behavioral Notes**
- Default layout row‑major. Strides explicit. Offset allows subviews without copies.
- Allocation always 32‑byte aligned. Tail loads handled by AVX2 kernels with fallback scalar loop.

**Acceptance**
- `tests/test_tensor.cpp`: construct, slice, reshape, verify strides and element access; alignment test (`reinterpret_cast<uintptr_t>(data)%32==0`).

---

## 2) MathKernels — Reference Implementations

**Goal**
- Provide correctness baselines for kernels; used by unit tests and to compare AVX2.

**Spec**
- `mathkernels.hpp` (declares) and `mathkernels_ref.cpp` (implements):
  - `void gemm_ref(int M,int N,int K,const float* A,int lda,const float* B,int ldb,float* C,int ldc,float beta)` (row‑major, C := A*B + beta*C)
  - `void gemv_ref(...)`, `float dot_ref(const float*,const float*,int)`, `void axpy_ref(float a,const float* x,float* y,int n)`, `void relu_ref(float* y,const float* x,int n)`, `void relu_bw_ref(float* dx,const float* dy,const float* y,int n)`, similarly `tanh_ref/sigmoid_ref (+bw)`.
- Reduction helpers: `float reduce_sum_ref(const float* x,int n)` with Kahan compensation (optional).

**Acceptance**
- `tests/test_mathkernels.cpp` validates ops on small random tensors vs straightforward loops.

---

## 3) MathKernels — AVX2 + Blocked GEMM

**Goal**
- High‑throughput kernels using 8‑wide AVX2 and cache blocking.

**Spec**
- `mathkernels_avx2.cpp` provides:
  - Blocked GEMM with tunables: `MC=256, NC=128, KC=256` (configurable via `config.hpp`).
  - Inner micro‑kernel: 8×4 (M×N) float32 tiles using `_mm256_fmadd_ps`.
  - Packing of A (row panels) and B (col panels). Prefetch next panels under `#ifdef SUR_PREFETCH`.
  - Tails: handle `K%8`, `M%8`, `N%4` via masked or scalar remainder loops.
  - Elementwise AVX2: `relu`, `tanh` (via `tanhf` fallback chunking), `sigmoid` (rational/poly approx with final clamp).
- Runtime selection: if `__AVX2__` not present, fall back to ref.

**Acceptance**
- Unit tests pass vs reference within tolerance (`1e-5`).
- `bench/bench_gemm.cpp` reports ≥ 70% of STREAM triad bandwidth or ≥ 40% of peak FMA GFLOP/s on a modern CPU (tunable target).

**Notes**
- Keep micro‑kernel in `detail` namespace; mark `inline`/`__attribute__((always_inline))` or MSVC `__forceinline`.

---

## 4) Layers

**Goal**
- Dense and activation layers with zero allocations in hot path.

**Spec**
- `layers.hpp`
  - Abstract base `struct Layer` with: `virtual Tensor forward(const Tensor& x)=0; virtual Tensor backward(const Tensor& dy)=0; virtual std::span<Tensor*> params(); virtual std::span<Tensor*> grads(); virtual void reserve_workspaces(int max_batch); virtual ~Layer()=default;`
  - `class Dense`: parameters `W[out,in]`, `b[out]`; caches `Xbatch`, `Ybatch`.
    - fwd: `Y = W * X + b` (GEMM or GEMV if batch=1)
    - bwd: `dW += dY * X^T`, `db += reduce(dY)`, `dX = W^T * dY`.
  - `class ReLU/Tanh/Sigmoid`: elementwise; store `Y` (or mask for ReLU) to backprop.
- `reserve_workspaces(int max_batch)` allocates per‑layer scratch once.

**Acceptance**
- `tests/test_layers.cpp`: forward/backward agree with numerical gradients on tiny nets.

---

## 5) Loss Functions

**Goal**
- MSE and MAE with vectorized gradient.

**Spec**
- `loss.hpp`/`loss.cpp`
  - `struct Loss { virtual float compute(const Tensor& pred,const Tensor& tgt)=0; virtual Tensor gradient(const Tensor& pred,const Tensor& tgt)=0; virtual ~Loss()=default; }`
  - `struct MSE : Loss { ... }`: `grad = 2*(pred-tgt)/N` (AVX2 elemwise); `compute` may use compensated sum in Debug.

**Acceptance**
- Unit tests vs reference implementations.

---

## 6) Optimizers

**Goal**
- SGD(+momentum, weight_decay) and Adam with contiguous states.

**Spec**
- `optimizer.hpp`/`optimizer_*.cpp`
  - Base: `struct Optimizer { virtual void step(std::span<Tensor*>, std::span<Tensor*>)=0; virtual void set_lr(float)=0; virtual ~Optimizer()=default; }`
  - `struct SGD { float lr, momentum, wd; /* states: v */ }`
  - `struct Adam { float lr, b1,b2,eps; /* states: m,v,t */ }`
  - States allocated contiguous per‑param to improve locality. Elementwise updates AVX2.

**Acceptance**
- Convergence on a small synthetic regression task; loss decreases monotonically in first N steps.

---

## 7) Model / Sequential Container

**Goal**
- Compose layers, gather params/grads, and manage workspaces.

**Spec**
- `model.hpp`/`model.cpp`
  - `class Model { public: void add(std::unique_ptr<Layer>); Tensor forward(const Tensor&); Tensor backward(const Tensor&); std::vector<Tensor*> parameters(); std::vector<Tensor*> gradients(); void reserve_workspaces(int max_batch); };`
  - Forward iterates layers; backward in reverse. `reserve_workspaces` bubbles down.

**Acceptance**
- Example MLP builds and runs forward/backward without allocations after warmup.

---

## 8) Trainer & Parallelism

**Goal**
- Orchestrate training with **OpenMP** across samples and **thread‑local gradient buffers**.

**Spec**
- `trainer.hpp`/`trainer.cpp`
  - `struct TrainConfig { int epochs; int batch_size; int threads; bool deterministic; int log_every; };`
  - `class Trainer { public: void train(Model&, Optimizer&, Loss&, DataLoader&, const TrainConfig&); };`
  - Inside each mini‑batch:
    - `#pragma omp parallel num_threads(cfg.threads)`
      - Each thread processes a shard of samples; accumulates into its **private grad buffers** (same shapes as params).
    - **Reduction pass** after region: sum thread locals into global grads.
  - Deterministic mode: fixed threads, `schedule(static)`, fixed seeds.

**Acceptance**
- Training scales ~linearly up to memory bandwidth; no `#pragma omp critical` in the hot path; correctness vs single‑thread run (loss curves match within tolerance).

**Reduction Implementation Choice**
- Start simple: second pass reduction over param arrays using `#pragma omp parallel for` across parameter blocks.
- Optional: an OpenMP `declare reduction` for POD arrays if you want single‑pass.

---

## 9) DataLoader

**Goal**
- Provide aligned, contiguous batches; enable NUMA‑friendly first‑touch.

**Spec**
- `dataloader.hpp`/`dataloader.cpp`
  - API: `bool next_batch(Tensor& X, Tensor& Y); void reset();` where `X[in_dim,batch]`, `Y[out_dim,batch]`.
  - Index shuffling per epoch; shard‑aware to preserve locality.
  - Double‑buffer optional: prefetch next batch on another thread.

**Acceptance**
- Measures show batch tensors are 32‑byte aligned; shuffling changes sample order across epochs.

---

## 10) Persistence (Save/Load)

**Goal**
- Simple, deterministic binary format + optional JSON sidecar for topology.

**Spec**
- `persist.hpp`/`persist.cpp`
  - Header struct: magic `"SUR0"`, version, endianness flag, layer count, per‑layer param shapes.
  - Then raw contiguous parameter blobs in model‑declared order.
  - API: `void save(const Model&, std::string path); void load(Model&, std::string path);`

**Acceptance**
- Save → zero model → load → bit‑identical parameters; inference equality.

---

## 11) Logging & Monitoring

**Goal**
- Lightweight CSV + console hooks.

**Spec**
- `logging.hpp`/`logging.cpp`
  - `struct Logger { void on_batch(int epoch,int batch,float loss,float lr,float grad_norm,double ms); void flush(); }`
  - Writes CSV: `epoch,batch,loss,lr,grad_norm,ms` and throttled console prints every `log_every`.

**Acceptance**
- CSV opens in spreadsheet; increasing epochs show decreasing loss on synthetic task.

---

## 12) Tests & Grad‑Checks

**Goal**
- Ensure math and backprop correctness before optimizing.

**Spec**
- `tests/test_gradcheck.cpp`
  - Finite‑difference gradient check for Dense + activation + MSE on tiny nets: perturb `W[i]` by `±ε`, compare analytical vs numerical `dL/dW[i]`.
  - Tolerance targets: `abs_err < 1e-3`, `rel_err < 1e-2`.
- `tests/test_trainer.cpp`
  - Train tiny MLP on synthetic linear/quadratic mapping; assert loss < threshold after N steps.

**Acceptance**
- All tests green in `Debug-Sanitize` and `Release-AVX2`.

---

## 13) Benchmarks

**Goal**
- Quantify GEMM and elementwise throughput; catch regressions.

**Spec**
- `bench/bench_gemm.cpp`
  - Sweep matrix sizes (e.g., M,N,K ∈ {128,256,512,1024}); report GFLOP/s and GB/s; compare AVX2 vs ref.
- `bench/bench_elemwise.cpp`
  - ReLU/Tanh/Sigmoid throughput in GB/s.

**Acceptance**
- AVX2 outperforms reference by ≥3× on medium sizes.

---

## 14) Example: MLP Surrogate

**Goal**
- Provide a runnable example that exercises the full stack.

**Spec**
- `examples/example_mlp.cpp`
  - Build `Model` with `Dense(128)→ReLU→Dense(64)→ReLU→Dense(out)`
  - `DataLoader` pulls synthetic `(X,Y)` with known mapping.
  - Train for a few epochs; print final MSE.

**Acceptance**
- Final MSE << initial; parameters persist & reload.

---

## 15) Performance & NUMA Notes

- Expose env hints in README:
  - `OMP_PROC_BIND=close`, `OMP_PLACES=cores`, `OMP_NUM_THREADS=<n>`
- Keep batch sizes that favor GEMM (`W[out,in] × X[in,B]`).
- Per‑thread workspaces: allocate once at trainer startup; index by `omp_get_thread_num()`.

---

## 16) Determinism Mode

- Fixed seeds; `omp_set_num_threads(cfg.threads)`; `schedule(static)`.
- Ordered reduction: do the second‑pass summation with a fixed iteration order.
- Document that floating‑point associativity prevents true bit‑wise reproducibility unless single‑threaded.

---

## 17) CMake & CI Details

- Detect AVX2 with `CheckCXXSourceCompiles`; define `SUR_HAS_AVX2`.
- Targets: `surrogate` (static lib), `surrogate_tests`, `surrogate_bench`, `example_mlp`.
- CI matrix builds: Ubuntu (GCC/Clang), Windows (MSVC), macOS (portable preset).

---

## 18) Done‑When Checklist (MVP)

- [ ] All unit tests pass in Debug and Release‑AVX2.
- [ ] Benchmarks show expected speedups.
- [ ] Example trains and persists.
- [ ] No allocations in the forward/backward hot path after warmup.
- [ ] Deterministic mode yields repeatable loss curves across runs on the same machine.

---

## Appendix A: Pseudocode Snippets (for reference)

**Thread‑local gradient buffers + reduction**
```cpp
// inside Trainer::train, per-batch
std::vector<GradBuffers> tls(cfg.threads, GradBuffers(model.parameters()));
#pragma omp parallel num_threads(cfg.threads)
{
  int tid = omp_get_thread_num();
  auto& gb = tls[tid];
  // process shard of samples → accumulate into gb
}
// reduction pass
auto params = model.parameters();
auto grads  = model.gradients();
#pragma omp parallel for schedule(static)
for (size_t p = 0; p < grads.size(); ++p) {
  for (int t = 0; t < cfg.threads; ++t)
    axpy_ref(1.0f, tls[t].grad[p]->data(), grads[p]->data(), grads[p]->size());
}
```

**AVX2 micro‑kernel (conceptual)**
```cpp
// C[m..m+7, n..n+3] += A_panel[m..m+7, k..k+kc] * B_panel[k..k+kc, n..n+3]
__m256 c0 = _mm256_setzero_ps(), c1 = c0, c2 = c0, c3 = c0;
for (int k=0; k<kc; ++k) {
  __m256 a = _mm256_load_ps(&A_panel[k*8]); // 8 rows
  __m128 b4 = _mm_load_ps(&B_panel[k*4]);   // 4 cols
  __m256 b0 = _mm256_broadcastss_ps(_mm_shuffle_ps(b4,b4,0x00));
  __m256 b1 = _mm256_broadcastss_ps(_mm_shuffle_ps(b4,b4,0x55));
  __m256 b2 = _mm256_broadcastss_ps(_mm_shuffle_ps(b4,b4,0xAA));
  __m256 b3 = _mm256_broadcastss_ps(_mm_shuffle_ps(b4,b4,0xFF));
  c0 = _mm256_fmadd_ps(a, b0, c0);
  c1 = _mm256_fmadd_ps(a, b1, c1);
  c2 = _mm256_fmadd_ps(a, b2, c2);
  c3 = _mm256_fmadd_ps(a, b3, c3);
}
_mm256_store_ps(&C_tile[0*8], c0);
_mm256_store_ps(&C_tile[1*8], c1);
_mm256_store_ps(&C_tile[2*8], c2);
_mm256_store_ps(&C_tile[3*8], c3);
```

**Finite‑difference grad check**
```cpp
float gradcheck(Tensor& W, Model& model, Loss& loss, const Tensor& X, const Tensor& Y) {
  const float eps=1e-3f; float max_rel_err=0.f;
  auto analytic = /* backprop to dW */;
  for (int i=0;i<W.size();++i) {
    float old=W.data()[i];
    W.data()[i]=old+eps; float L1=loss.compute(model.forward(X),Y);
    W.data()[i]=old-eps; float L2=loss.compute(model.forward(X),Y);
    W.data()[i]=old;
    float num=(L1-L2)/(2*eps);
    max_rel_err = std::max(max_rel_err, fabs(analytic[i]-num)/std::max(1e-6f,fabs(num)));
  }
  return max_rel_err;
}
```

---

## Appendix B: Future Extensions (post‑MVP)
- RBF layer (Gaussian kernels) with AVX2 distance kernels.
- L‑BFGS optimizer for small models.
- BF16/FP16 storage with FP32 accumulation (emulated on AVX2).
- Multi‑socket NUMA aware sharding (one OpenMP team per socket).
- Mixed precision loggers; structured binary trace for perf tooling.

