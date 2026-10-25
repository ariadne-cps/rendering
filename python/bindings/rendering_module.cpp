/***************************************************************************
 *            rendering_module.cpp
 *
 *  Copyright  2026  Luca Geretti
 ****************************************************************************/

#include "pybind11.hpp"

void rendering_submodule(pybind11::module& module);

void rendering_module(pybind11::module& module) {
    rendering_submodule(module);
}
