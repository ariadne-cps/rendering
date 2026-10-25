# Rendering tutorial

The C++ and Python tutorials use only the mathematics-independent Rendering
API. Examples of mathematical sets, drawer policies, and geometry-to-rendering
adapters remain in Kernel's `tutorial_drawing`.

## C++

Build and install the standalone Rendering library, then configure this
tutorial against the installed headers and shared library (as in Satisfiability):

```sh
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --target ariadne-rendering bindings
cmake --install build-release --prefix "$PWD/install"
cmake -S tutorials/tutorial_rendering -B build-tutorial \
  -DCMAKE_PREFIX_PATH="$PWD/install"
cmake --build build-tutorial
(cd build-tutorial && ./tutorial_rendering)
```

On Unix, you may need to set `LD_LIBRARY_PATH` (Linux) or
`DYLD_LIBRARY_PATH` (macOS) to `$PWD/install/lib` when running an installed
shared library. The tutorial produces PNG files, demonstrating layered styling,
clearing, 2D projections of a standalone `Drawable2dInterface`, and `plot()`.

## Python

Ensure the standalone Rendering Python module from the build is importable,
then run:

```sh
PYTHONPATH="$PWD/build-release/python" python python/tutorials/tutorial_rendering.py
```

This tutorial demonstrates backend selection, rendering properties, figure
metadata, clearing and PNG writing. Currently the standalone Python binding
does not provide a concrete Python `Drawable` constructor. Mathematical
figure adapters are supplied by Drawing.

## CI

Unix and Windows Release jobs compile and execute the C++ tutorial against the
installed library, execute the Python tutorial from the build, and verify that
the expected PNG outputs were written with PNG signatures.
