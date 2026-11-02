/***************************************************************************
 *            test_rendering_utility_api.cpp
 *
 *  This file is part of Ariadne, licensed under GPL-3.0-or-later.
 ****************************************************************************/
#include <type_traits>
#include <sstream>
#include "ariadne-testing.hpp"
#include "rendering/rendering_box.hpp"
#include "rendering/figure.hpp"
#include "rendering/null_rendering.hpp"

using namespace Ariadne;

class RenderingOnlyShape final : public Drawable2dInterface {
  public:
    RenderingOnlyShape* clone() const override { return new RenderingOnlyShape(*this); }
    DimensionType dimension() const override { return 2u; }
    Void draw(CanvasInterface& canvas, Projection2d const&) const override {
        canvas.dot(0.0,0.0);
    }
};

class RenderingCountingCanvas final : public NullCanvas {
  public:
    Nat dots=0u;
    Void dot(double,double) override { ++dots; }
};

Void test_rendering_box(Ariadne::Testing::Context& ctx) {
    RenderingBox box({{-1.0,1.0},{-2.0,2.0}});
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, box.dimension(), 2u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, box[0].lower_bound(), -1.0);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, box[1].upper_bound(), 2.0);
    RenderingBox empty(0u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, empty.dimension(), 0u);
    std::ostringstream out;
    out<<box;
    ARIADNE_TESTING_PROVE_CTX(ctx, not out.str().empty());
}

Void test_rendering_figure(Ariadne::Testing::Context& ctx) {
    RenderingBox bounds({{-1.0,1.0},{-1.0,1.0}});
    Figure figure(bounds,Projection2d(2u,0u,1u));
    figure.draw(RenderingOnlyShape());
    RenderingCountingCanvas canvas;
    figure._paint_all(canvas);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, canvas.dots, 1u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, figure.get_bounding_box().dimension(), 2u);
}

int main(int argc, char** argv) {
    Ariadne::Testing::Suite suite("rendering_utility_api");
    suite.add("rendering_box", [](Ariadne::Testing::Context& ctx) { test_rendering_box(ctx); });
    suite.add("rendering_figure", [](Ariadne::Testing::Context& ctx) { test_rendering_figure(ctx); });
    return Ariadne::Testing::run_suite(suite, argc, argv);
}
