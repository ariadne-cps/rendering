/***************************************************************************
 *            rendering/null_rendering.hpp
 *
 *  Copyright  2020-21  Mirko Albanese, Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Rendering.
 *
 *  Ariadne Rendering is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Rendering is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Rendering. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file rendering/null_rendering.hpp
 *  @brief Canvas and backend implementations that discard rendering operations.
 */

#ifndef ARIADNE_NULL_RENDERING_HPP
#define ARIADNE_NULL_RENDERING_HPP

#include "rendering/geometry2d.hpp"
#include "rendering/rendering_backend_interface.hpp"
#include "rendering/rendering_interface.hpp"

namespace Ariadne {

/** @brief Provide a canvas that ignores drawing operations. */
class NullCanvas : public CanvasInterface {
  public:
    /** @brief Initialize the canvas coordinate system. */
    virtual Void initialise(StringType, StringType, double, double, double, double) {}
    /** @brief Finish the drawing operation. */
    virtual Void finalise() {}

    /** @brief Write the rendered output to the requested file. */
    virtual Void write(const char*) const {}

    /** @brief Move the current path to a coordinate. */
    virtual Void move_to(double, double) {}
    /** @brief Append a line segment to the current path. */
    virtual Void line_to(double, double) {}
    /** @brief Append a circle to the current path. */
    virtual Void circle(double, double, double) {}
    /** @brief Draw a point using the current dot radius. */
    virtual Void dot(double, double) {}
    /** @brief Stroke the current drawing path. */
    virtual Void stroke() {}
    /** @brief Fill the current drawing path. */
    virtual Void fill() {}

    /** @brief Provide the fill boundary operation. */
    virtual Void fill_boundary(const List<Point2d>&) {}

    /** @brief Set line width. */
    virtual Void set_line_width(double) {}
    /** @brief Set line colour. */
    virtual Void set_line_colour(double, double, double) {}
    /** @brief Set fill opacity. */
    virtual Void set_fill_opacity(double) {}
    /** @brief Set fill colour. */
    virtual Void set_fill_colour(double, double, double) {}

    /** @brief Provide the scaling operation. */
    virtual Vector2d scaling() const { return Vector2d(0, 0); }
    /** @brief Provide the bounds operation. */
    virtual Box2d bounds() const { return Box2d(0, 0, 0, 0); }
};

/** @brief Create no-op canvases for disabled drawing. */
class NullRenderingBackend : public RenderingBackendInterface {
  public:
    /** @brief Create canvas. */
    SharedPointer<CanvasInterface> make_canvas(const char*, Nat, Nat, Bool) const override { return std::make_shared<NullCanvas>(); }
};

} // namespace Ariadne

#endif // ARIADNE_NULL_RENDERING_HPP

