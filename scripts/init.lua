-- Retitling the main window.
tram.ui.SetWindowTitle("Teapot Explorer v1.0")
tram.ui.SetWindowSize(640, 480)



sundir = tram.math.DIRECTION_FORWARD
sundir = sundir + tram.math.DIRECTION_SIDE + tram.math.DIRECTION_SIDE
sundir = sundir + tram.math.DIRECTION_UP + tram.math.DIRECTION_UP + tram.math.DIRECTION_UP
sundir = sundir:normalize()

-- Setting up the global lighting.
tram.render.SetSunColor(tram.math.vec3(1.0, 1.0, 1.0))
tram.render.SetSunDirection(sundir)
tram.render.SetAmbientColor(tram.math.vec3(0.4, 0.4, 0.4))
tram.render.SetScreenClearColor(tram.render.COLOR_BLACK)
