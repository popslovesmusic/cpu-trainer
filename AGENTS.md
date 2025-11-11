# AGENTS.md — Root Specification and Agent Hierarchy

This document defines the **Agent Architecture** for the **CPU Surrogate Trainer** project. Each agent has a defined scope, responsibilities, and dependencies. Agents follow the **deterministic collaboration protocol**—every agent reads specifications from `spec.md` files in its directory and reports upward to the root orchestrator. The goal is **transparent automation, reproducibility, and modular extensibility**.

---

## 1. Overview

**Purpose:**  This system uses autonomous build, test, and analysis agents to maintain and extend a C++‑based surrogate‑model training framework. Agents ensure that development, optimization, and documentation tasks remain reproducible, test‑driven, and performance‑verified.

**Hierarchy:**
```
Root AGENTS.md
├── Build Agent
├── Kernel Agent
├── Layer Agent
├── Trainer Agent
├── Data Agent
├── Test Agent
├── Benchmark Agent
├── Persist Agent
├── Logger Agent
└── Orchestrator Agent (meta‑controller)
```

---

## 2. Root Orchestrator Agent

**Name:** `Orchestrator`

**Role:** Governs lifecycle: build → validate → benchmark → document.

**Responsibilities:**
- Parse all subordinate `spec.md` files.
- Run checksums for code & configuration consistency.
- Trigger agents in dependency order.
- Aggregate results into `analysis/report.md`.

**Inputs:** All project specs, CMake outputs, and test logs.

**Outputs:** Unified validation report with success/failure per phase.

**Execution Policy:** Deterministic (single‑thread control). Parallel agents use OpenMP for internal workloads only.

---

## 3. Build Agent

**Scope:** Toolchain configuration and CI setup.

**Responsibilities:**
- Verify compiler flags (`-O3`, `-mavx2`, `-fopenmp`, etc.).
- Maintain portable presets (`Debug‑Sanitize`, `Release‑AVX2`).
- Confirm that AVX2 feature detection and OpenMP linking succeed.
- Produce build summaries and binary metadata (`size`, `symbols`, `linktime`).

**Artifacts:** `build/summary.log`, `build/config.json`.

**Triggers:**
- Run before Test Agent and Benchmark Agent.
- Report upward any flag drift or compiler regression.

---

## 4. Kernel Agent

**Scope:** Low‑level performance kernels (`MathKernels`).

**Responsibilities:**
- Validate GEMM, GEMV, dot, AXPY, and activation intrinsics.
- Compare reference vs AVX2 output for tolerance compliance.
- Collect GFLOP/s and GB/s metrics from `bench/bench_gemm.cpp`.
- Auto‑tune block sizes (optional heuristic search mode).

**Artifacts:** `analysis/kernel_metrics.csv`, `analysis/kernel_validation.log`.

**Triggers:**
- Runs after Build Agent passes.
- On failure, halts dependent agents (Layer, Trainer).

---

## 5. Layer Agent

**Scope:** `Dense` and `Activation` layer correctness & workspace allocation.

**Responsibilities:**
- Validate forward/backward parity with finite‑difference checks.
- Ensure no dynamic allocation in hot paths (profiling hooks enabled).
- Measure per‑layer runtime and cache miss ratios (if hardware counters available).

**Artifacts:** `analysis/layer_gradcheck.log`, `analysis/layer_perf.csv`.

---

## 6. Trainer Agent

**Scope:** End‑to‑end training orchestration.

**Responsibilities:**
- Execute full mini‑batch loop under OpenMP parallelism.
- Detect race conditions, reduction errors, or non‑determinism.
- Compare single‑thread vs multi‑thread loss curves.
- Record per‑epoch throughput, wall time, and gradient norms.

**Artifacts:** `analysis/trainer_metrics.csv`, `analysis/loss_curves.png`.

---

## 7. Data Agent

**Scope:** DataLoader and memory alignment integrity.

**Responsibilities:**
- Verify 32‑byte alignment of input tensors.
- Check shuffling and batch boundaries.
- Optionally simulate multi‑NUMA memory access patterns.

**Artifacts:** `analysis/data_alignment.log`, `analysis/loader_trace.csv`.

---

## 8. Test Agent

**Scope:** Unit and integration validation.

**Responsibilities:**
- Run all tests under both `Debug‑Sanitize` and `Release‑AVX2`.
- Check for memory leaks, undefined behavior, and tolerance compliance.
- Aggregate results to `analysis/test_summary.md`.

**Artifacts:** GTest XML and Markdown summaries.

---

## 9. Benchmark Agent

**Scope:** Performance regression detection.

**Responsibilities:**
- Run benchmark suite and compare with baseline thresholds.
- Alert if performance < 80% of last known best per kernel.
- Update rolling mean GFLOP/s log.

**Artifacts:** `benchmarks/history.csv`, `benchmarks/summary.md`.

---

## 10. Persist Agent

**Scope:** Save/load subsystem.

**Responsibilities:**
- Validate binary I/O integrity for parameters.
- Perform checksum and byte‑equality verification after reload.
- Generate JSON topology manifests.

**Artifacts:** `persist/integrity_report.md`.

---

## 11. Logger Agent

**Scope:** Metric logging and live monitoring.

**Responsibilities:**
- Capture runtime statistics and write CSV logs.
- Maintain ring buffer for console output.
- Handle `on_batch_end()` callbacks from Trainer.
- Generate plots via external scripts (optional Python hook).

**Artifacts:** `logs/train_metrics.csv`, `plots/loss_vs_epoch.png`.

---

## 12. Interaction & Dependencies

| Agent | Depends On | Provides To |
| :--- | :--- | :--- |
| Build | – | Kernel, Layer, Trainer |
| Kernel | Build | Layer, Trainer |
| Layer | Kernel | Trainer |
| Trainer | Layer, Data, Loss, Optimizer | Benchmark, Logger |
| Data | – | Trainer |
| Test | Build | Orchestrator |
| Benchmark | Trainer | Orchestrator |
| Persist | Model | Orchestrator |
| Logger | Trainer | Orchestrator |

**All agents report to Orchestrator**, which merges their artifacts into `analysis/report.md`.

---

## 13. Agent Execution Protocol

Each agent implements the following contract (in pseudocode):
```cpp
struct AgentContext {
  std::filesystem::path root;
  std::map<std::string,std::string> env;
  std::ofstream log;
};

class Agent {
 public:
  virtual bool init(const AgentContext&) = 0;
  virtual bool execute() = 0;
  virtual bool validate() = 0;
  virtual void report(std::ostream&) const = 0;
  virtual ~Agent() = default;
};
```

Agents may be chained through the Orchestrator:
```cpp
std::vector<std::unique_ptr<Agent>> agents = {
  make_build_agent(), make_kernel_agent(), make_layer_agent(), ...
};
for(auto& a: agents){
  if(!a->init(ctx) || !a->execute() || !a->validate()) break;
  a->report(report_file);
}
```

---

## 14. Quality Rules

- **Spec Alignment:** Each agent must read its sibling `spec.md` for parameters and scope.
- **Deterministic Outputs:** All timestamps converted to monotonic deltas; avoid nondeterministic random seeds unless isolated.
- **Fail‑Fast:** If an agent reports `false` from `validate()`, execution halts.
- **Traceability:** All artifacts are written to `analysis/` or subfolders; filenames prefixed by agent name.

---

## 15. Agent Creation Template

To add a new agent (e.g., `Profiler Agent`):
1. Create `profiler_agent.cpp` and header.
2. Add entry in `AGENTS.md` under hierarchy.
3. Update `orchestrator_agent.cpp` registration.
4. Write its `spec.md` defining inputs, outputs, and validation rules.
5. Run Orchestrator with `--agents=profiler` to test in isolation.

---

## 16. Root Validation Targets

| Phase | Pass Criteria |
| :--- | :--- |
| Build | All presets compile and link without warnings. |
| Kernel | AVX2 kernels pass correctness tests and exceed baseline throughput. |
| Layer | Grad‑check error < 1e‑3; no heap allocations in forward/backward. |
| Trainer | Deterministic convergence; linear scaling ≤ 10% deviation. |
| Persist | Round‑trip checksum equality. |
| Logger | CSV non‑empty; columns match spec. |
| Benchmark | ≥80% of baseline throughput per kernel. |

---

## 17. Reporting Summary

Final aggregated output: `analysis/report.md`
- **Sections:** Build, Kernels, Layers, Trainer, Data, Tests, Benchmarks, Persist, Logs.
- **Metrics:** compile time, GFLOP/s, GB/s, grad‑check error, loss convergence, wall‑clock throughput.
- **Outcome Codes:** `PASS`, `WARN`, `FAIL`, with timestamp and deterministic seed ID.

---

## 18. Maintenance Notes

- Each agent includes a `--selftest` mode to run its isolated validation.
- Logs rotate weekly; older artifacts compressed to `analysis/archive/`.
- Any spec change triggers auto‑regeneration of dependent specs via the Orchestrator.

---

**End of AGENTS.md**

