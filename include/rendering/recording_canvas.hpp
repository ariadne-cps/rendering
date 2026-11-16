/***************************************************************************
 *            rendering/recording_canvas.hpp
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 */

#ifndef ARIADNE_RENDERING_RECORDING_CANVAS_HPP
#define ARIADNE_RENDERING_RECORDING_CANVAS_HPP

#include <array>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "rendering/geometry2d.hpp"
#include "rendering/rendering_interface.hpp"

namespace Ariadne {

//! A canvas that owns a stream of drawing commands instead of raster state.
//! Each instance may be populated by a different worker, then replayed by a
//! single consumer on the final canvas. Concurrent writes to one instance are
//! deliberately not supported.
class RecordingCanvas final : public CanvasInterface {
  public:
    //! Number of recorded drawing commands.
    std::size_t size() const noexcept { return _commands.size(); }
    //! True if no commands have been recorded.
    bool empty() const noexcept { return _commands.empty(); }
    //! Discard every recorded command.
    void clear() noexcept { _commands.clear(); }

    //! Replay the recorded commands in their original order.
    void replay(CanvasInterface& target) const {
        for (const auto& cmd: _commands) {
            const auto& a = cmd.args;
            switch (cmd.operation) {
                case Operation::INITIALISE:
                    target.initialise(cmd.x_label, cmd.y_label, a[0], a[1], a[2], a[3]);
                    break;
                case Operation::FINALISE:
                    target.finalise();
                    break;
                case Operation::MOVE_TO:
                    target.move_to(a[0], a[1]);
                    break;
                case Operation::LINE_TO:
                    target.line_to(a[0], a[1]);
                    break;
                case Operation::CIRCLE:
                    target.circle(a[0], a[1], a[2]);
                    break;
                case Operation::DOT:
                    target.dot(a[0], a[1]);
                    break;
                case Operation::STROKE:
                    target.stroke();
                    break;
                case Operation::FILL:
                    target.fill();
                    break;
                case Operation::FILL_BOUNDARY:
                    target.fill_boundary(cmd.boundary);
                    break;
                case Operation::LINE_WIDTH:
                    target.set_line_width(a[0]);
                    break;
                case Operation::LINE_COLOUR:
                    target.set_line_colour(a[0], a[1], a[2]);
                    break;
                case Operation::FILL_COLOUR:
                    target.set_fill_colour(a[0], a[1], a[2]);
                    break;
                case Operation::FILL_OPACITY:
                    target.set_fill_opacity(a[0]);
                    break;
                default:
                    throw std::logic_error("Unknown recording canvas command");
            }
        }
    }

    void initialise(StringType x, StringType y, double lx, double ux, double ly, double uy) override {
        _commands.push_back({
            Operation::INITIALISE, {lx, ux, ly, uy},
             std::move(x), std::move(y)
        });
    }
    void finalise() override { _commands.push_back({Operation::FINALISE}); }
    // Export is intentionally not deferred: recording is not itself a raster.
    void write(const char*) const override {
        throw std::logic_error("RecordingCanvas cannot write an image; replay into a rendering canvas first");
    }
    void move_to(double x, double y) override { _commands.push_back({
        Operation::MOVE_TO, {x, y}
 }); }
    void line_to(double x, double y) override { _commands.push_back({
        Operation::LINE_TO, {x, y}
 }); }
    void circle(double x, double y, double radius) override { _commands.push_back({
        Operation::CIRCLE, {x, y, radius}
 }); }
    void dot(double x, double y) override { _commands.push_back({
        Operation::DOT, {x, y}
 }); }
    void stroke() override { _commands.push_back({Operation::STROKE}); }
    void fill() override { _commands.push_back({Operation::FILL}); }
    void fill_boundary(const List<Point2d>& boundary) override {
        _commands.push_back({Operation::FILL_BOUNDARY, {}, {}, {}, boundary});
    }
    void set_line_width(double width) override { _commands.push_back({Operation::LINE_WIDTH, {width}}); }
    void set_line_colour(double r, double g, double b) override { _commands.push_back({
        Operation::LINE_COLOUR, {r, g, b}
 }); }
    void set_fill_colour(double r, double g, double b) override { _commands.push_back({
        Operation::FILL_COLOUR, {r, g, b}
 }); }
    void set_fill_opacity(double opacity) override { _commands.push_back({Operation::FILL_OPACITY, {opacity}}); }

  private:
    enum class Operation {
        INITIALISE,
        FINALISE,
        MOVE_TO,
        LINE_TO,
        CIRCLE,
        DOT,
        STROKE,
        FILL,
        FILL_BOUNDARY,
        LINE_WIDTH,
        LINE_COLOUR,
        FILL_COLOUR,
        FILL_OPACITY
    };
    struct Command {
        Operation operation;
        std::array<double, 4> args{};
        StringType x_label{};
        StringType y_label{};
        List<Point2d> boundary{};
    };
    std::vector<Command> _commands;
};

} // namespace Ariadne

#endif // ARIADNE_RENDERING_RECORDING_CANVAS_HPP
