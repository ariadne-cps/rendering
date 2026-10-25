#include <iostream>
#include <string>

#include "ariadne-rendering.hpp"

using namespace Ariadne;
using std::cout;

namespace {

void section(const std::string& title) {
    cout << "\n== " << title << " ==\n";
}

// This intentionally uses only primitive rendering coordinates, never a
// mathematical set or an adapter from the Drawing module.
class ProjectedTriangle final : public Drawable2dInterface {
  public:
    ProjectedTriangle* clone() const override { return new ProjectedTriangle(*this); }
    DimensionType dimension() const override { return 3u; }

    Void draw(CanvasInterface& canvas, Projection2d const& projection) const override {
        const double vertices[3][3] = {
            {-1.0, -0.8, -0.5}, {1.0, -0.2, 0.8}, {0.0, 1.0, 0.2}
        };
        const auto x = projection.x_coordinate();
        const auto y = projection.y_coordinate();
        canvas.move_to(vertices[0][x],vertices[0][y]);
        for (unsigned k=1; k<3; ++k) {
            canvas.line_to(vertices[k][x],vertices[k][y]);
        }
        canvas.line_to(vertices[0][x],vertices[0][y]);
        canvas.fill();
    }
};

} // namespace

int main() {
    section("Rendering backend");
    RenderingManager::instance().set_backend(DefaultRenderingBackend());

    section("Colours and reusable rendering properties");
    Colour custom_blue(0.15,0.35,0.80,0.60);
    RenderingProperties style;
    style.set_line_width(2.0)
         .set_line_colour(black)
         .set_fill_colour(custom_blue)
         .set_fill_opacity(0.60);
    cout << "colour: " << custom_blue << "\nproperties: " << style << "\n";

    section("Layering a two-dimensional figure");
    RenderingBox viewport({{-2.5,2.5},{-2.0,2.0}});
    Figure figure(viewport,Projection2d(2u,0u,1u));
    Polytope2d region_a(List<Point2d>{
        Point2d(-1.8,-1.0), Point2d(0.2,-1.0),
        Point2d(0.2,0.7), Point2d(-1.8,0.7)
    });
    Polytope2d region_b(List<Point2d>{
        Point2d(-0.4,-0.2), Point2d(1.7,-0.2),
        Point2d(1.7,1.4), Point2d(-0.4,1.4)
    });
    figure.set_fill_colour(0.20,0.60,0.90)
          .set_fill_opacity(0.45)
          .set_line_colour(0.05,0.20,0.35)
          .set_line_width(2.0)
          .draw(region_a);
    figure.set_fill_colour(0.95,0.55,0.15)
          .set_fill_opacity(0.35)
          .set_line_colour(0.45,0.20,0.05)
          .draw(region_b);
    cout << "bounding box: " << figure.get_bounding_box() << "\n";
    cout << "line width: " << figure.get_line_width() << "\n";
    cout << "fill opacity: " << figure.get_fill_opacity() << "\n";
    figure.write("tutorial_rendering_layers");
    figure.write("tutorial_rendering_layers_large",1000,700);

    section("Reusing a figure with clear()");
    figure.clear();
    figure.set_fill_colour(green).set_fill_opacity(0.30)
          .set_line_colour(darkgrey).set_line_width(1.0).draw(region_b);
    figure.write("tutorial_rendering_cleared");

    section("Projecting rendering-native three-dimensional shapes");
    RenderingBox viewport3d({{-2.0,2.0},{-2.0,2.0},{-2.0,2.0}});
    ProjectedTriangle triangle;
    Figure xy(viewport3d,Projection2d(3u,0u,1u));
    xy.set_fill_colour(cyan).set_fill_opacity(0.40).draw(triangle);
    xy.write("tutorial_rendering_projection_xy");
    Figure xz(viewport3d,Projection2d(3u,0u,2u));
    xz.set_fill_colour(magenta).set_fill_opacity(0.40).draw(triangle);
    xz.write("tutorial_rendering_projection_xz");
    cout << "xy projection: " << xy.get_2d_projection_map() << "\n";
    cout << "xz projection: " << xz.get_2d_projection_map() << "\n";

    section("One-shot plot with rendering-native objects");
    plot("tutorial_rendering_plot",viewport,blue,region_a,orange,region_b);

    section("Rendering versus Drawing");
    cout << "Rendering owns colours, figures, projections, backends and PNG output.\n"
         << "Drawing adapts mathematical sets and selects nonlinear-set drawer policies.\n";
    return 0;
}
