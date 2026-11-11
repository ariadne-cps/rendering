/***************************************************************************
 *            src/rendering_manager.cpp
 *
 *  Copyright  2008-21  Mirko Albanese, Luca Geretti
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

/** @file src/rendering_manager.cpp
 *  @brief Implement the shared rendering backend manager.
 */

#include "rendering/rendering_manager.hpp"

#include "rendering/default_rendering.hpp"
#include "rendering/geometry2d.hpp"
#include "rendering/rendering_backend_interface.hpp"
#include "utility/handle.hpp"

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

const RenderingBackend& RenderingManager::backend() const {
    return _backend;
}

Void RenderingManager::set_backend(const RenderingBackend& backend) {
    _backend = backend;
}

} // namespace Ariadne
