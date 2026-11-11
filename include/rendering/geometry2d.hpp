/***************************************************************************
 *            rendering/geometry2d.hpp
 *
 *  Copyright  2009-20  Davide Bresolin, Pieter Collins
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

/** @file rendering/geometry2d.hpp
 *  @brief Primitive display-coordinate geometry and drawable polytopes.
 */

#ifndef ARIADNE_GEOMETRY2D_HPP
#define ARIADNE_GEOMETRY2D_HPP

#include "rendering/rendering_interface.hpp"


namespace Ariadne {

/** @brief Describe the List rendering type. */
template<class T >
class List;

/** @brief Represent a displacement in two-dimensional display coordinates. */
struct Vector2d {
    double x,y; Vector2d(double x_, double y_) : x(x_), y(y_) { }
    template<class X, class Y> Vector2d(const X& x_, const Y& y_) : x(static_cast<double>(x_)), y(static_cast<double>(y_)) { }
};
/** @brief Provide the operator- operation. */
inline Vector2d operator-(const Vector2d& v) { return Vector2d(-v.x,-v.y); }
/** @brief Provide the operator+ operation. */
inline Vector2d operator+(const Vector2d& v1, const Vector2d& v2) { return Vector2d(v1.x+v2.x,v1.y+v2.y); }
/** @brief Provide the operator- operation. */
inline Vector2d operator-(const Vector2d& v1, const Vector2d& v2) { return Vector2d(v1.x-v2.x,v1.y-v2.y); }
/** @brief Provide the operator* operation. */
inline Vector2d operator*(const double& s1, const Vector2d& v2) { return Vector2d(s1*v2.x,s1*v2.y); }
/** @brief Provide the operator<< operation. */
inline OutputStream& operator<<(OutputStream& os, const Vector2d& v) {
    return os << "["<<v.x<<","<<v.y<<"]"; }

/** @brief Represent a point in two-dimensional display coordinates. */
struct Point2d {
    double x,y; Point2d() : x(), y() { } Point2d(double x_, double y_) : x(x_), y(y_) { }
    /** @brief Provide the operator[] operation. */
    double& operator[](SizeType i) { return (&x)[i]; } double const& operator[](SizeType i) const { return (&x)[i]; }
    template<class X, class Y> Point2d(const X& x_, const Y& y_) : x(static_cast<double>(x_)), y(static_cast<double>(y_)) { }
};
/** @brief Provide the operator== operation. */
inline Bool operator==(const Point2d& pt1, const Point2d& pt2) { return pt1.x==pt2.x && pt1.y==pt2.y; }
/** @brief Provide the operator+= operation. */
inline Point2d& operator+=(Point2d& pt, const Vector2d& v) { pt.x+=v.x; pt.y+=v.y; return pt; }
/** @brief Provide the operator-= operation. */
inline Point2d& operator-=(Point2d& pt, const Vector2d& v) { pt.x-=v.x; pt.y-=v.y; return pt; }
/** @brief Provide the operator<< operation. */
inline OutputStream& operator<<(OutputStream& os, const Point2d& pt) {
    return os << "("<<pt.x<<","<<pt.y<<")"; }

/** @brief Store rectangular display bounds. */
struct Box2d {
    double xl,xu,yl,yu; Box2d() { }
    /** @brief Construct a rectangular box from four bounds. */
    Box2d(double xl_, double xu_, double yl_, double yu_) : xl(xl_), xu(xu_), yl(yl_), yu(yu_) { }
};
/** @brief Provide the operator<< operation. */
inline OutputStream& operator<<(OutputStream& os, const Box2d& bx) {
    return os << "["<<bx.xl<<","<<bx.xu<<"]x["<<bx.yl<<","<<bx.yu<<"]"; }

/** @brief Draw a polygon from a sequence of display-coordinate vertices. */
struct Polytope2d
    : public Drawable2dInterface
{
    List<Point2d> boundary;
  public:
    /** @brief Construct a drawable polygon from its boundary vertices. */
    Polytope2d(List<Point2d> bd_) : boundary(bd_) { }
    /** @brief Return number of vertices. */
    SizeType number_of_vertices() const { return boundary.size(); }
    /** @brief Access the polygon vertices. */
    List<Point2d>& vertices() { return boundary; }
    /** @brief Access the polygon vertices. */
    List<Point2d>const& vertices() const { return boundary; }
    /** @brief Access the polygon vertices. */
    Point2d const& vertex(SizeType i) const { return boundary[i]; }

    /** @brief Create a dynamically allocated copy of this object. */
    virtual Polytope2d* clone() const { return new Polytope2d(*this); }
    /** @brief Return dimension. */
    virtual DimensionType dimension() const { return 2u; }

    /** @brief Draw the object on the selected canvas. */
    virtual Void draw(CanvasInterface& canvas, const Projection2d&) const {
        if(boundary.size()==1) { canvas.dot(boundary[0].x,boundary[0].y); return; }
        canvas.move_to(boundary[0].x,boundary[0].y);
        for(SizeType i=1; i!=boundary.size(); ++i) {
            canvas.line_to(boundary[i].x,boundary[i].y);
        }
        canvas.line_to(boundary[0].x,boundary[0].y);
        canvas.fill();
    }

    /** @brief Provide the operator+ operation. */
    Polytope2d operator+(const Vector2d& v) {
        /** @brief Provide the r operation. */
        Polytope2d r(*this); for(SizeType i=0; i!=r.boundary.size(); ++i) { r.boundary[i]+=v; } return r;
    }

    /** @brief Provide the operator<< operation. */
    friend OutputStream& operator<<(OutputStream& os, const Polytope2d& p) {
        return p._write(os);
    }

  private:
    /** @brief Provide the  write operation. */
    virtual OutputStream& _write(OutputStream& os) const { return os << "Polytope2d(boundary="<<boundary<<")"; }
};


} // namespace Ariadne


#endif // ARIADNE_GEOMETRY2D_HPP
