/***************************************************************************
 *            tests/test_public_header.cpp
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

/** @file tests/test_public_header.cpp
 *  @brief Check usability of the public umbrella header.
 */

#include "ariadne-rendering.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

class TestPublicHeader : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    void test_public_api() {
        RenderingBox box({
            {-1.0, 1.0},
            {-2.0, 2.0}
        });
        Figure figure(box, Projection2d(2u, 0u, 1u));
        figure.set_line_colour(red).set_fill_colour(blue);

        NullCanvas null_canvas;
        figure._paint_all(null_canvas);

        DefaultRenderingBackend default_backend;
        auto default_canvas = default_backend.make_canvas("unused.png", 32u, 32u, false);
        NullRenderingBackend null_backend;
        auto empty_canvas = null_backend.make_canvas("unused.png", 32u, 32u, false);

        ImageSize2d image_size(32u, 32u);
        Point2d position(0.0, 0.0);
        position += Vector2d(1.0, 2.0);

        ARIADNE_TESTING_EXPECT_EQUAL(box.dimension(), 2u);
        ARIADNE_TESTING_EXPECT_EQUAL(figure.get_bounding_box().dimension(), 2u);
        ARIADNE_TESTING_EXPECT_EQUAL(image_size.nx, 32u);
        ARIADNE_TESTING_EXPECT_EQUAL(image_size.ny, 32u);
        ARIADNE_TESTING_EXPECT_EQUAL(position.x, 1.0);
        ARIADNE_TESTING_EXPECT_EQUAL(position.y, 2.0);
        ARIADNE_TESTING_PROVE(static_cast<bool>(default_canvas));
        ARIADNE_TESTING_PROVE(static_cast<bool>(empty_canvas));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("public_header");
    ARIADNE_TESTING_ADD_TEST(TestPublicHeader, test_public_api);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
