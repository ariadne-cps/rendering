import pyariadne as rendering


def test_rendering_primitives():
    point=rendering.Point2d(1.0,2.0)
    assert point.x==1.0
    assert point.y==2.0
    colour=rendering.Colour(0.1,0.2,0.3)
    assert colour.opacity==1.0
    size=rendering.ImageSize2d(320,240)
    assert size.nx==320
    assert size.ny==240


def test_rendering_manager():
    backend=rendering.DefaultRenderingBackend()
    rendering.RenderingManager.instance().set_backend(backend)



def test_figure_overloads(tmp_path):
    bounds=rendering.RenderingBox(2)
    figure=rendering.Figure(bounds, rendering.Projection2d(2,0,1))

    figure.set_line_colour(0.1,0.2,0.3)
    figure.set_fill_colour(0.4,0.5,0.6)
    assert figure.get_line_colour().red==0.1
    assert figure.get_fill_colour().green==0.5

    props=rendering.RenderingProperties()
    props.set_line_colour(0.2,0.3,0.4)
    props.set_fill_colour(0.5,0.6,0.7)
    assert props.line_colour.blue==0.4
    assert props.fill_colour.red==0.5

    output=str(tmp_path / "rendering-python-overload")
    figure.write(output,64,48)
