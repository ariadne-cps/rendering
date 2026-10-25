/***************************************************************************
 *            rendering_submodule.cpp
 *
 *  Copyright  2008-26  Luca Geretti, Pieter Collins
 ****************************************************************************/

#include "pybind11.hpp"
#include "utilities.hpp"

#include "rendering/colour.hpp"
#include "rendering/default_rendering.hpp"
#include "rendering/figure.hpp"
#include "rendering/geometry2d.hpp"
#include "rendering/image_size.hpp"
#include "rendering/rendering_manager.hpp"

using namespace Ariadne;

namespace {

class DrawableWrapper
    : public pybind11::wrapper<Drawable2dInterface>
{
  public:
    Drawable2dInterface* clone() const override { return this->get_override("clone")(); }
    Void draw(CanvasInterface& canvas, Projection2d const& projection) const override {
        this->get_override("draw")(canvas,projection);
    }
    DimensionType dimension() const override { return this->get_override("dimension")(); }
};


void export_geometry(pybind11::module& module) {
    pybind11::class_<Point2d>(module,"Point2d")
        .def(pybind11::init<double,double>())
        .def_readwrite("x",&Point2d::x)
        .def_readwrite("y",&Point2d::y)
        .def("__repr__",&__cstr__<Point2d>);

    pybind11::class_<Vector2d>(module,"Vector2d")
        .def(pybind11::init<double,double>())
        .def_readwrite("x",&Vector2d::x)
        .def_readwrite("y",&Vector2d::y)
        .def("__repr__",&__cstr__<Vector2d>);

    pybind11::class_<Projection2d>(module,"Projection2d")
        .def(pybind11::init<DimensionType,DimensionType,DimensionType>())
        .def_readwrite("n",&Projection2d::n)
        .def_readwrite("i",&Projection2d::i)
        .def_readwrite("j",&Projection2d::j);

    pybind11::class_<RenderingInterval>(module,"RenderingInterval")
        .def(pybind11::init<double,double>())
        .def_readonly("lower",&RenderingInterval::lower)
        .def_readonly("upper",&RenderingInterval::upper);

    pybind11::class_<RenderingBox>(module,"RenderingBox")
        .def(pybind11::init<DimensionType>())
        .def("dimension",&RenderingBox::dimension);

    pybind11::class_<ImageSize2d>(module,"ImageSize2d")
        .def(pybind11::init<Nat,Nat>())
        .def_readwrite("nx",&ImageSize2d::nx)
        .def_readwrite("ny",&ImageSize2d::ny);
}

void export_colour(pybind11::module& module) {
    pybind11::class_<Colour>(module,"Colour")
        .def(pybind11::init<double,double,double>())
        .def(pybind11::init<double,double,double,double>())
        .def_readonly("name",&Colour::name)
        .def_readonly("red",&Colour::red)
        .def_readonly("green",&Colour::green)
        .def_readonly("blue",&Colour::blue)
        .def_readonly("opacity",&Colour::opacity)
        .def("__str__",&__cstr__<Colour>)
        .def("__repr__",&__cstr__<Colour>);

    module.attr("transparent")=transparent;
    module.attr("white")=white;
    module.attr("black")=black;
    module.attr("red")=red;
    module.attr("green")=green;
    module.attr("blue")=blue;
    module.attr("yellow")=yellow;
    module.attr("cyan")=cyan;
    module.attr("magenta")=magenta;
    module.attr("orange")=orange;
    module.attr("grey")=grey;
    module.attr("lightgrey")=lightgrey;
    module.attr("darkgrey")=darkgrey;
}

void export_properties(pybind11::module& module) {
    static constexpr auto reference_internal=pybind11::return_value_policy::reference_internal;
    pybind11::class_<RenderingProperties>(module,"RenderingProperties")
        .def(pybind11::init<>())
        .def("set_dot_radius",&RenderingProperties::set_dot_radius,reference_internal)
        .def("set_line_style",&RenderingProperties::set_line_style,reference_internal)
        .def("set_line_width",&RenderingProperties::set_line_width,reference_internal)
        .def("set_line_colour",(RenderingProperties&(RenderingProperties::*)(double,double,double))&RenderingProperties::set_line_colour,reference_internal)
        .def("set_line_colour",(RenderingProperties&(RenderingProperties::*)(Colour))&RenderingProperties::set_line_colour,reference_internal)
        .def("set_fill_style",&RenderingProperties::set_fill_style,reference_internal)
        .def("set_fill_colour",(RenderingProperties&(RenderingProperties::*)(double,double,double))&RenderingProperties::set_fill_colour,reference_internal)
        .def("set_fill_colour",(RenderingProperties&(RenderingProperties::*)(Colour))&RenderingProperties::set_fill_colour,reference_internal)
        .def("set_fill_opacity",&RenderingProperties::set_fill_opacity,reference_internal)
        .def("set_animated",&RenderingProperties::set_animated,reference_internal)
        .def_readwrite("dot_radius",&RenderingProperties::dot_radius)
        .def_readwrite("line_style",&RenderingProperties::line_style)
        .def_readwrite("line_width",&RenderingProperties::line_width)
        .def_readwrite("line_colour",&RenderingProperties::line_colour)
        .def_readwrite("fill_style",&RenderingProperties::fill_style)
        .def_readwrite("fill_colour",&RenderingProperties::fill_colour)
        .def_readwrite("is_animated",&RenderingProperties::is_animated)
        .def("__str__",&__cstr__<RenderingProperties>)
        .def("__repr__",&__cstr__<RenderingProperties>);
}

void export_figure(pybind11::module& module) {
    static constexpr auto reference_internal=pybind11::return_value_policy::reference_internal;

    pybind11::class_<Drawable2dInterface,DrawableWrapper> drawable_class(module,"Drawable");
    drawable_class.def("clone",&Drawable2dInterface::clone);
    drawable_class.def("draw",&Drawable2dInterface::draw);
    drawable_class.def("dimension",&Drawable2dInterface::dimension);

    pybind11::class_<Figure>(module,"Figure")
        .def(pybind11::init<RenderingBoundingBoxType const&,Projection2d const&>())
        .def("set_projection_map",&Figure::set_projection_map,reference_internal)
        .def("set_bounding_box",(Figure&(Figure::*)(RenderingBoundingBoxType const&))&Figure::set_bounding_box,reference_internal)
        .def("set_projection",(Figure&(Figure::*)(DimensionType,DimensionType,DimensionType))&Figure::set_projection,reference_internal)
        .def("set_axis_labels",&Figure::set_axis_labels,reference_internal)
        .def("set_dot_radius",&Figure::set_dot_radius,reference_internal)
        .def("set_line_style",&Figure::set_line_style,reference_internal)
        .def("set_line_width",&Figure::set_line_width,reference_internal)
        .def("set_line_colour",(Figure&(Figure::*)(double,double,double))&Figure::set_line_colour,reference_internal)
        .def("set_line_colour",(Figure&(Figure::*)(Colour))&Figure::set_line_colour,reference_internal)
        .def("set_fill_style",&Figure::set_fill_style,reference_internal)
        .def("set_fill_colour",(Figure&(Figure::*)(double,double,double))&Figure::set_fill_colour,reference_internal)
        .def("set_fill_colour",(Figure&(Figure::*)(Colour))&Figure::set_fill_colour,reference_internal)
        .def("set_fill_opacity",&Figure::set_fill_opacity,reference_internal)
        .def("get_bounding_box",&Figure::get_bounding_box)
        .def("get_2d_projection_map",&Figure::get_2d_projection_map)
        .def("get_line_style",&Figure::get_line_style)
        .def("get_line_width",&Figure::get_line_width)
        .def("get_dot_radius",&Figure::get_dot_radius)
        .def("get_line_colour",&Figure::get_line_colour)
        .def("get_fill_style",&Figure::get_fill_style)
        .def("get_fill_opacity",&Figure::get_fill_opacity)
        .def("get_fill_colour",&Figure::get_fill_colour)
        .def("properties",(RenderingProperties&(Figure::*)())&Figure::properties,reference_internal)
        .def("clear",&Figure::clear,reference_internal)
        .def("set_animated",&Figure::set_animated,reference_internal)
        .def("draw",(Figure&(Figure::*)(Drawable2dInterface const&))&Figure::draw,reference_internal)
        .def("write",(Void(Figure::*)(Char const*)const)&Figure::write)
        .def("write",(Void(Figure::*)(Char const*,Nat,Nat)const)&Figure::write);
}

void export_backend(pybind11::module& module) {
    pybind11::class_<RenderingBackend>(module,"RenderingBackend")
        .def(pybind11::init<DefaultRenderingBackend>());

    pybind11::class_<DefaultRenderingBackend>(module,"DefaultRenderingBackend")
        .def(pybind11::init<>());

    pybind11::implicitly_convertible<DefaultRenderingBackend,RenderingBackend>();

    auto const reference=pybind11::return_value_policy::reference;
    pybind11::class_<RenderingManager>(module,"RenderingManager")
        .def_static("instance",&RenderingManager::instance,reference)
        .def("backend",&RenderingManager::backend,reference)
        .def("set_backend",&RenderingManager::set_backend);
}

} // namespace

void rendering_submodule(pybind11::module& module) {
    export_geometry(module);
    export_colour(module);
    export_properties(module);
    export_figure(module);
    export_backend(module);
}
