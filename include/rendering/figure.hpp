/***************************************************************************
 *            rendering/figure.hpp
 *
 *  Copyright  2008-21  Pieter Collins, Mirko Albanese, Luca Geretti
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

/** @file rendering/figure.hpp
 *  @brief Figure construction, styling, drawing, and export.
 */

#ifndef ARIADNE_FIGURE_HPP
#define ARIADNE_FIGURE_HPP
#include <iosfwd>
#include <string>
#include <vector>
#include <memory>

#include "utility/typedefs.hpp"
#include "colour.hpp"
#include "rendering_base.hpp"
#include "rendering_backend_interface.hpp"

namespace Ariadne {

/** @brief Collect shapes and rendering properties and write figures. */
class Figure;
/** @brief Provide the draw to figure operation. */
Void draw_to_figure(Figure&,Drawable2dInterface const&);

/** @brief Toggle line strokes for a figure. */
struct LineStyle { explicit LineStyle(Bool ls) : _style(ls) { } operator Bool() const { return this->_style; } private: Bool _style; };
/** @brief Specify the width of figure outlines. */
struct LineWidth { explicit LineWidth(Dbl lw) : _width(lw) { } operator Dbl() const { return this->_width; } private: Dbl _width; };
/** @brief Specify the colour of figure outlines. */
struct LineColour : Colour { LineColour(const Colour& lc) : Colour(lc) { } LineColour(Dbl r, Dbl g, Dbl b) : Colour(r,g,b) { } };
/** @brief Toggle filling of drawn shapes. */
struct FillStyle { explicit FillStyle(Bool fs) : _style(fs) { } operator Bool() const { return this->_style; } private: Bool _style; };
/** @brief Specify opacity of figure fills. */
struct FillOpacity { explicit FillOpacity(Dbl fo) : _opacity(fo) { } operator Dbl() const { return this->_opacity; } private: Dbl _opacity; };
/** @brief Specify the colour used to fill shapes. */
struct FillColour : Colour { FillColour(const Colour& fc) : Colour(fc) { } FillColour(Dbl r, Dbl g, Dbl b) : Colour(r,g,b) { } };
/** @brief Select animated output for a figure. */
struct Animated { explicit Animated(Bool b) : _anim(b) { } operator Bool() const { return this-> _anim; } private: Bool _anim; };

/** @brief Provide the line style operation. */
inline LineStyle line_style(Bool s) { return LineStyle(s); }
/** @brief Provide the line width operation. */
inline LineWidth line_width(Dbl w) { return LineWidth(w); }
/** @brief Provide the line colour operation. */
inline LineColour line_colour(const Colour& c) { return LineColour(c); }
/** @brief Provide the line colour operation. */
inline LineColour line_colour(Dbl r, Dbl g, Dbl b) { return LineColour(Colour(r,g,b)); }
/** @brief Provide the fill style operation. */
inline FillStyle fill_style(Bool s) { return FillStyle(s); }
/** @brief Provide the fill opacity operation. */
inline FillOpacity fill_opacity(Dbl o) { return FillOpacity(o); }
/** @brief Provide the fill colour operation. */
inline FillColour fill_colour(const Colour& c) { return FillColour(c); }
/** @brief Provide the fill colour operation. */
inline FillColour fill_colour(Dbl r, Dbl g, Dbl b) { return FillColour(Colour(r,g,b)); }
/** @brief Set animated. */
inline Animated set_animated(Bool b) { return Animated(b); }

/** @brief Collect the current styling options for a figure. */
struct RenderingProperties {
    RenderingProperties()
        : dot_radius(1.0), line_style(true), line_width(1.0), line_colour(black), fill_style(true), fill_colour(orange), is_animated(false) { }
    RenderingProperties(Bool ls, Dbl lw, Dbl dr, Colour lc, Bool fs, Colour fc)
        : dot_radius(dr), line_style(ls), line_width(lw), line_colour(lc), fill_style(fs), fill_colour(fc), is_animated(false) { }
    Dbl dot_radius;
    Bool line_style;
    Dbl line_width;
    Colour line_colour;
    Bool fill_style;
    Colour fill_colour;

    Bool is_animated;

    /** @brief Set dot radius. */
    RenderingProperties& set_dot_radius(Dbl);
    /** @brief Set line style. */
    RenderingProperties& set_line_style(Bool);
    /** @brief Set line width. */
    RenderingProperties& set_line_width(Dbl);
    /** @brief Set line colour. */
    RenderingProperties& set_line_colour(Colour);
    /** @brief Set fill style. */
    RenderingProperties& set_fill_style(Bool);
    /** @brief Set fill colour. */
    RenderingProperties& set_fill_colour(Colour);

    /** @brief Set line colour. */
    RenderingProperties& set_line_colour(Dbl, Dbl, Dbl);
    /** @brief Set fill colour. */
    RenderingProperties& set_fill_colour(Dbl, Dbl, Dbl);
    /** @brief Set fill opacity. */
    RenderingProperties& set_fill_opacity(Dbl);

    /** @brief Set animated. */
    RenderingProperties& set_animated(Bool);


    /** @brief Provide the operator<< operation. */
    friend OutputStream& operator<<(OutputStream& os, RenderingProperties const& gp);
};

/** @brief Set properties. */
Void set_properties(CanvasInterface& canvas, const RenderingProperties& properties);

//! \brief Class for plotting figures.
class Figure
    : public FigureInterface
{
  public:
    ~Figure();
    Figure(Figure const&);
    /** @brief Provide the operator= operation. */
    Figure& operator=(Figure const&);
    Figure(Figure&&) noexcept;
    /** @brief Provide the operator= operation. */
    Figure& operator=(Figure&&) noexcept;
    //! Construct a figure projecting \a bbx onto the \a proj coordinates
    Figure(const RenderingBoundingBoxType& bbx, const Projection2d& proj);
    //! Construct a figure projecting \a bbx onto the (\a ix, \a iy) coordinates
    Figure(const RenderingBoundingBoxType& bbx, DimensionType ix, DimensionType iy);
    //! Construct a figure projecting \a bbx onto the (\a ix, \a iy) coordinates
    Figure(const RenderingBoundingBoxType& bbx, Pair<DimensionType,DimensionType> ixy);

    // Mathematical box conversions are supplied via ADL by the drawing module.
    // No mathematics-specific type is mentioned or instantiated in rendering.
    template<class B>
    Figure(B const& box, Projection2d const& projection)
        : Figure(to_rendering_box(box),projection) { }
    template<class B>
    Figure(B const& box, DimensionType i, DimensionType j)
        : Figure(to_rendering_box(box),i,j) { }
    template<class B>
    Figure(B const& box, Pair<DimensionType,DimensionType> coordinates)
        : Figure(to_rendering_box(box),coordinates) { }

    /** @brief Set projection map. */
    Figure& set_projection_map(const Projection2d&);
    
    /** @brief Return 2d projection map. */
    Projection2d get_2d_projection_map() const;
    /** @brief Set axis labels. */
    Figure& set_axis_labels(StringType const& x, StringType const& y);

    /** @brief Set bounding box. */
    Figure& set_bounding_box(const RenderingBoundingBoxType&);
    /** @brief Set bounding box. */
    template<class B> Figure& set_bounding_box(B const& box) {
        return this->set_bounding_box(to_rendering_box(box));
    }

    /** @brief Return bounding box. */
    RenderingBoundingBoxType get_bounding_box() const;
    //! Set the displayed coordinates to (\a i, \a j)
    Figure& set_projection(DimensionType i, DimensionType j);
    /** @brief Set projection. */
    Figure& set_projection(DimensionType as, DimensionType ix, DimensionType iy);
    
    //! Set the radiues to draw points (dots).
    Figure& set_dot_radius(Dbl);
    /** @brief Set line style. */
    Figure& set_line_style(Bool);
    //! Set the width to draw lines. A width of 0 means no lines are drawn.
    Figure& set_line_width(Dbl);
    //! Set the colour draw lines
    Figure& set_line_colour(Colour);
    /** @brief Set fill style. */
    Figure& set_fill_style(Bool);
    //! Set the colour to fill shapes.
    Figure& set_fill_colour(Colour);

    /** @brief Set line colour. */
    Figure& set_line_colour(Dbl, Dbl, Dbl);
    /** @brief Set fill colour. */
    Figure& set_fill_colour(Dbl, Dbl, Dbl);
    //! Set the opacity of shapes. An opacity of 0 means no fill.
    Figure& set_fill_opacity(Dbl);

    /** @brief Return line style. */
    Bool get_line_style() const;
    /** @brief Return line width. */
    Dbl get_line_width() const;
    /** @brief Return dot radius. */
    Dbl get_dot_radius() const;
    /** @brief Return line colour. */
    Colour get_line_colour() const;
    /** @brief Return fill style. */
    Bool get_fill_style() const;
    /** @brief Return fill opacity. */
    Dbl get_fill_opacity() const;
    /** @brief Return fill colour. */
    Colour get_fill_colour() const;

  

    /** @brief Provide the properties operation. */
    RenderingProperties& properties();
    /** @brief Provide the properties operation. */
    RenderingProperties const& properties() const;

    //! Add a set to draw onto the figure.
    Figure& draw(const Drawable2dInterface& shape);
    /** @brief Draw the object on the selected canvas. */
    template<class Shape> Figure& draw(Shape const& shape) {
        draw_to_figure(*this,shape);
        return *this;
    }


    //! Clear the figure.
    Figure& clear();
    //! Set animation
    Figure& set_animated(Bool b);
    //! Write to \a filename.
    Void write(const Char* filename) const;

    //! Write out to file, using width \a nx pixels, and height \a ny pixels
    Void write(const Char* filename, Nat nx, Nat ny) const;

  public:
    /** @brief Describe the Data rendering type. */
    struct Data;
  public:
    /** @brief Provide the  paint all operation. */
    Void _paint_all(CanvasInterface& canvas) const;
  
  private:
    std::unique_ptr<Data> _data;
};


/** @brief Provide the draw to figure operation. */
inline Void draw_to_figure(Figure& figure, Drawable2dInterface const& shape) { figure.draw(shape); }

/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure& g, const LineStyle& ls) { g.properties().set_line_style(ls); return g; }
/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure& g, const LineWidth& lw) { g.properties().set_line_width(lw); return g; }
/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure& g, const LineColour& lc) { g.properties().set_line_colour(lc); return g; }
/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure& g, const FillStyle& fs) { g.properties().set_fill_style(fs); return g; }
/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure& g, const FillOpacity& fo) { g.properties().set_fill_opacity(fo); return g; }
/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure& g, const FillColour& fc) { g.properties().set_fill_colour(fc); return g; }
/** @brief Provide the operator<< operation. */
inline Figure& operator<<(Figure&g, const Animated& animation) {g.properties().set_animated(animation); return g; }

/** @brief Draw the object on the selected canvas. */
inline Void draw(Figure&) { }

template<class SET, class... CSETS> inline Void
draw(Figure& g, const Colour& fc, const SET& set, CSETS const& ... csets) {
    g.set_fill_colour(fc); draw(g,set); draw(g,csets...); }

template<class... CSETS> Void
plot(const char* filename, const Projection2d& pr, const RenderingBoundingBoxType& bbox, CSETS const&... csets) {
    /** @brief Provide the g operation. */
    Figure g(bbox,pr); draw(g,csets...);  g.write(filename); }

template<class... CSETS> Void
plot(const char* filename, const RenderingBoundingBoxType& bbox, CSETS const&... csets) {
    plot(filename, Projection2d(2u,0,1), bbox, csets...); }

/** @brief Render the supplied shapes to an output file. */
Void plot(const char* filename, const Projection2d& pr, const RenderingBoundingBoxType& bbox, List<Pair<Colour,Drawable2dInterface const&>> const& csets);

} // namespace Ariadne

#endif // ARIADNE_FIGURE_HPP

