let socket = null
let world = null

function setup() {
	createCanvas(800, 600, WEBGL)
	world = null

	protobuf.load('proto/box2d.proto', (error, root) => {
		if (error === null) {
			window.b2World = root.lookupType('ggj2021.b2World')

			socket = new WebSocket(`ws://${window.location.hostname}:9091`)

			socket.addEventListener('close', (...args) => {
				console.log(args)
			})
			socket.addEventListener('error', (...args) => {
				console.log(args)
			})
			socket.addEventListener('message', (...args) => {
				const { data } = args.shift()
				data.arrayBuffer().then((buffer) => {
					world = b2World.decode(new Uint8Array(buffer))
				})
			})
			socket.addEventListener('open', (...args) => {
				console.log(args)
			})
		}
	})
}

function draw() {
	background(0)

	const w2p = (vec) => {
		// console.log(vec)
		const x = 0 + vec.x * 9
		const y = 280 - vec.y * 9
		return { x, y }
	}

	if (world !== null) {
		// console.log(world)
		world.circles.forEach(({ center }) => {
			const { x, y } = w2p(center)
			ellipse(x, y, 10)
		})
		world.points.forEach(({ position }) => {
			const { x, y } = w2p(position)
			ellipse(x, y, 10)
		})
		world.polygons.forEach((polygon) => {
			const {
				solid,
				vertices,
				color: { r, g, b, a },
			} = polygon
			fill.apply(
				this,
				[r, g, b, a].map((x) => Math.floor(x * 256))
			)
			beginShape()
			vertices.forEach((vertice) => {
				const { x, y } = w2p(vertice)
				vertex(x, y)
			})
			endShape(CLOSE)
		})
		world.segments.forEach(({ p1, p2 }) => {
			{
				const { x, y } = w2p(p1)
				ellipse(x, y, 10)
			}
			{
				const { x, y } = w2p(p2)
				ellipse(x, y, 10)
			}
		})
	}
}

function mousePressed(event) {
	console.log(socket)
	socket.send('message from the client')
}

function keyPressed({ code }) {
	socket.send(`keyPressed: ${code}`)
}
