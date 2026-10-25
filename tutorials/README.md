# Rendering tutorial

The C++ and Python tutorials cover Ariadne's **mathematics-independent**
rendering API. They are adapted from `kernel/decouple-drawing`; mathematical
sets, the drawer policy and conversion adapters remain in Kernel's Drawing
tutorial.

## C++

From the rendering repository root, after initializing the submodules:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake -S tutorials/tutorial_rendering -B build-tutorial \
  -DARIADNE_RENDERING_SOURCE_DIR="$PWD"
cmake --build build-tutorial --target tutorial_rendering
cd build-tutorial && ./tutorial_rendering
```

The executable writes several PNGs in its working directory. It illustrates
layered styling, clearing a figure, projections of a small standalone
`Drawable2dInterface`, and the `plot()` convenience API.

## Python

Build the standalone Rendering Python module, ensure it is importable as
`pyariadne`, then run:

```sh
python python/tutorials/tutorial_rendering.py
```

The Python tutorial covers backend selection, style, figures, projection
metadata, clearing and PNG output. The current standalone Python binding does
not expose a concrete `Drawable` constructor, so a fully populated scene is
demonstrated in C++ only. Mathematical figure adapters belong to Drawing.
