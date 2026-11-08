/***************************************************************************
 *            benchmarks/rendering/benchmark_rendering.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

/*! \file benchmarks/rendering/benchmark_rendering.cpp
 *  \brief Deterministic standalone drawing performance measurements.
 */

// This benchmark deliberately drives CanvasInterface directly. In particular,
// no Figure, Drawer, constrained set, approximation, or geometry computation
// is included in the measured rendering time.

#include "rendering/default_rendering.hpp"
#include "ariadne-benchmarking.hpp"

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace Ariadne;
namespace Bench=Ariadne::Benchmarking;

namespace {
constexpr double pi = 3.14159265358979323846;

//! \brief A replayable canvas command, prepared before timing.
enum class Operation {
    move, line, circle, fill, stroke, width, line_colour, fill_colour, opacity
};

//! \brief One rendering operation and up to three numerical arguments.
struct Command {
    Operation op;
    double a=0.0, b=0.0, c=0.0;
};

//! \brief Precomputed drawing workload with its primitive count.
struct Scene {
    std::string name;
    std::vector<Command> commands;
    std::size_t primitives=0;

    void add(Operation op,double a=0.0,double b=0.0,double c=0.0) {
        commands.push_back({op,a,b,c});
    }
    void rectangle(double xl,double yl,double xu,double yu) {
        add(Operation::move,xl,yl);
        add(Operation::line,xu,yl);
        add(Operation::line,xu,yu);
        add(Operation::line,xl,yu);
        add(Operation::line,xl,yl);
        add(Operation::fill);
        ++primitives;
    }
    void circle(double x,double y,double radius) {
        add(Operation::circle,x,y,radius);
        add(Operation::fill);
        ++primitives;
    }
};

struct Options {
    unsigned width=640u,height=480u,iterations=5u;
    std::string scene="all",png_prefix="benchmark-drawing";
    bool write_png=false,breakdown=false;
};

// Use CommandLine's standard help, threading, verbosity and warm-up parsing.
List<ArgumentParser> benchmark_parsers(Options& opts) {
    List<ArgumentParser> parsers;
    auto positive=[&](String const& name,unsigned& field,unsigned max,
                      String const& help) {
        parsers.append(make_valued_argument_parser(
            "",name,help,[&field,max](String const& input)->VoidFunction {
                unsigned value=Bench::parse_unsigned<unsigned>(input);
                if(value==0u||value>max)
                    throw std::invalid_argument("rendering benchmark integer out of range");
                return [&field,value]{field=value;};
            }));
    };
    positive("width",opts.width,16384u,"Canvas width (1..16384)");
    positive("height",opts.height,16384u,"Canvas height (1..16384)");
    positive("iterations",opts.iterations,10000u,
             "Measured rounds (1..10000; default 5)");
    parsers.append(make_valued_argument_parser(
        "","scene","rectangles|polylines|circles|polygons|winding|mixed|all",
        [&opts](String const& value)->VoidFunction {
            std::string scene(value);
            if(scene!="all"&&scene!="rectangles"&&scene!="polylines"&&
               scene!="circles"&&scene!="polygons"&&scene!="winding"&&
               scene!="mixed")
                throw std::invalid_argument("invalid rendering scene: "+scene);
            return [&opts,scene]{opts.scene=scene;};
        }));
    parsers.append(make_valued_argument_parser(
        "","png-prefix","Output PNG filename prefix",
        [&opts](String const& value)->VoidFunction {
            std::string prefix(value);
            if(prefix.empty())throw std::invalid_argument("empty PNG prefix");
            return [&opts,prefix]{opts.png_prefix=prefix;};
        }));
    parsers.append(make_unvalued_argument_parser(
        "","write-png","Include PNG encoding and file output",
        [&opts]{opts.write_png=true;}));
    parsers.append(make_unvalued_argument_parser(
        "","breakdown","Run full/geometry/fill-only/stroke-only passes",
        [&opts]{opts.breakdown=true;}));
    return parsers;
}

void validate_options(Options const& opts) {
    // Include reserved plot margins when checking memory limits.
    if(static_cast<std::uint64_t>(opts.width+170u)*(opts.height+50u)>100000000ULL)
        throw std::invalid_argument("canvas too large");
}

void set_style(Scene& scene,double width,double opacity) {
    scene.add(Operation::width,width);
    scene.add(Operation::line_colour,0.08,0.12,0.36);
    scene.add(Operation::fill_colour,0.76,0.34,0.16);
    scene.add(Operation::opacity,opacity);
}

Scene rectangles() {
    Scene scene{"rectangles",{},0};
    set_style(scene,0.8,0.6);
    constexpr int columns=24,rows=16;
    for(int iy=0;iy<rows;++iy) for(int ix=0;ix<columns;++ix) {
        const double dx=1.8/columns, dy=1.8/rows;
        const double x=-0.9+ix*dx, y=-0.9+iy*dy;
        scene.rectangle(x,y,x+dx*0.82,y+dy*0.82);
    }
    return scene;
}

Scene polylines() {
    Scene scene{"polylines",{},0};
    set_style(scene,2.0,1.0);
    constexpr int paths=72, segments=24;
    for(int j=0;j<paths;++j) {
        const double offset=-0.85+1.7*j/(paths-1);
        scene.add(Operation::move,-0.95,offset);
        for(int k=1;k<=segments;++k) {
            const double x=-0.95+1.9*k/segments;
            const double y=offset+0.025*std::sin(7.0*x+0.19*j);
            scene.add(Operation::line,x,y);
        }
        scene.add(Operation::stroke);
        ++scene.primitives;
    }
    return scene;
}

Scene circles() {
    Scene scene{"circles",{},0};
    set_style(scene,1.2,0.55);
    constexpr int columns=12,rows=10;
    for(int iy=0;iy<rows;++iy) for(int ix=0;ix<columns;++ix) {
        const double x=-0.85+1.7*ix/(columns-1);
        const double y=-0.83+1.66*iy/(rows-1);
        const double radius=0.032+0.004*((ix+iy)%3);
        scene.circle(x,y,radius);
    }
    return scene;
}

Scene polygons() {
    Scene scene{"polygons",{},0};
    set_style(scene,1.0,0.65);
    constexpr int columns=10,rows=8,vertices=12;
    for(int iy=0;iy<rows;++iy) for(int ix=0;ix<columns;++ix) {
        const double cx=-0.82+1.64*ix/(columns-1);
        const double cy=-0.80+1.60*iy/(rows-1);
        for(int k=0;k<=vertices;++k) {
            const double angle=2*pi*(k%vertices)/vertices;
            const double radius=(k%2==0) ? 0.075 : 0.038;
            const double x=cx+radius*std::cos(angle);
            const double y=cy+radius*std::sin(angle);
            scene.add(k==0 ? Operation::move : Operation::line,x,y);
        }
        scene.add(Operation::fill);
        ++scene.primitives;
    }
    return scene;
}

Scene winding() {
    Scene scene{"winding",{},0};
    set_style(scene,1.0,0.7);
    constexpr int columns=9,rows=6;
    for(int iy=0;iy<rows;++iy) for(int ix=0;ix<columns;++ix) {
        const double x=-0.85+1.7*ix/(columns-1);
        const double y=-0.82+1.64*iy/(rows-1);
        const double outer=0.071,inner=0.037;
        // Outer CCW and inner CW in the same fill: requires nonzero winding.
        scene.add(Operation::move,x-outer,y-outer);
        scene.add(Operation::line,x+outer,y-outer);
        scene.add(Operation::line,x+outer,y+outer);
        scene.add(Operation::line,x-outer,y+outer);
        scene.add(Operation::line,x-outer,y-outer);
        scene.add(Operation::move,x-inner,y-inner);
        scene.add(Operation::line,x-inner,y+inner);
        scene.add(Operation::line,x+inner,y+inner);
        scene.add(Operation::line,x+inner,y-inner);
        scene.add(Operation::line,x-inner,y-inner);
        scene.add(Operation::fill);
        ++scene.primitives;
    }
    return scene;
}

std::vector<Scene> make_scenes() {
    std::vector<Scene> scenes;
    scenes.push_back(rectangles());
    scenes.push_back(polylines());
    scenes.push_back(circles());
    scenes.push_back(polygons());
    scenes.push_back(winding());
    Scene mixed{"mixed",{},0};
    for(auto const& scene:scenes) {
        mixed.commands.insert(mixed.commands.end(),scene.commands.begin(),scene.commands.end());
        mixed.primitives+=scene.primitives;
    }
    scenes.push_back(std::move(mixed));
    return scenes;
}

//! \brief Paint-suppression modes for non-additive diagnostic timing.
enum class RenderPass {
    full, geometry, fill_only, stroke_only
};

const char* pass_name(RenderPass pass) {
    switch(pass) {
        case RenderPass::full: return "full";
        case RenderPass::geometry: return "geometry";
        case RenderPass::fill_only: return "fill-only";
        case RenderPass::stroke_only: return "stroke-only";
        default: throw std::logic_error("invalid drawing benchmark pass");
    }
}

// Diagnostic passes use the exact same precomputed path commands as the
// regular benchmark. Disabling the paint, rather than skipping fill()/stroke(),
// retains path lifecycle and circle tessellation costs in each measurement.
// The default full pass keeps the original style and drawing commands.
void draw(CanvasInterface& canvas,Scene const& scene,
          RenderPass pass=RenderPass::full) {
    const bool suppress_fill=pass==RenderPass::geometry ||
                             pass==RenderPass::stroke_only;
    const bool suppress_stroke=pass==RenderPass::geometry ||
                               pass==RenderPass::fill_only;
    if(suppress_fill) canvas.set_fill_opacity(0.0);
    if(suppress_stroke) canvas.set_line_width(0.0);

    for(auto const& command:scene.commands) {
        switch(command.op) {
            case Operation::move: canvas.move_to(command.a,command.b); break;
            case Operation::line: canvas.line_to(command.a,command.b); break;
            case Operation::circle: canvas.circle(command.a,command.b,command.c); break;
            case Operation::fill: canvas.fill(); break;
            case Operation::stroke: canvas.stroke(); break;
            case Operation::width:
                canvas.set_line_width(suppress_stroke ? 0.0 : command.a);
                break;
            case Operation::line_colour:
                canvas.set_line_colour(command.a,command.b,command.c); break;
            case Operation::fill_colour:
                canvas.set_fill_colour(command.a,command.b,command.c); break;
            case Operation::opacity:
                canvas.set_fill_opacity(suppress_fill ? 0.0 : command.a);
                break;
            default: throw std::logic_error("invalid drawing benchmark command");
        }
    }
}

// Each Runner round performs the same complete canvas lifecycle as before.
// Individual stage clocks preserve the original phase boundaries, while
// Runner owns warm-ups, round count and optional per-round logging.
struct PhaseSamples {
    Bench::Samples creation,initialisation,render,export_png;
};
struct Result {
    std::string scene;
    RenderPass pass=RenderPass::full;
    std::size_t primitives=0,commands=0;
    Bench::Statistics creation,initialisation,rendering,png;
    double primitives_per_second=0.0;
};

Result benchmark(Options const& opts,Scene const& scene,
                 Bench::Runner const& runner,RenderPass pass=RenderPass::full) {
    PhaseSamples stages;
    unsigned round=0u;
    std::string const filename=opts.png_prefix+"-"+scene.name+
        (pass==RenderPass::full ? "" : std::string("-")+pass_name(pass))+".png";
    auto const elapsed=runner.run(scene.name+"/"+pass_name(pass),opts.iterations,
        [] {},
        [&]{
            auto const start=Bench::Clock::now();
            auto canvas=DefaultRenderingBackend().make_canvas(
                "",opts.width,opts.height,false);
            auto const made=Bench::Clock::now();
            canvas->initialise("x","y",-1.0,1.0,-1.0,1.0);
            auto const initialized=Bench::Clock::now();
            draw(*canvas,scene,pass);
            auto const rendered=Bench::Clock::now();
            if(opts.write_png)canvas->write(filename.c_str());
            auto const finished=Bench::Clock::now();
            if(round>=runner.warmups()) {
                stages.creation.add(Bench::elapsed_ns(start,made));
                stages.initialisation.add(Bench::elapsed_ns(made,initialized));
                stages.render.add(Bench::elapsed_ns(initialized,rendered));
                if(opts.write_png)
                    stages.export_png.add(Bench::elapsed_ns(rendered,finished));
            }
            ++round;
        },[] {});
    if(elapsed.size()!=opts.iterations||stages.render.size()!=opts.iterations)
        throw std::runtime_error("rendering sample count mismatch");
    auto creation=Bench::summarize(stages.creation);
    auto initialization=Bench::summarize(stages.initialisation);
    auto rendering=Bench::summarize(stages.render);
    auto png=opts.write_png?Bench::summarize(stages.export_png):Bench::Statistics{};
    double const throughput=rendering.median>0.0
        ? 1e9*static_cast<double>(scene.primitives)/rendering.median : 0.0;
    return {scene.name,pass,scene.primitives,scene.commands.size(),
            creation,initialization,rendering,png,throughput};
}

void print_table(Options const& opts,Bench::CommandLine const& cli,
                 std::vector<Result> const& results) {
    std::vector<Bench::TableColumn> columns={
        {"scene"},{"shapes",0u,Bench::TableAlignment::Right},
        {"commands",0u,Bench::TableAlignment::Right},
        {"create ms",0u,Bench::TableAlignment::Right},
        {"init ms",0u,Bench::TableAlignment::Right},
        {"render ms",0u,Bench::TableAlignment::Right},
        {"p95 ms",0u,Bench::TableAlignment::Right},
        {"shapes/s",0u,Bench::TableAlignment::Right}};
    if(opts.breakdown)columns.insert(columns.begin()+1u,{"pass"});
    if(opts.write_png)columns.push_back({"PNG ms",0u,Bench::TableAlignment::Right});
    auto style=Bench::TableOptions::ruled();
    style.group_columns={0u};
    Bench::Table overview(std::move(columns),style);
    for(auto const& result:results) {
        std::vector<std::string> row={
            result.scene,std::to_string(result.primitives),
            std::to_string(result.commands),
            Bench::Table::number(result.creation.median/1e6),
            Bench::Table::number(result.initialisation.median/1e6),
            Bench::Table::number(result.rendering.median/1e6),
            Bench::Table::number(result.rendering.p95/1e6),
            Bench::Table::number(result.primitives_per_second,0u)};
        if(opts.breakdown)row.insert(row.begin()+1u,pass_name(result.pass));
        if(opts.write_png)row.push_back(Bench::Table::number(result.png.median/1e6));
        overview.add_row(std::move(row));
    }
    LOGGING_PRINTLN("Drawing performance benchmark")
    LOGGING_PRINTLN("Plot: "<<opts.width<<" x "<<opts.height
        <<" px | iterations: "<<opts.iterations
        <<" | warmups: "<<cli.warmups()<<" | scene: "<<opts.scene)
    LOGGING_PRINTLN("Render excludes canvas creation, initialization and PNG I/O.")
    if(opts.breakdown) {
        LOGGING_PRINTLN("Diagnostic passes suppress paints, not path construction;"
            " independent end-to-end passes must not be added together.")
    }
    LOGGING_PRINTLN(overview.str())

    Bench::Report report("Rendering phase statistics (nanoseconds)",
                         cli.summary_fields());
    report.set_dimension_columns({
        {"Scene"},{"Pass"},{"Phase"},
        {"Shapes",0u,Bench::TableAlignment::Right}});
    report.set_table_options(style);
    for(auto const& result:results) {
        auto add=[&](char const* phase,Bench::Statistics const& stats) {
            report.add_row({result.scene,pass_name(result.pass),phase,
                            std::to_string(result.primitives)},stats);
        };
        add("creation",result.creation);
        add("initialisation",result.initialisation);
        add("rendering",result.rendering);
        if(opts.write_png)add("PNG",result.png);
    }
    report.print();
}

} // namespace

int main(int argc,const char* argv[]) {
    try {
        Options opts;
        Bench::CommandLine cli;
        cli.add(CommandLineContribution("rendering-benchmark",benchmark_parsers(opts)));
        if(!cli.acquire(argc,argv))return 0;
        validate_options(opts);
        Bench::Runner runner(cli,opts.scene=="all"?6u:1u);
        // All input geometry is prepared outside the measured rendering.
        const auto scenes=make_scenes();
        std::vector<Result> results;
        results.reserve(scenes.size()*(opts.breakdown ? 4u : 1u));
        const std::vector<RenderPass> passes=opts.breakdown ?
            std::vector<RenderPass>{RenderPass::full,RenderPass::geometry,
                                    RenderPass::fill_only,RenderPass::stroke_only} :
            std::vector<RenderPass>{RenderPass::full};
        for(auto const& scene:scenes) {
            if(opts.scene!="all" && scene.name!=opts.scene) continue;
            for(auto pass:passes)
                results.push_back(benchmark(opts,scene,runner,pass));
        }
        print_table(opts,cli,results);
        return 0;
    } catch(std::exception const& error) {
        LOGGING_PRINTLN("rendering benchmark failed: "<<error.what())
        return 1;
    }
}
