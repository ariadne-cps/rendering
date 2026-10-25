/***************************************************************************
 *            rendering/rendering_manager.hpp
 *
 *  Copyright  2009-21  Luca Geretti, Mirko Albanese
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

/*! \file rendering/rendering_manager.hpp
 *  \brief Rendering singleton class for managing rendering backend preferences.
 */

#ifndef ARIADNE_RENDERING_MANAGER_HPP
#define ARIADNE_RENDERING_MANAGER_HPP

#include "rendering_backend_interface.hpp"

namespace Ariadne {

//! \ingroup RenderingModule
//! \brief Manager for rendering preferences and global setup
class RenderingManager {
  private:
    RenderingManager();
  public:
    //! \brief Disable copy construction
    RenderingManager(RenderingManager const&) = delete;
    //! \brief Disable copy assignment
    Void operator=(RenderingManager const&) = delete;

    //! \brief Return the static instance, lazily instantiated
    static RenderingManager& instance();

    //! \brief The backend to use
    RenderingBackend const& backend() const;
    //! \brief Set the backend
    Void set_backend(RenderingBackend const& backend);

  private:
    RenderingBackend _backend;
};

} // namespace Ariadne


#endif // ARIADNE_RENDERING_MANAGER_HPP
