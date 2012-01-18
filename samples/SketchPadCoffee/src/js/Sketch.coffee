

define ["lib/FieldKit"], () ->

	sketch = (width, height) ->

		print "launching SketchPadCoffee"

		g = new Graphics2D()
		mouse = new fk.Vec2()
		frame = 0

		@update = ->
			frame++

		@draw = ->
			g.background 0.99

			g.stroke 1,0,0
			g.pointSize 15
			g.point mouse.x, mouse.y

			g.fill 0
			fps = Math.round(GetAverageFps())

			# watch out! a + b means something different than a +b in CS
			# see https://github.com/jashkenas/coffee-script/wiki/FAQ
			g.text("SketchPadCoffee " +fps + "fps", 15, height - 25)

		@mouseDrag = (event) ->
			mouse.set2(event.x, event.y)

		@keyDown = (event) ->
			switch event.char
				when ' ' then print('space')
				when 'r' then SaveFrame("Desktop/SketchPadCoffee_" +frame + ".png")

		undefined # to avoid returning only the last method