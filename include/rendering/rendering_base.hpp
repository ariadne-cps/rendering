/***************************************************************************
 *            rendering/rendering_base.hpp
 *
 *  Copyright  2009-21  Luca Geretti
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

/*! \file rendering/rendering_base.hpp
 *  \brief Base rendering implementation.
 */

#ifndef ARIADNE_RENDERING_BASE_HPP
#define ARIADNE_RENDERING_BASE_HPP

#include "rendering_interface.hpp"
#include "geometry2d.hpp"

namespace Ariadne {

//! \brief Base for canvas classes
class CanvasBase : public CanvasInterface {
  public:
    Void fill_boundary(List<Point2d> const& boundary) override final {
        if(boundary.size()==1) { this->dot(boundary[0].x,boundary[0].y); }

        this->move_to(boundary[0].x,boundary[0].y);
        for(SizeType i=1; i!=boundary.size(); ++i) {
            this->line_to(boundary[i].x,boundary[i].y);
        }
        this->line_to(boundary[0].x,boundary[0].y);
        this->fill();
    }
};

} // namespace Ariadne


#endif // ARIADNE_RENDERING_BASE_HPP
