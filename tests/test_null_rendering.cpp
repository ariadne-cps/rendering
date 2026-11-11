/***************************************************************************
 *            tests/test_null_rendering.cpp
 *
 *  Copyright  2026  Luca Geretti
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

/** @file tests/test_null_rendering.cpp
 *  @brief Exercise the no-op canvas and rendering backend.
 */

#include <sstream>

#include "rendering/null_rendering.hpp"

#include "ariadne-testing.hpp"

using namespace Ariadne;

class TestNullRendering : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;
    void test_canvas() {
        NullCanvas canvas;

        canvas.initialise("x", "y", -1.0, 1.0, -2.0, 2.0);
        canvas.move_to(0.0, 0.0);
        canvas.line_to(1.0, 1.0);
        canvas.circle(0.5, 0.5, 0.25);
        canvas.dot(0.5, 0.5);
        canvas.stroke();
        canvas.fill();

        List<Point2d> boundary;
        canvas.fill_boundary(boundary);

        canvas.set_line_width(2.0);
        canvas.set_line_colour(0.1, 0.2, 0.3);
        canvas.set_fill_opacity(0.5);
        canvas.set_fill_colour(0.4, 0.5, 0.6);
        Vector2d scaling = canvas.scaling();
        ARIADNE_TESTING_EXPECT_EQUAL(scaling.x, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(scaling.y, 0.0);

        Box2d bounds = canvas.bounds();
        ARIADNE_TESTING_EXPECT_EQUAL(bounds.xl, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(bounds.xu, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(bounds.yl, 0.0);
        ARIADNE_TESTING_EXPECT_EQUAL(bounds.yu, 0.0);

        canvas.write("ignored");
        canvas.finalise();
    }

    void test_backend() {
        NullRenderingBackend backend;
        SharedPointer<CanvasInterface> canvas = backend.make_canvas("ignored", 640u, 480u, false);
        ARIADNE_TESTING_PROVE(canvas);
        canvas->move_to(0.0, 0.0);
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("null_rendering");
    ARIADNE_TESTING_ADD_TEST(TestNullRendering, test_canvas);
    ARIADNE_TESTING_ADD_TEST(TestNullRendering, test_backend);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
