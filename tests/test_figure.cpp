/***************************************************************************
 *            tests/test_figure.cpp
 *
 *  Copyright  2008-26  Pieter Collins, Luca Geretti
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

/** @file tests/test_figure.cpp
 *  @brief Verify figure styles, projections, and drawing operations.
 */

#include <sstream>

#include "ariadne-testing.hpp"

#include "rendering/figure.hpp"
#include "rendering/default_rendering.hpp"
#include "rendering/null_rendering.hpp"
#include "rendering/geometry2d.hpp"

using namespace Ariadne;

class TestDrawable final : public Drawable2dInterface {
  public:
    explicit TestDrawable(DimensionType dimension) : _dimension(dimension) { }
    TestDrawable* clone() const override { return new TestDrawable(*this); }
    Void draw(CanvasInterface&, Projection2d const&) const override { }
    DimensionType dimension() const override { return _dimension; }
  private:
    DimensionType _dimension;
};

class RecordingCanvas final : public NullCanvas {
  public:
    Void initialise(StringType x, StringType y, double, double, double, double) override {
        x_label=x;
        y_label=y;
    }

    Void set_line_width(double width) override { observed_line_width=width; }
    Void set_fill_opacity(double opacity) override { observed_fill_opacity=opacity; }

    StringType x_label;
    StringType y_label;
    double observed_line_width=-1.0;
    double observed_fill_opacity=-1.0;
};

class TestRendering : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;
    void test_point2d() {
        ARIADNE_TESTING_INSPECT("Point2d(1,1)", Point2d(1,1));
    }

    void test_geometry2d() {
        Vector2d v1(1.0,2.0);
        Vector2d v2(3.0,4.0);

        Vector2d neg=-v1;
        ARIADNE_TESTING_EXPECT_EQUAL(neg.x, -1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(neg.y, -2.0);

        Vector2d sum=v1+v2;
        ARIADNE_TESTING_EXPECT_EQUAL(sum.x, 4.0);
        ARIADNE_TESTING_EXPECT_EQUAL(sum.y, 6.0);

        Vector2d difference=v2-v1;
        ARIADNE_TESTING_EXPECT_EQUAL(difference.x, 2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(difference.y, 2.0);

        Vector2d scaled=2.0*v1;
        ARIADNE_TESTING_EXPECT_EQUAL(scaled.x, 2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(scaled.y, 4.0);

        std::ostringstream vector_stream;
        vector_stream << v1;
        ARIADNE_TESTING_EXPECT_EQUAL(vector_stream.str(), "[1,2]");

        Point2d default_point;
        default_point.x=0.0;
        default_point.y=0.0;
        ARIADNE_TESTING_EXPECT_EQUAL(default_point.x, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(default_point.y, 0.0);

        Point2d point(1.0,2.0);
        point[0]=3.0;
        Point2d const& const_point=point;
        ARIADNE_TESTING_EXPECT_EQUAL(const_point[0], 3.0);

        Point2d equal_point(3.0,2.0);
        Point2d different_y(3.0,4.0);
        Point2d different_x(4.0,2.0);
        ARIADNE_TESTING_PROVE(point==equal_point);
        ARIADNE_TESTING_PROVE(!(point==different_y));
        ARIADNE_TESTING_PROVE(!(point==different_x));

        point-=Vector2d(1.0,1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(point.x, 2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(point.y, 1.0);

        Box2d box;
        box.xl=-1.0; box.xu=1.0; box.yl=-2.0; box.yu=2.0;
        std::ostringstream box_stream;
        box_stream << box;
        ARIADNE_TESTING_EXPECT_EQUAL(box_stream.str(), "[-1,1]x[-2,2]");

        Polytope2d polytope(List<Point2d>{Point2d(0.0,0.0),Point2d(1.0,1.0)});
        ARIADNE_TESTING_EXPECT_EQUAL(polytope.number_of_vertices(), 2u);
        polytope.vertices()[0]=Point2d(2.0,3.0);
        Polytope2d const& const_polytope=polytope;
        ARIADNE_TESTING_EXPECT_EQUAL(const_polytope.vertices().size(), 2u);
        ARIADNE_TESTING_EXPECT_EQUAL(const_polytope.vertex(0).x, 2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(const_polytope.vertex(0).y, 3.0);
    }

    void test_geometry_paths() {
        Polytope2d single(List<Point2d>{Point2d(2.0,3.0)});
        Polytope2d triangle(List<Point2d>{Point2d(0.0,0.0),Point2d(2.0,0.0),Point2d(0.0,2.0)});
        NullCanvas canvas;
        Projection2d projection(2u,0,1);
        ARIADNE_TESTING_EXPECT_EQUAL(single.dimension(), 2u);
        ARIADNE_TESTING_EXPECT_EQUAL(triangle.dimension(), 2u);
        single.draw(canvas,projection);
        triangle.draw(canvas,projection);
        std::unique_ptr<Polytope2d> copy(triangle.clone());
        ARIADNE_TESTING_EXPECT_EQUAL(copy->number_of_vertices(), 3u);
        Polytope2d shifted=triangle+Vector2d(3.0,-1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(shifted.vertex(0).x, 3.0);
        ARIADNE_TESTING_EXPECT_EQUAL(shifted.vertex(0).y, -1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(shifted.vertex(1).x, 5.0);
        ARIADNE_TESTING_EXPECT_EQUAL(shifted.vertex(2).y, 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(triangle.vertex(0).x, 0.0);
        std::ostringstream stream;
        stream << shifted;
        ARIADNE_TESTING_PROVE(stream.str().find("Polytope2d(boundary=")==0u);
        DefaultRenderingCanvas filled(ImageSize2d(32u,32u));
        filled.initialise("x","y",-1.0,5.0,-2.0,3.0);
        filled.fill_boundary(triangle.vertices());
    }

    void test_colour() {
        ARIADNE_TESTING_INSPECT("Colour(1,1,1,0)", Colour(1,1,1,0));
        ARIADNE_TESTING_INSPECT("Colour(\"white\",1,1,1)", Colour("white",1,1,1));
    }

    void test_rendering_properties() {
        RenderingProperties p;
        p.set_dot_radius(2.0)
         .set_line_colour(0,0,1)
         .set_fill_opacity(0.5)
         .set_fill_colour(0,1,0);
        ARIADNE_TESTING_EXPECT_EQUAL(p.dot_radius, 2.0);
        ARIADNE_TESTING_INSPECT("p", p);
    }

    void test_figure_header_helpers() {
        LineStyle line_style_value=line_style(false);
        ARIADNE_TESTING_EXPECT_EQUAL(static_cast<Bool>(line_style_value), false);

        LineWidth line_width_value=line_width(2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(static_cast<Dbl>(line_width_value), 2.0);

        LineColour direct_line_colour(0.1,0.2,0.3);

        FillStyle fill_style_value=fill_style(false);
        ARIADNE_TESTING_EXPECT_EQUAL(static_cast<Bool>(fill_style_value), false);

        FillOpacity fill_opacity_value=fill_opacity(0.25);
        ARIADNE_TESTING_EXPECT_EQUAL(static_cast<Dbl>(fill_opacity_value), 0.25);

        FillColour direct_fill_colour(0.4,0.5,0.6);

        Animated animated_value=set_animated(true);
        ARIADNE_TESTING_EXPECT_EQUAL(static_cast<Bool>(animated_value), true);

        RenderingProperties properties(false,2.0,3.0,black,false,white);
        ARIADNE_TESTING_EXPECT_EQUAL(properties.line_style, false);
        ARIADNE_TESTING_EXPECT_EQUAL(properties.line_width, 2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(properties.dot_radius, 3.0);
        ARIADNE_TESTING_EXPECT_EQUAL(properties.fill_style, false);

        RenderingInterval interval(1.0,2.0);
        ARIADNE_TESTING_EXPECT_EQUAL(interval.lower_bound(), 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(interval.upper_bound(), 2.0);

        RenderingBoundingBoxType bounding_box({{0.0,1.0},{0.0,1.0}});
        Figure figure(bounding_box,0,1);
        figure << line_style_value
               << line_width_value
               << direct_line_colour
               << fill_style_value
               << fill_opacity_value
               << direct_fill_colour
               << animated_value;
    }

    void test_rendering_interface() {
        RenderingBoundingBoxType bx({{0.0,1.0},{0.0,1.0}});
        Figure figure(bx,0,1);
        Polytope2d shape(List<Point2d>{Point2d(0.0,0.0)});
        FigureInterface& figure_interface=figure;
        Drawable2dInterface const& drawable=shape;
        draw(figure_interface,drawable);
    }

    void test_rendering_base() {
        DefaultRenderingCanvas canvas(ImageSize2d(32u,32u));
        List<Point2d> boundary{Point2d(0.0,0.0)};
        canvas.fill_boundary(boundary);
    }

    void test_default_rendering() {
        ARIADNE_TESTING_EXPECT_EXCEPTION(ImageSize2d(0,1));
        ARIADNE_TESTING_EXPECT_EXCEPTION(ImageSize2d(1,0));
        DefaultRenderingCanvas canvas(ImageSize2d(32u,32u));
        canvas.initialise("x","y",-1.0,1.0,-1.0,1.0);
        canvas.set_dot_radius(4.0);
        canvas.circle(0.0,0.0,0.25);
        canvas.dot(0.5,0.5);
        canvas.stroke();
        canvas.finalise();
        canvas.write("test_default_rendering-explicit.png");
    }

    void test_construct_figure() {
        RenderingBoundingBoxType bx3({{0.0,1.0},{-1.0,0.0},{-2.0,2.0}});
        ARIADNE_TESTING_EXPECT_NO_THROW(Figure(bx3,0,1));
        ARIADNE_TESTING_EXPECT_NO_THROW(Figure(bx3,{0,1}));
        ARIADNE_TESTING_EXPECT_NO_THROW(Figure(bx3,Projection2d(3,0,1)));
        RenderingBoundingBoxType bx1({{0.0,1.0}});
        ARIADNE_TESTING_EXPECT_EXCEPTION(Figure(bx1,0,1));
        ARIADNE_TESTING_EXPECT_EXCEPTION(Figure(bx1,{0,1}));
        ARIADNE_TESTING_EXPECT_EXCEPTION(Figure(bx1,Projection2d(2,0,1)));
        Figure fig(bx3,0,1);
        ARIADNE_TESTING_INSPECT("fig.get_bounding_box()", fig.get_bounding_box());
        auto const& p=fig.properties();
        ARIADNE_TESTING_INSPECT("p", p);
    }

    void test_figure_projection() {
        RenderingBoundingBoxType bx({{0.0,1.0},{-1.0,0.0},{-2.0,2.0}});
        Figure f(bx,0,1);
        ARIADNE_TESTING_EXPECT_EXCEPTION(f.set_projection(4,0,1));
        ARIADNE_TESTING_EXPECT_NO_THROW(f.set_projection(3,0,1));
        ARIADNE_TESTING_EXPECT_EXCEPTION(f.set_projection_map(Projection2d(4,0,1)));
        ARIADNE_TESTING_EXPECT_NO_THROW(f.set_projection_map(Projection2d(3,0,1)));
        ARIADNE_TESTING_INSPECT("f.get_2d_projection_map()", f.get_2d_projection_map());
    }

    void test_subclassed_rendering_construction() {
        struct DerivedColour : Colour {
            DerivedColour() : Colour() { }
        };
        struct DerivedFigure : Figure {
            DerivedFigure(RenderingBoundingBoxType const& box,
                          DimensionType x,DimensionType y) : Figure(box,x,y) { }
            DerivedFigure(RenderingBoundingBoxType const& box,
                          Pair<DimensionType,DimensionType> xy) : Figure(box,xy) { }
        };

        DerivedColour no_paint;
        ARIADNE_TESTING_EXPECT_EQUAL(no_paint.opacity, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(no_paint.red, 1.0);

        RenderingBoundingBoxType box({{-1.0,1.0},{-2.0,2.0}});
        DerivedFigure from_coordinates(box,DimensionType(0),DimensionType(1));
        DerivedFigure from_pair(box,Pair<DimensionType,DimensionType>{0,1});
        ARIADNE_TESTING_EXPECT_EQUAL(from_coordinates.get_bounding_box().dimension(), 2u);
        ARIADNE_TESTING_EXPECT_EQUAL(from_pair.get_bounding_box().dimension(), 2u);

        from_coordinates.set_axis_labels("derived x","derived y");
        RecordingCanvas canvas;
        from_coordinates._paint_all(canvas);
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.x_label, StringType("derived x"));
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.y_label, StringType("derived y"));
    }

    void test_figure_public_api() {
        RenderingBoundingBoxType bx({{-1.0,1.0},{-1.0,1.0}});
        Figure figure(bx,0,1);

        figure.set_dot_radius(3.0)
              .set_line_style(false)
              .set_fill_style(false)
              .set_animated(true);

        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_dot_radius(), 3.0);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_line_style(), false);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_line_width(), 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_fill_style(), false);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_fill_opacity(), 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_line_colour().red, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_fill_colour().red, 1.0);

        Figure const& const_figure=figure;
        ARIADNE_TESTING_EXPECT_EQUAL(const_figure.properties().is_animated, true);

        figure.set_axis_labels("horizontal","vertical");
        RecordingCanvas canvas;
        figure._paint_all(canvas);
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.x_label, StringType("horizontal"));
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.y_label, StringType("vertical"));
    }

    void test_figure_additional_api() {
        RenderingBoundingBoxType bounds({{-1.0,1.0},{-1.0,1.0}});
        Figure figure(bounds,0,1);
        figure.set_fill_opacity(0.375);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_fill_opacity(), 0.375);
        figure.set_fill_colour(0.2,0.3,0.4);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_fill_opacity(), 0.375);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_fill_colour().green, 0.3);
        figure.set_line_colour(0.4,0.5,0.6);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_line_colour().blue, 0.6);
        TestDrawable drawable(2u);
        figure << drawable;
        RecordingCanvas canvas;
        figure._paint_all(canvas);
        figure.clear();
        figure._paint_all(canvas);
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.x_label, StringType("x0"));
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.y_label, StringType("x1"));
        draw(figure);
        LineColour from_colour=line_colour(red);
        LineColour from_components=line_colour(0.25,0.5,0.75);
        FillColour fill_from_colour=fill_colour(blue);
        FillColour fill_from_components=fill_colour(0.75,0.5,0.25);
        ARIADNE_TESTING_EXPECT_EQUAL(from_colour.red, 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(from_components.blue, 0.75);
        ARIADNE_TESTING_EXPECT_EQUAL(fill_from_colour.blue, 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(fill_from_components.red, 0.75);
    }

    void test_figure_remaining_paths() {
        RenderingBoundingBoxType bounds({{-1.0,1.0},{-1.0,1.0}});
        Figure figure(bounds,0,1);
        TestDrawable shape(2u);
        figure.set_line_style(false).set_fill_style(false);
        draw(figure,static_cast<Drawable2dInterface const&>(shape));
        Figure const& same_figure=figure;
        figure=same_figure;
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_bounding_box().dimension(), 2u);
        RecordingCanvas canvas;
        figure._paint_all(canvas);
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.observed_line_width, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(canvas.observed_fill_opacity, 0.0);
        figure.write("test_figure_remaining-default.png");
        List<Pair<Colour,Drawable2dInterface const&>> shapes{
            Pair<Colour,Drawable2dInterface const&>(red,shape)
        };
        plot("test_figure_remaining-plot.png",Projection2d(2u,0,1),bounds,shapes);
    }

    void test_figure_copy_move() {
        RenderingBoundingBoxType bounds({{-1.0,1.0},{-2.0,2.0}});
        Figure original(bounds,0,1);
        original.set_axis_labels("x","y").set_dot_radius(3.0);

        Figure copy(original);
        copy.set_dot_radius(5.0);
        ARIADNE_TESTING_EXPECT_EQUAL(original.get_dot_radius(), 3.0);
        ARIADNE_TESTING_EXPECT_EQUAL(copy.get_dot_radius(), 5.0);

        Figure assigned(bounds,0,1);
        assigned=original;
        assigned.set_line_width(7.0);
        ARIADNE_TESTING_EXPECT_EQUAL(original.get_line_width(), 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(assigned.get_line_width(), 7.0);

        Figure moved(std::move(copy));
        ARIADNE_TESTING_EXPECT_EQUAL(moved.get_dot_radius(), 5.0);

        Figure move_assigned(bounds,0,1);
        move_assigned=std::move(assigned);
        ARIADNE_TESTING_EXPECT_EQUAL(move_assigned.get_line_width(), 7.0);
    }

    void test_figure_edge_paths() {
        RenderingBoundingBoxType bx2({{-1.0,1.0},{-1.0,1.0}});
        RenderingBoundingBoxType bx3({{-1.0,1.0},{-1.0,1.0},{-1.0,1.0}});
        NullCanvas canvas;

        Figure fallback(bx2,0,1);
        fallback.set_bounding_box(RenderingBoundingBoxType(0));
        fallback._paint_all(canvas);

        Figure mismatched_box(bx2,0,1);
        mismatched_box.set_bounding_box(bx3);
        ARIADNE_TESTING_EXPECT_EXCEPTION(mismatched_box._paint_all(canvas));

        Figure invalid_x(bx2,Projection2d(2,2,1));
        ARIADNE_TESTING_EXPECT_EXCEPTION(invalid_x._paint_all(canvas));

        Figure invalid_y(bx2,Projection2d(2,0,2));
        ARIADNE_TESTING_EXPECT_EXCEPTION(invalid_y._paint_all(canvas));

        Figure empty_shape_figure(bx2,0,1);
        TestDrawable empty_shape(0u);
        empty_shape_figure.draw(empty_shape);
        empty_shape_figure._paint_all(canvas);

        Figure mismatched_shape_figure(bx2,0,1);
        TestDrawable mismatched_shape(3u);
        mismatched_shape_figure.draw(mismatched_shape);
        ARIADNE_TESTING_EXPECT_EXCEPTION(mismatched_shape_figure._paint_all(canvas));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("figure");
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_point2d);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_geometry2d);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_geometry_paths);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_colour);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_rendering_properties);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_header_helpers);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_rendering_interface);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_rendering_base);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_default_rendering);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_construct_figure);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_projection);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_subclassed_rendering_construction);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_public_api);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_additional_api);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_remaining_paths);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_copy_move);
    ARIADNE_TESTING_ADD_TEST(TestRendering, test_figure_edge_paths);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
