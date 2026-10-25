/***************************************************************************
 *            rendering/rendering_box.hpp
 *
 *  This file is part of Ariadne, licensed under GPL-3.0-or-later.
 ****************************************************************************/
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
    double lower_bound() const { return lower; }
    double upper_bound() const { return upper; }
};
inline OutputStream& operator<<(OutputStream& os, RenderingInterval const& interval) {
    return os << "[" << interval.lower << "," << interval.upper << "]";
}

//! An N-dimensional viewing region containing only primitive coordinates.
class RenderingBox {
  public:
    RenderingBox() = default;
    explicit RenderingBox(DimensionType n) : _intervals(n,RenderingInterval(-1.0,1.0)) { }
    RenderingBox(DimensionType n, RenderingInterval interval) : _intervals(n,interval) { }
    RenderingBox(std::initializer_list<RenderingInterval> intervals) : _intervals(intervals) { }

    DimensionType dimension() const { return _intervals.size(); }
    RenderingInterval const& operator[](DimensionType i) const { return _intervals[i]; }
    RenderingInterval& operator[](DimensionType i) { return _intervals[i]; }

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
