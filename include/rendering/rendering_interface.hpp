/***************************************************************************
 *            rendering/rendering_interface.hpp
 *
 *  Copyright  2009-20  Pieter Collins, Mirko Albanese
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

/** @file rendering/rendering_interface.hpp
 *  @brief Interfaces for figures, canvases, projections, and drawable objects.
 */

#ifndef ARIADNE_RENDERING_INTERFACE_HPP
#define ARIADNE_RENDERING_INTERFACE_HPP
#include <ostream>

#include "rendering/rendering_box.hpp"
#include "utility/container.hpp"

namespace Ariadne {

/** @brief Represent Vector2d for rendering operations. */
struct Vector2d;
/** @brief Represent Point2d for rendering operations. */
struct Point2d;
/** @brief Represent Box2d for rendering operations. */
struct Box2d;

using RenderingBoundingBoxType = RenderingBox;
/** @brief Represent Colour for rendering operations. */
struct Colour;

/** @brief Define objects that can render themselves onto a canvas. */
class Drawable2dInterface;
/** @brief Define the configurable figure and drawing interface. */
class FigureInterface;
/** @brief Define the primitive drawing operations supported by canvases. */
class CanvasInterface;

/** @brief Select two coordinates from an N-dimensional drawing space. */
struct Projection2d;

/** @brief Represent ProjType for rendering operations. */
enum class ProjType { no_proj,
                      x1_proj,
                      x2_proj };

/** @brief Select two coordinates from an N-dimensional drawing space. */
struct Projection2d {
    DimensionType n, i, j;
    /** @brief Construct a projection by specifying the source dimension and two coordinates. */
    Projection2d(DimensionType nn, DimensionType ii, DimensionType jj) : n(nn), i(ii), j(jj) {}
    /** @brief Return argument size. */
    DimensionType argument_size() const { return n; }
    /** @brief Return x coordinate. */
    DimensionType x_coordinate() const { return i; }
    /** @brief Return y coordinate. */
    DimensionType y_coordinate() const { return j; }
    /** @brief Implement the operator<< operation. */
    friend OutputStream& operator<<(OutputStream& os, const Projection2d& p) {
        return os << "P<R" << p.n << ";R2>[x" << p.i << ",x" << p.j << "]";
    }
};

//! \ingroup RenderingModule
//! \brief Base interface for plotting and drawing classes.
class FigureInterface {
  public:
    /** @brief Destroy the rendering interface. */
    virtual ~FigureInterface() = default;
    /** @brief Set projection map. */
    virtual FigureInterface& set_projection_map(const Projection2d& prj) = 0;
    /** @brief Set bounding box. */
    virtual FigureInterface& set_bounding_box(const RenderingBoundingBoxType& bx) = 0;
    /** @brief Set projection. */
    virtual FigureInterface& set_projection(DimensionType as, DimensionType ix, DimensionType iy) = 0;
    /** @brief Set line style. */
    virtual FigureInterface& set_line_style(Bool) = 0;
    /** @brief Set line width. */
    virtual FigureInterface& set_line_width(Dbl) = 0;
    /** @brief Set dot radius. */
    virtual FigureInterface& set_dot_radius(Dbl) = 0;
    /** @brief Set line colour. */
    virtual FigureInterface& set_line_colour(Colour) = 0;
    /** @brief Set fill opacity. */
    virtual FigureInterface& set_fill_opacity(Dbl) = 0;
    /** @brief Set fill colour. */
    virtual FigureInterface& set_fill_colour(Colour) = 0;
    /** @brief Return line style. */
    virtual Bool get_line_style() const = 0;
    /** @brief Return line width. */
    virtual Dbl get_line_width() const = 0;
    /** @brief Return line colour. */
    virtual Colour get_line_colour() const = 0;
    /** @brief Return fill style. */
    virtual Bool get_fill_style() const = 0;
    /** @brief Return fill opacity. */
    virtual Dbl get_fill_opacity() const = 0;
    /** @brief Return fill colour. */
    virtual Colour get_fill_colour() const = 0;
    /** @brief Perform the draw canvas operation. */
    virtual FigureInterface& draw(const Drawable2dInterface&) = 0;
};
/** @brief Perform the draw canvas operation. */
inline Void draw(FigureInterface& fig, const Drawable2dInterface& shape) { fig.draw(shape); }

/** @brief Represent Figure for rendering operations. */
class Figure;
/** @brief Perform the draw canvas operation. */
Void draw(Figure& fig, const Drawable2dInterface& shape);
/** @brief Implement the operator<< operation. */
Figure& operator<<(Figure& fig, const Drawable2dInterface& shape);

//! \ingroup RenderingModule
//! \brief Interface to two-dimensional drawing canvas with the ability to draw polyhedra.
class CanvasInterface {
  public:
    //! \brief Destructor
    virtual ~CanvasInterface() = default;

    /** @brief Perform the initialise canvas operation. */
    virtual Void initialise(StringType x, StringType y, double lx, double ux, double ly, double uy) = 0;
    /** @brief Perform the finalise canvas operation. */
    virtual Void finalise() = 0;

    /** @brief Perform the write canvas operation. */
    virtual Void write(const char* filename) const = 0;

    //! \brief Move the current initial point for a line to the point \a (x,y).
    virtual Void move_to(double x, double y) = 0;
    //! \brief Create a line segment from the current point to the point \a (x,y).
    virtual Void line_to(double x, double y) = 0;
    //! \brief Draw a circle with centre \a (x,y) and radius \a r.
    virtual Void circle(double x, double y, double r) = 0;
    //! \brief Draw a dot with centre \a (x,y).
    virtual Void dot(double x, double y) = 0;
    //! \brief Draw the working line.
    virtual Void stroke() = 0;
    //! \brief Draw and fill the working line.
    virtual Void fill() = 0;

    //! \brief Draw the lines from the \a boundary and fill the corresponding closed polytope
    virtual Void fill_boundary(const List<Point2d>& boundary) = 0;

    //! Set the width of the lines bounding shapes, in pixels.
    virtual Void set_line_width(double lw) = 0;
    //! \brief Set the colour of subsequent line to be drawn. The \a r, \a g and \a b
    //! arguments are intensities of red, green and blue on a scale of 0.0 to 1.0.
    virtual Void set_line_colour(double r, double g, double b) = 0;
    //! \brief  Set the colour of the next regions to be filled.
    virtual Void set_fill_opacity(double fo) = 0;
    //! \brief Set the colour of subsequent regions to be filled.
    virtual Void set_fill_colour(double r, double g, double b) = 0;
};

//! \ingroup RenderingModule
//! \brief Base interface for drawable objects
class Drawable2dInterface {
  public:
    //! brief Virtual destructor.
    virtual ~Drawable2dInterface() = default;
    //! brief Make a dynamically-allocated copy.
    virtual Drawable2dInterface* clone() const = 0;
    //! brief Draw the object on the canvas \a c using line segments and fill/stroke commands.
    virtual Void draw(CanvasInterface& c, const Projection2d& p) const = 0;
    //! brief The dimension of the object in Euclidean space
    virtual DimensionType dimension() const = 0;
};

} // namespace Ariadne

#endif // ARIADNE_RENDERING_INTERFACE_HPP
