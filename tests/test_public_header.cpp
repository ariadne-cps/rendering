/*
 * This file is part of Ariadne, licensed under GPL-3.0-or-later.
 *
 * Standalone consumer smoke test: include only the public umbrella header.
 */

#include "ariadne-rendering.hpp"

using namespace Ariadne;

int main()
{
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

    return box.dimension()==2u
        && figure.get_bounding_box().dimension()==2u
        && size.nx==32u && size.ny==32u
        && position.x==1.0 && position.y==2.0
        && default_canvas && empty_canvas ? 0 : 1;
}
