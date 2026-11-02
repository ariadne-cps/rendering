/*
 * This file is part of Ariadne, licensed under GPL-3.0-or-later.
 *
 * Standalone consumer smoke test: include only the public umbrella header.
 */

#include "ariadne-rendering.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

int main(int argc, char** argv)
{
    Ariadne::Testing::Suite suite("public_header");
    suite.add("public umbrella header", [](Ariadne::Testing::Context& ctx) {
    RenderingBox box({{-1.0,1.0},{-2.0,2.0}});
    Figure figure(box,Projection2d(2u,0u,1u));
    figure.set_line_colour(red).set_fill_colour(blue);

    NullCanvas null_canvas;
    figure._paint_all(null_canvas);

    DefaultRenderingBackend default_backend;
    auto default_canvas=default_backend.make_canvas("unused.png",32u,32u,false);
    NullRenderingBackend null_backend;
    auto empty_canvas=null_backend.make_canvas("unused.png",32u,32u,false);

    ImageSize2d size(32u,32u);
    Point2d position(0.0,0.0);
    position+=Vector2d(1.0,2.0);

    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, box.dimension(), 2u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, figure.get_bounding_box().dimension(), 2u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, size.nx, 32u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, size.ny, 32u);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, position.x, 1.0);
    ARIADNE_TESTING_EXPECT_EQUAL_CTX(ctx, position.y, 2.0);
    ARIADNE_TESTING_PROVE_CTX(ctx, static_cast<bool>(default_canvas));
    ARIADNE_TESTING_PROVE_CTX(ctx, static_cast<bool>(empty_canvas));
    });
    return Ariadne::Testing::run_suite(suite, argc, argv);
}
