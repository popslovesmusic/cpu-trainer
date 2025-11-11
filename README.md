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

