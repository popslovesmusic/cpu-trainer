# CPU Surrogate Trainer

This repository hosts a C++20 surrogate-model training framework optimized for high-core CPUs. The current milestone establishes the project skeleton, build configuration, and continuous integration scaffolding that future phases will expand with full functionality.

## Layout

```
include/surrogate/        # Public headers for the library API
src/                      # Library implementation files
tests/                    # Placeholder unit tests
bench/                    # Benchmark entry points
examples/                 # Sample applications
cmake/                    # Supplemental toolchain and preset hooks
.github/workflows/        # Continuous integration definitions
```

## Build Presets

The project defines three CMake presets:

- `Debug-Sanitize` – debug build with address/UB sanitizers enabled (AVX2 disabled).
- `Release-AVX2` – optimized build with AVX2, FMA, and OpenMP enabled.
- `Release-Portable` – optimized build without AVX2 to support broader hardware.

List available presets with:

```bash
cmake --list-presets
```

## Continuous Integration

GitHub Actions runs the preset matrix across Linux, macOS, and Windows to ensure configuration coverage.

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
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
## Performance Hints

For best throughput on multi-core machines, pin OpenMP threads close to one another and provide an explicit thread count:

```bash
export OMP_PROC_BIND=close
export OMP_PLACES=cores
export OMP_NUM_THREADS=$(nproc)
```

Keep batch sizes large enough to feed the GEMM kernels efficiently (multiples of 8 rows and 4 columns deliver the best cache reuse).

## Deterministic Training

`sur::TrainConfig` exposes a `deterministic` flag and `seed` so repeated runs can reproduce data ordering and OpenMP scheduling. When determinism is enabled the trainer fixes the OpenMP schedule to `static`, disables dynamic teams, seeds the `DataLoader`, and keeps reductions ordered. Floating-point associativity still allows tiny numeric drift, so bitwise-identical results require single-threaded execution.

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
