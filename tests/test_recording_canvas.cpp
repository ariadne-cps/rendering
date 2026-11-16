/***************************************************************************
 *            tests/test_recording_canvas.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Rendering.
 *  Ariadne Rendering is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  Ariadne Rendering is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 */

#include <sstream>
#include <string>
#include <vector>

#include "rendering/recording_canvas.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

class TracingCanvas final : public CanvasBase {
  public:
    std::vector<std::string> events;
    void record(const char* action, double a = 0, double b = 0, double c = 0, double d = 0) {
        std::ostringstream out;
        out << action << ":" << a << ":" << b << ":" << c << ":" << d;
        events.push_back(out.str());
    }
    void initialise(StringType x, StringType y, double lx, double ux, double ly, double uy) override {
        events.push_back(std::string("initialise:") + x + ":" + y);
        record("bounds", lx, ux, ly, uy);
    }
    void finalise() override { record("finalise"); }
    void write(const char*) const override {}
    void move_to(double x, double y) override { record("move", x, y); }
    void line_to(double x, double y) override { record("line", x, y); }
    void circle(double x, double y, double r) override { record("circle", x, y, r); }
    void dot(double x, double y) override { record("dot", x, y); }
    void stroke() override { record("stroke"); }
    void fill() override { record("fill"); }
    void set_line_width(double w) override { record("width", w); }
    void set_line_colour(double r, double g, double b) override { record("line_colour", r, g, b); }
    void set_fill_colour(double r, double g, double b) override { record("fill_colour", r, g, b); }
    void set_fill_opacity(double a) override { record("opacity", a); }
};

class TestRecordingCanvas : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;
    void test_replay() {
        RecordingCanvas recording;
        TracingCanvas direct;
        auto draw = [](CanvasInterface& canvas) {
            canvas.initialise("x", "y", -1, 2, -3, 4);
            canvas.set_line_width(2);
            canvas.set_line_colour(0.1, 0.2, 0.3);
            canvas.set_fill_colour(0.4, 0.5, 0.6);
            canvas.set_fill_opacity(0.75);
            canvas.move_to(0, 0);
            canvas.line_to(1, 2);
            canvas.circle(2, 3, 4);
            canvas.dot(1, 1);
            canvas.fill();
            canvas.stroke();
            canvas.finalise();
        };
        draw(recording);
        draw(direct);
        TracingCanvas replayed;
        recording.replay(replayed);
        ARIADNE_TESTING_EXPECT_EQUAL(replayed.events, direct.events);
        ARIADNE_TESTING_PROVE(recording.size() > 0);
        recording.clear();
        ARIADNE_TESTING_PROVE(recording.empty());
        recording.replay(replayed);
        ARIADNE_TESTING_EXPECT_EQUAL(replayed.events, direct.events);
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("recording_canvas");
    ARIADNE_TESTING_ADD_TEST(TestRecordingCanvas, test_replay);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
