/***************************************************************************
 *            rendering/null_rendering.hpp
 *
 *  Copyright  2020-21  Mirko Albanese, Luca Geretti
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

/*! \file rendering/null_rendering.hpp
 *  \brief Null output in the case of missing rendering support.
 */

#ifndef ARIADNE_NULL_RENDERING_HPP
#define ARIADNE_NULL_RENDERING_HPP

#include "rendering/rendering_interface.hpp"
#include "rendering/geometry2d.hpp"
#include "rendering/rendering_backend_interface.hpp"

namespace Ariadne {

class NullCanvas : public CanvasInterface
{
public:
    virtual Void initialise(StringType, StringType, double, double, double, double) { }
    virtual Void finalise() { }

    virtual Void write(const char*) const { }

    virtual Void move_to(double, double) { }
    virtual Void line_to(double, double) { }
    virtual Void circle(double, double, double) { }
    virtual Void dot(double, double) { }
    virtual Void stroke() { }
    virtual Void fill() { }

    virtual Void fill_boundary(List<Point2d> const&) { }

    virtual Void set_line_width(double) { }
    virtual Void set_line_colour(double, double, double) { }
    virtual Void set_fill_opacity(double) { }
    virtual Void set_fill_colour(double, double, double) { }

    virtual Vector2d scaling() const { return Vector2d(0,0); }
    virtual Box2d bounds() const { return Box2d(0,0,0,0); }
};

class NullRenderingBackend : public RenderingBackendInterface {
  public:
    SharedPointer<CanvasInterface> make_canvas(const char*, Nat, Nat, Bool) const override { return std::make_shared<NullCanvas>(); }
};

} // namespace Ariadne

#endif // ARIADNE_NULL_RENDERING_HPP


