/***************************************************************************
 *            rendering/rendering_backend_interface.hpp
 *
 *  Copyright  2011-21  Luca Geretti, Mirko Albanese
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

/** @file rendering/rendering_backend_interface.hpp
 *  @brief Abstractions for rendering backends and their canvases.
 */

#ifndef ARIADNE_RENDERING_BACKEND_INTERFACE_HPP
#define ARIADNE_RENDERING_BACKEND_INTERFACE_HPP

#include <iosfwd>
#include "utility/pointer.hpp"
#include "utility/handle.hpp"

namespace Ariadne {

/** @brief Define the primitive drawing operations supported by canvases. */
class CanvasInterface;

//! \brief Interface for a rendering backend.
class RenderingBackendInterface {
  public:
    //! \brief Get a canvas from the backend
    virtual SharedPointer<CanvasInterface> make_canvas(const char* cfilename, Nat drawing_width, Nat drawing_height, Bool is_animated) const = 0;

    /** @brief Destroy the rendering interface. */
    virtual ~RenderingBackendInterface() = default;
};

//! \brief Handle for rendering backends
class RenderingBackend
    : public Handle<const RenderingBackendInterface>
{
  public:
    using Handle<const RenderingBackendInterface>::Handle;
    /** @brief Create canvas. */
    SharedPointer<CanvasInterface> make_canvas(const char* cfilename, Nat drawing_width, Nat drawing_height, Bool is_animated) const {
        return this->_ptr->make_canvas(cfilename,drawing_width,drawing_height, is_animated); }
};


} //namespace Ariadne

#endif /* ARIADNE_RENDERING_BACKEND_INTERFACE_HPP */
