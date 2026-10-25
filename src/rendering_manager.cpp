/***************************************************************************
 *            rendering/rendering_manager.cpp
 *
 *  Copyright  2008-21  Mirko Albanese, Luca Geretti
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

#include "utility/handle.hpp"
#include "rendering/rendering_backend_interface.hpp"
#include "rendering/rendering_manager.hpp"
#include "rendering/geometry2d.hpp"
#include "rendering/default_rendering.hpp"

namespace Ariadne {

RenderingBackend default_rendering() {
    return DefaultRenderingBackend();
}

RenderingManager::RenderingManager() : _backend(default_rendering()) {
}

RenderingManager& RenderingManager::instance() {
    static RenderingManager instance;
    return instance;
}

RenderingBackend const& RenderingManager::backend() const {
    return _backend;
}

Void RenderingManager::set_backend(RenderingBackend const& backend) {
    _backend = backend;
}

} // namespace Ariadne
