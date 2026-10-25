# Ariadne Rendering

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Unix Status](https://github.com/ariadne-cps/rendering/actions/workflows/unix.yml/badge.svg)](https://github.com/ariadne-cps/rendering/actions/workflows/unix.yml)
[![Windows Status](https://github.com/ariadne-cps/rendering/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/rendering/actions/workflows/win.yml) [![Sanitizers Status](https://github.com/ariadne-cps/rendering/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/rendering/actions/workflows/sanitizers.yml) [![Coverage Status](https://github.com/ariadne-cps/rendering/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/rendering/actions/workflows/coverage.yml) [![codecov](https://codecov.io/gh/ariadne-cps/rendering/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/rendering)

Rendering provides Ariadne's mathematics-independent rendering primitives, figure abstraction, backend interface, and self-contained raster PNG backend.

Its only Ariadne runtime dependency is [ariadne-cps/utility](https://github.com/ariadne-cps/utility). Build configuration is shared through [ariadne-cps/configuration](https://github.com/ariadne-cps/configuration). Optional Python bindings use [ariadne-cps/python-common](https://github.com/ariadne-cps/python-common).

The repository deliberately does not depend on numeric, interval, algebra, function, geometry, solving, or drawing modules. Mathematical objects are adapted to rendering by higher-level repositories.

## C++ public header

C++ executables linking `ariadne-rendering` can include the full rendering API with a single header, `ariadne-rendering.hpp`:

```cpp
#include "ariadne-rendering.hpp"

int main() {
    using namespace Ariadne;
    Figure figure(RenderingBox({{-1.0,1.0},{-1.0,1.0}}),Projection2d(2u,0u,1u));
    figure.set_fill_colour(blue);
}
```

The header covers figures, colours, rendering geometry, canvas and backend interfaces, the default and null backends, and backend management.

## Build

Clone the repository together with its Git submodules:

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/rendering.git
cd rendering
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-rendering bindings
cmake --install build-release --prefix install
```

A C++20 compiler, CMake and Python 3.11+ with development headers are required for building the optional Python bindings. The default renderer is self-contained and requires no external graphics, font, compression, or image library.

## Tests

Build and run the C++ and Python tests independently in Debug mode. Install `pytest` for the chosen Python interpreter first.

```bash
python -m pip install pytest
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DPYTHON_ENFORCE_TESTS=ON
cmake --build build-debug --parallel --target tests
ctest --test-dir build-debug -LE python --output-on-failure
ctest --test-dir build-debug -L python --output-on-failure
```

## Sanitizers

The Sanitizers workflow runs independent Debug jobs on Ubuntu with Clang: ThreadSanitizer for data races, and AddressSanitizer + UndefinedBehaviorSanitizer for memory errors and undefined behavior. Python bindings are disabled in these jobs.

## Coverage

Configure a separate Debug build with coverage enabled:

```bash
mkdir build-coverage
cd build-coverage
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
cmake --build . --parallel --target coverage
```

On Ubuntu coverage is generated with GCC/lcov. On macOS it is generated with AppleClang/LLVM coverage tools.
