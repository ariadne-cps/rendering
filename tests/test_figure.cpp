/***************************************************************************
 *            test_figure.cpp
 *
 *  Copyright  2008-26  Pieter Collins, Luca Geretti
 ****************************************************************************/

#include <sstream>

#include "macro/testing.hpp"

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

    StringType x_label;
    StringType y_label;
};

class TestRendering {
  public:
    void test() {
        ARIADNE_TEST_CALL(test_point2d())
        ARIADNE_TEST_CALL(test_geometry2d())
        ARIADNE_TEST_CALL(test_colour())
        ARIADNE_TEST_CALL(test_rendering_properties())
        ARIADNE_TEST_CALL(test_figure_header_helpers())
        ARIADNE_TEST_CALL(test_rendering_interface())
        ARIADNE_TEST_CALL(test_rendering_base())
        ARIADNE_TEST_CALL(test_default_rendering())
        ARIADNE_TEST_CALL(test_construct_figure())
        ARIADNE_TEST_CALL(test_figure_projection())
        ARIADNE_TEST_CALL(test_figure_public_api())
        ARIADNE_TEST_CALL(test_subclassed_rendering_construction())
        ARIADNE_TEST_CALL(test_figure_copy_move())
        ARIADNE_TEST_CALL(test_figure_edge_paths())
    }

    void test_point2d() {
        ARIADNE_TEST_PRINT(Point2d(1,1))
    }

    void test_geometry2d() {
        Vector2d v1(1.0,2.0);
        Vector2d v2(3.0,4.0);

        Vector2d neg=-v1;
        ARIADNE_TEST_EQUALS(neg.x,-1.0)
        ARIADNE_TEST_EQUALS(neg.y,-2.0)

        Vector2d sum=v1+v2;
        ARIADNE_TEST_EQUALS(sum.x,4.0)
        ARIADNE_TEST_EQUALS(sum.y,6.0)

        Vector2d difference=v2-v1;
        ARIADNE_TEST_EQUALS(difference.x,2.0)
        ARIADNE_TEST_EQUALS(difference.y,2.0)

        Vector2d scaled=2.0*v1;
        ARIADNE_TEST_EQUALS(scaled.x,2.0)
        ARIADNE_TEST_EQUALS(scaled.y,4.0)

        std::ostringstream vector_stream;
        vector_stream << v1;
        ARIADNE_TEST_EQUALS(vector_stream.str(),"[1,2]")

        Point2d default_point;
        default_point.x=0.0;
        default_point.y=0.0;
        ARIADNE_TEST_EQUALS(default_point.x,0.0)
        ARIADNE_TEST_EQUALS(default_point.y,0.0)

        Point2d point(1.0,2.0);
        point[0]=3.0;
        Point2d const& const_point=point;
        ARIADNE_TEST_EQUALS(const_point[0],3.0)

        Point2d equal_point(3.0,2.0);
        Point2d different_y(3.0,4.0);
        Point2d different_x(4.0,2.0);
        ARIADNE_TEST_ASSERT(point==equal_point)
        ARIADNE_TEST_ASSERT(!(point==different_y))
        ARIADNE_TEST_ASSERT(!(point==different_x))

        point-=Vector2d(1.0,1.0);
        ARIADNE_TEST_EQUALS(point.x,2.0)
        ARIADNE_TEST_EQUALS(point.y,1.0)

        Box2d box;
        box.xl=-1.0; box.xu=1.0; box.yl=-2.0; box.yu=2.0;
        std::ostringstream box_stream;
        box_stream << box;
        ARIADNE_TEST_EQUALS(box_stream.str(),"[-1,1]x[-2,2]")

        Polytope2d polytope(List<Point2d>{Point2d(0.0,0.0),Point2d(1.0,1.0)});
        ARIADNE_TEST_EQUALS(polytope.number_of_vertices(),2u)
        polytope.vertices()[0]=Point2d(2.0,3.0);
        Polytope2d const& const_polytope=polytope;
        ARIADNE_TEST_EQUALS(const_polytope.vertices().size(),2u)
        ARIADNE_TEST_EQUALS(const_polytope.vertex(0).x,2.0)
        ARIADNE_TEST_EQUALS(const_polytope.vertex(0).y,3.0)
    }

    void test_colour() {
        ARIADNE_TEST_PRINT(Colour(1,1,1,0))
        ARIADNE_TEST_PRINT(Colour("white",1,1,1))
    }

    void test_rendering_properties() {
        RenderingProperties p;
        p.set_dot_radius(2.0)
         .set_line_colour(0,0,1)
         .set_fill_opacity(0.5)
         .set_fill_colour(0,1,0);
        ARIADNE_TEST_EQUALS(p.dot_radius,2.0)
        ARIADNE_TEST_PRINT(p)
    }

    void test_figure_header_helpers() {
        LineStyle line_style_value=line_style(false);
        ARIADNE_TEST_EQUALS(static_cast<Bool>(line_style_value),false)

        LineWidth line_width_value=line_width(2.0);
        ARIADNE_TEST_EQUALS(static_cast<Dbl>(line_width_value),2.0)

        LineColour direct_line_colour(0.1,0.2,0.3);

        FillStyle fill_style_value=fill_style(false);
        ARIADNE_TEST_EQUALS(static_cast<Bool>(fill_style_value),false)

        FillOpacity fill_opacity_value=fill_opacity(0.25);
        ARIADNE_TEST_EQUALS(static_cast<Dbl>(fill_opacity_value),0.25)

        FillColour direct_fill_colour(0.4,0.5,0.6);

        Animated animated_value=set_animated(true);
        ARIADNE_TEST_EQUALS(static_cast<Bool>(animated_value),true)

        RenderingProperties properties(false,2.0,3.0,black,false,white);
        ARIADNE_TEST_EQUALS(properties.line_style,false)
        ARIADNE_TEST_EQUALS(properties.line_width,2.0)
        ARIADNE_TEST_EQUALS(properties.dot_radius,3.0)
        ARIADNE_TEST_EQUALS(properties.fill_style,false)

        RenderingInterval interval(1.0,2.0);
        ARIADNE_TEST_EQUALS(interval.lower_bound(),1.0)
        ARIADNE_TEST_EQUALS(interval.upper_bound(),2.0)

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
        ARIADNE_TEST_FAIL(ImageSize2d(0,1))
        ARIADNE_TEST_FAIL(ImageSize2d(1,0))
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
        ARIADNE_TEST_EXECUTE(Figure(bx3,0,1))
        ARIADNE_TEST_EXECUTE(Figure(bx3,{0,1}))
        ARIADNE_TEST_EXECUTE(Figure(bx3,Projection2d(3,0,1)))
        RenderingBoundingBoxType bx1({{0.0,1.0}});
        ARIADNE_TEST_FAIL(Figure(bx1,0,1))
        ARIADNE_TEST_FAIL(Figure(bx1,{0,1}))
        ARIADNE_TEST_FAIL(Figure(bx1,Projection2d(2,0,1)))
        Figure fig(bx3,0,1);
        ARIADNE_TEST_PRINT(fig.get_bounding_box())
        auto const& p=fig.properties();
        ARIADNE_TEST_PRINT(p)
    }

    void test_figure_projection() {
        RenderingBoundingBoxType bx({{0.0,1.0},{-1.0,0.0},{-2.0,2.0}});
        Figure f(bx,0,1);
        ARIADNE_TEST_FAIL(f.set_projection(4,0,1))
        ARIADNE_TEST_EXECUTE(f.set_projection(3,0,1))
        ARIADNE_TEST_FAIL(f.set_projection_map(Projection2d(4,0,1)))
        ARIADNE_TEST_EXECUTE(f.set_projection_map(Projection2d(3,0,1)))
        ARIADNE_TEST_PRINT(f.get_2d_projection_map())
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
        ARIADNE_TEST_EQUALS(no_paint.opacity,0.0)
        ARIADNE_TEST_EQUALS(no_paint.red,1.0)

        RenderingBoundingBoxType box({{-1.0,1.0},{-2.0,2.0}});
        DerivedFigure from_coordinates(box,DimensionType(0),DimensionType(1));
        DerivedFigure from_pair(box,Pair<DimensionType,DimensionType>{0,1});
        ARIADNE_TEST_EQUALS(from_coordinates.get_bounding_box().dimension(),2u)
        ARIADNE_TEST_EQUALS(from_pair.get_bounding_box().dimension(),2u)

        from_coordinates.set_axis_labels("derived x","derived y");
        RecordingCanvas canvas;
        from_coordinates._paint_all(canvas);
        ARIADNE_TEST_EQUALS(canvas.x_label,StringType("derived x"))
        ARIADNE_TEST_EQUALS(canvas.y_label,StringType("derived y"))
    }

    void test_figure_public_api() {
        RenderingBoundingBoxType bx({{-1.0,1.0},{-1.0,1.0}});
        Figure figure(bx,0,1);

        figure.set_dot_radius(3.0)
              .set_line_style(false)
              .set_fill_style(false)
              .set_animated(true);

        ARIADNE_TEST_EQUALS(figure.get_dot_radius(),3.0)
        ARIADNE_TEST_EQUALS(figure.get_line_style(),false)
        ARIADNE_TEST_EQUALS(figure.get_line_width(),1.0)
        ARIADNE_TEST_EQUALS(figure.get_fill_style(),false)
        ARIADNE_TEST_EQUALS(figure.get_fill_opacity(),1.0)
        ARIADNE_TEST_EQUALS(figure.get_line_colour().red,0.0)
        ARIADNE_TEST_EQUALS(figure.get_fill_colour().red,1.0)

        Figure const& const_figure=figure;
        ARIADNE_TEST_EQUALS(const_figure.properties().is_animated,true)

        figure.set_axis_labels("horizontal","vertical");
        RecordingCanvas canvas;
        figure._paint_all(canvas);
        ARIADNE_TEST_EQUALS(canvas.x_label,StringType("horizontal"))
        ARIADNE_TEST_EQUALS(canvas.y_label,StringType("vertical"))
    }

    void test_figure_copy_move() {
        RenderingBoundingBoxType bounds({{-1.0,1.0},{-2.0,2.0}});
        Figure original(bounds,0,1);
        original.set_axis_labels("x","y").set_dot_radius(3.0);

        Figure copy(original);
        copy.set_dot_radius(5.0);
        ARIADNE_TEST_EQUALS(original.get_dot_radius(),3.0)
        ARIADNE_TEST_EQUALS(copy.get_dot_radius(),5.0)

        Figure assigned(bounds,0,1);
        assigned=original;
        assigned.set_line_width(7.0);
        ARIADNE_TEST_EQUALS(original.get_line_width(),1.0)
        ARIADNE_TEST_EQUALS(assigned.get_line_width(),7.0)

        Figure moved(std::move(copy));
        ARIADNE_TEST_EQUALS(moved.get_dot_radius(),5.0)

        Figure move_assigned(bounds,0,1);
        move_assigned=std::move(assigned);
        ARIADNE_TEST_EQUALS(move_assigned.get_line_width(),7.0)
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
        ARIADNE_TEST_FAIL(mismatched_box._paint_all(canvas))

        Figure invalid_x(bx2,Projection2d(2,2,1));
        ARIADNE_TEST_FAIL(invalid_x._paint_all(canvas))

        Figure invalid_y(bx2,Projection2d(2,0,2));
        ARIADNE_TEST_FAIL(invalid_y._paint_all(canvas))

        Figure empty_shape_figure(bx2,0,1);
        TestDrawable empty_shape(0u);
        empty_shape_figure.draw(empty_shape);
        empty_shape_figure._paint_all(canvas);

        Figure mismatched_shape_figure(bx2,0,1);
        TestDrawable mismatched_shape(3u);
        mismatched_shape_figure.draw(mismatched_shape);
        ARIADNE_TEST_FAIL(mismatched_shape_figure._paint_all(canvas))
    }
};

Int main() {
    TestRendering().test();
    return ARIADNE_TEST_FAILURES;
}
