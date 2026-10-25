#!/usr/bin/env python3
"""Standalone rendering tutorial: no geometry or drawing bindings required."""

from pyariadne import (
    Colour, RenderingProperties, RenderingManager, DefaultRenderingBackend,
    RenderingBox, Projection2d, Figure, Point2d, ImageSize2d, black, green
)


def tutorial_rendering():
    print("\n== Rendering backend and properties ==")
    RenderingManager.instance().set_backend(DefaultRenderingBackend())
    custom_blue = Colour(0.15, 0.35, 0.80, 0.60)
    style = RenderingProperties()
    style.set_line_width(2.0)
    style.set_line_colour(black)
    style.set_fill_colour(custom_blue)
    style.set_fill_opacity(0.60)
    print("colour:", custom_blue)
    print("properties:", style)

    print("\n== Standalone figure and output ==")
    # The standalone Python binding exposes RenderingBox and Figure, but
    # does not yet expose a concrete Drawable for Figure.draw().
    viewport = RenderingBox(2)
    projection = Projection2d(2, 0, 1)
    figure = Figure(viewport, projection)
    figure.set_fill_colour(green)
    figure.set_fill_opacity(0.30)
    figure.set_line_colour(black)
    figure.set_line_width(2.0)
    print("viewport:", figure.get_bounding_box())
    print("projection:", figure.get_2d_projection_map())
    print("line width:", figure.get_line_width())
    print("fill opacity:", figure.get_fill_opacity())
    figure.write("tutorial_rendering_empty", 640, 480)
    figure.clear()
    figure.write("tutorial_rendering_cleared", 320, 240)
    print("point:", Point2d(1.0, 2.0))
    print("image size:", ImageSize2d(640, 480).nx, ImageSize2d(640, 480).ny)


if __name__ == "__main__":
    tutorial_rendering()
