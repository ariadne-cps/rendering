/***************************************************************************
 *            rendering/image_size.hpp
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

/** @file rendering/image_size.hpp
 *  @brief Dimensions of a raster image in pixels.
 */

#ifndef ARIADNE_IMAGE_SIZE_HPP
#define ARIADNE_IMAGE_SIZE_HPP

#include "utility/macros.hpp"
#include "utility/typedefs.hpp"

namespace Ariadne {

/** @brief Represent the pixel width and height of a raster image. */
struct ImageSize2d {
    Nat nx, ny;
    /** @brief Construct a raster size from unsigned pixel dimensions. */
    ImageSize2d(Nat x, Nat y) : nx(x), ny(y) {}
    /** @brief Construct a raster size from signed positive pixel dimensions. */
    ImageSize2d(Int x, Int y) {
        ARIADNE_PRECONDITION(x > 0 && y > 0);
        nx = static_cast<Nat>(x);
        ny = static_cast<Nat>(y);
    }
};

} // namespace Ariadne
#endif // ARIADNE_IMAGE_SIZE_HPP
