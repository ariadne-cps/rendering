/***************************************************************************
 *            rendering/rendering_box.hpp
 *
 *  Copyright  2026  Luca Geretti
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

/** @file rendering/rendering_box.hpp
 *  @brief Display-coordinate intervals and multidimensional viewing boxes.
 */

#ifndef ARIADNE_RENDERING_BOX_HPP
#define ARIADNE_RENDERING_BOX_HPP

#include <initializer_list>
#include <ostream>
#include <vector>
#include "utility/typedefs.hpp"

namespace Ariadne {

//! A pair of display-coordinate bounds, unrelated to arithmetic intervals.
struct RenderingInterval {
    double lower, upper;
    RenderingInterval(double l, double u) : lower(l), upper(u) { }
    /** @brief Return lower bound. */
    double lower_bound() const { return lower; }
    /** @brief Return upper bound. */
    double upper_bound() const { return upper; }
};
/** @brief Implement the operator<< operation. */
inline OutputStream& operator<<(OutputStream& os, RenderingInterval const& interval) {
    return os << "[" << interval.lower << "," << interval.upper << "]";
}

//! An N-dimensional viewing region containing only primitive coordinates.
class RenderingBox {
  public:
    RenderingBox() = default;
    /** @brief Perform the RenderingBox operation. */
    explicit RenderingBox(DimensionType n) : _intervals(n,RenderingInterval(-1.0,1.0)) { }
    RenderingBox(DimensionType n, RenderingInterval interval) : _intervals(n,interval) { }
    RenderingBox(std::initializer_list<RenderingInterval> intervals) : _intervals(intervals) { }

    /** @brief Return dimension. */
    DimensionType dimension() const { return _intervals.size(); }
    /** @brief Implement the operator[] operation. */
    RenderingInterval const& operator[](DimensionType i) const { return _intervals[i]; }
    /** @brief Implement the operator[] operation. */
    RenderingInterval& operator[](DimensionType i) { return _intervals[i]; }

    /** @brief Implement the operator<< operation. */
    friend OutputStream& operator<<(OutputStream& os, RenderingBox const& box) {
        os << "RenderingBox(";
        for(DimensionType i=0; i!=box.dimension(); ++i) {
            if(i!=0) os << ",";
            os << box[i];
        }
        return os << ")";
    }

  private:
    std::vector<RenderingInterval> _intervals;
};

} // namespace Ariadne

#endif // ARIADNE_RENDERING_BOX_HPP
