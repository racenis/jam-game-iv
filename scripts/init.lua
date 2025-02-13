-- Nightmare Explorer v1.0 initialization code

tram.ui.SetWindowTitle("Nightmare Explorer v1.0")
tram.ui.SetWindowSize(640, 480)

-- this directional light direction looks pretty good with our level.
-- it won't actually be seen normally, since we set the level to have interior
-- lighting, but it was useful while creating the level, before lightmapping
sundir = tram.math.DIRECTION_FORWARD
sundir = sundir - tram.math.DIRECTION_SIDE - tram.math.DIRECTION_SIDE
sundir = sundir + tram.math.DIRECTION_UP + tram.math.DIRECTION_UP + tram.math.DIRECTION_UP
sundir = sundir:normalize()

tram.render.SetSunColor(tram.math.vec3(1.0, 1.0, 1.0))
tram.render.SetSunDirection(sundir)
tram.render.SetAmbientColor(tram.math.vec3(0.4, 0.4, 0.4))
tram.render.SetScreenClearColor(tram.render.COLOR_BLACK)

-- this light will be set for the layer 2, which will be used for the item
-- animations when they are picked up
tram.render.SetSunColor(tram.math.vec3(1.0, 1.0, 1.0), 2)
tram.render.SetSunDirection(tram.math.vec3(-0.5, 2.0, 1.0):normalize(), 2)
tram.render.SetAmbientColor(tram.math.vec3(0.4, 0.4, 0.4), 2)