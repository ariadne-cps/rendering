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
