# Rendering backend benchmark

`benchmark_rendering` measures **absolute rendering performance** independently
of the Geometry and Function modules. It calls `CanvasInterface` directly,
using deterministically precomputed drawing commands, rather than constructing
`Figure` objects or constrained image sets.

The benchmark measures the Default rendering renderer directly; no backend selection or reference renderer is involved.

## Build and run

Build a Release configuration, since Debug benchmarks are misleading:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target benchmark_rendering --parallel
./build/benchmarks/benchmark_rendering --iterations 7 --warmups 2
```

The executable's location depends on the generator and build configuration.
The existing project-wide `benchmarks` target also builds this benchmark.

Select one scenario/size (for tracking performance across commits):

```sh
./build/benchmarks/benchmark_rendering --scene rectangles --width 800 --height 800 --iterations 10 --warmups 2
```

Optionally measure PNG generation and file I/O **separately**, by writing to
`benchmark-drawing-<scene>.png` in the current directory:

```sh
./build/benchmarks/benchmark_rendering --write-png --png-prefix /tmp/drawing-bench
```

## Diagnostic breakdown

Add `--breakdown` to run four independent passes for each selected scene:
`full` (the unchanged benchmark), `geometry` (path construction and
path lifecycle with both paints disabled), `fill-only` (fill enabled,
stroke disabled), and `stroke-only` (fill disabled, stroke enabled).

```sh
./build/benchmarks/benchmark_rendering --scene rectangles --iterations 20 --warmups 3 --breakdown
```

The table gains a `pass` column. These are **separate end-to-end
measurements**, not exclusive function-level profiles: geometry/path overhead
is present in every pass and the passes should not be summed to reproduce
`full`. In particular, fill-only can still execute zero-width stroke
commands to maintain path lifecycle. The regular invocation (without
`--breakdown`) remains unchanged; PNG output is optional in either mode.

## Scenarios

| Scenario | Primitives | Purpose |
| --- | ---: | --- |
| rectangles | 384 | Dense paving-like fills and narrow strokes |
| polylines | 72 | Long polylines, antialiasing and joins |
| circles | 120 | Curved filled paths, tessellation |
| polygons | 80 | Concave multi-edge paths |
| winding | 54 | Multiple subpaths and holes |
| mixed | 710 | Combination of all preceding scenes |

The scene command buffers, including coordinates and trigonometric values,
are prepared **outside the measured section**. Every timed repetition creates a fresh canvas so no previous
frame can affect the next. Measurements use `Benchmarking::Runner` and the shared monotonic clock. Each round
still creates and initializes one canvas, draws the selected scene, and optionally
writes PNG; the independent phase durations are recorded in nanoseconds and
summarized by `Benchmarking::Statistics`. The overview retains median and
nearest-rank p95 timings in milliseconds and shapes per second.

The framework's `--warmups` option excludes warm-up rounds (default: 0;
use `--warmups 1` for the old driver's default). `--summary` chooses the
statistics displayed by the supplementary per-phase `Benchmarking::Report`.
`--verbosity 1` prints both tables; `--verbosity 0` suppresses ordinary output.

Results are presented through the shared `Benchmarking::Table` and `Report`
facilities at ordinary Logging verbosity. No CSV is generated.

The benchmark has a standalone command-line parser and depends only on the
rendering library. Supported options are `--scene`, `--width`, `--height`,
`--iterations`, `--warmup`, `--write-png`, `--png-prefix` and
`--breakdown`.


The columns contain creation and initialization median times, pure drawing
median and nearest-rank p95 times, and shapes per second. When requested,
the report also includes PNG encoding and file-write time. 
These measurements are not a correctness test. Continue to use the tests
under `tests/rendering` for drawing regression checks. Compare measurements only with the same build type, machine,
compiler, size, scenario and iteration settings.
