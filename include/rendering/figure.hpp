/***************************************************************************
 *            rendering/figure.hpp
 *
 *  Copyright  2008-21  Pieter Collins, Mirko Albanese, Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

/*! \file rendering/figure.hpp
 *  \brief Figure classes for drawing and outputting shapes in Euclidean space.
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

class Figure;
Void draw_to_figure(Figure&,Drawable2dInterface const&);

struct LineStyle { explicit LineStyle(Bool ls) : _style(ls) { } operator Bool() const { return this->_style; } private: Bool _style; };
struct LineWidth { explicit LineWidth(Dbl lw) : _width(lw) { } operator Dbl() const { return this->_width; } private: Dbl _width; };
struct LineColour : Colour { LineColour(const Colour& lc) : Colour(lc) { } LineColour(Dbl r, Dbl g, Dbl b) : Colour(r,g,b) { } };
struct FillStyle { explicit FillStyle(Bool fs) : _style(fs) { } operator Bool() const { return this->_style; } private: Bool _style; };
struct FillOpacity { explicit FillOpacity(Dbl fo) : _opacity(fo) { } operator Dbl() const { return this->_opacity; } private: Dbl _opacity; };
struct FillColour : Colour { FillColour(const Colour& fc) : Colour(fc) { } FillColour(Dbl r, Dbl g, Dbl b) : Colour(r,g,b) { } };
struct Animated { explicit Animated(Bool b) : _anim(b) { } operator Bool() const { return this-> _anim; } private: Bool _anim; };

inline LineStyle line_style(Bool s) { return LineStyle(s); }
inline LineWidth line_width(Dbl w) { return LineWidth(w); }
inline LineColour line_colour(const Colour& c) { return LineColour(c); }
inline LineColour line_colour(Dbl r, Dbl g, Dbl b) { return LineColour(Colour(r,g,b)); }
inline FillStyle fill_style(Bool s) { return FillStyle(s); }
inline FillOpacity fill_opacity(Dbl o) { return FillOpacity(o); }
inline FillColour fill_colour(const Colour& c) { return FillColour(c); }
inline FillColour fill_colour(Dbl r, Dbl g, Dbl b) { return FillColour(Colour(r,g,b)); }
inline Animated set_animated(Bool b) { return Animated(b); }

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

    RenderingProperties& set_dot_radius(Dbl);
    RenderingProperties& set_line_style(Bool);
    RenderingProperties& set_line_width(Dbl);
    RenderingProperties& set_line_colour(Colour);
    RenderingProperties& set_fill_style(Bool);
    RenderingProperties& set_fill_colour(Colour);

    RenderingProperties& set_line_colour(Dbl, Dbl, Dbl);
    RenderingProperties& set_fill_colour(Dbl, Dbl, Dbl);
    RenderingProperties& set_fill_opacity(Dbl);

    RenderingProperties& set_animated(Bool);


    friend OutputStream& operator<<(OutputStream& os, RenderingProperties const& gp);
};

Void set_properties(CanvasInterface& canvas, const RenderingProperties& properties);

//! \brief Class for plotting figures.
class Figure
    : public FigureInterface
{
  public:
    ~Figure();
    Figure(Figure const&);
    Figure& operator=(Figure const&);
    Figure(Figure&&) noexcept;
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

    Figure& set_projection_map(const Projection2d&);
    
    Projection2d get_2d_projection_map() const;
    Figure& set_axis_labels(StringType const& x, StringType const& y);

    Figure& set_bounding_box(const RenderingBoundingBoxType&);
    template<class B> Figure& set_bounding_box(B const& box) {
        return this->set_bounding_box(to_rendering_box(box));
    }

    RenderingBoundingBoxType get_bounding_box() const;
    //! Set the displayed coordinates to (\a i, \a j)
    Figure& set_projection(DimensionType i, DimensionType j);
    Figure& set_projection(DimensionType as, DimensionType ix, DimensionType iy);
    
    //! Set the radiues to draw points (dots).
    Figure& set_dot_radius(Dbl);
    Figure& set_line_style(Bool);
    //! Set the width to draw lines. A width of 0 means no lines are drawn.
    Figure& set_line_width(Dbl);
    //! Set the colour draw lines
    Figure& set_line_colour(Colour);
    Figure& set_fill_style(Bool);
    //! Set the colour to fill shapes.
    Figure& set_fill_colour(Colour);

    Figure& set_line_colour(Dbl, Dbl, Dbl);
    Figure& set_fill_colour(Dbl, Dbl, Dbl);
    //! Set the opacity of shapes. An opacity of 0 means no fill.
    Figure& set_fill_opacity(Dbl);

    Bool get_line_style() const;
    Dbl get_line_width() const;
    Dbl get_dot_radius() const;
    Colour get_line_colour() const;
    Bool get_fill_style() const;
    Dbl get_fill_opacity() const;
    Colour get_fill_colour() const;

  

    RenderingProperties& properties();
    RenderingProperties const& properties() const;

    //! Add a set to draw onto the figure.
    Figure& draw(const Drawable2dInterface& shape);
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
    struct Data;
  public:
    Void _paint_all(CanvasInterface& canvas) const;
  
  private:
    std::unique_ptr<Data> _data;
};


inline Void draw_to_figure(Figure& figure, Drawable2dInterface const& shape) { figure.draw(shape); }

inline Figure& operator<<(Figure& g, const LineStyle& ls) { g.properties().set_line_style(ls); return g; }
inline Figure& operator<<(Figure& g, const LineWidth& lw) { g.properties().set_line_width(lw); return g; }
inline Figure& operator<<(Figure& g, const LineColour& lc) { g.properties().set_line_colour(lc); return g; }
inline Figure& operator<<(Figure& g, const FillStyle& fs) { g.properties().set_fill_style(fs); return g; }
inline Figure& operator<<(Figure& g, const FillOpacity& fo) { g.properties().set_fill_opacity(fo); return g; }
inline Figure& operator<<(Figure& g, const FillColour& fc) { g.properties().set_fill_colour(fc); return g; }
inline Figure& operator<<(Figure&g, const Animated& animation) {g.properties().set_animated(animation); return g; }

inline Void draw(Figure&) { }

template<class SET, class... CSETS> inline Void
draw(Figure& g, const Colour& fc, const SET& set, CSETS const& ... csets) {
    g.set_fill_colour(fc); draw(g,set); draw(g,csets...); }

template<class... CSETS> Void
plot(const char* filename, const Projection2d& pr, const RenderingBoundingBoxType& bbox, CSETS const&... csets) {
    Figure g(bbox,pr); draw(g,csets...);  g.write(filename); }

template<class... CSETS> Void
plot(const char* filename, const RenderingBoundingBoxType& bbox, CSETS const&... csets) {
    plot(filename, Projection2d(2u,0,1), bbox, csets...); }

Void plot(const char* filename, const Projection2d& pr, const RenderingBoundingBoxType& bbox, List<Pair<Colour,Drawable2dInterface const&>> const& csets);

} // namespace Ariadne

#endif // ARIADNE_FIGURE_HPP

