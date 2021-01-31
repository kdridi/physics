let socket = null
let state = null
let font = null
let lastScorer = ''
let leaderboard = []

function preload() {
	font = loadFont('arial.ttf')
}

function setup() {
	createCanvas(800, 600, WEBGL)
	state = null

	protobuf.load('proto/box2d.proto', (error, root) => {
		if (error === null) {
			window.s2c = root.lookupType('ggj2021.s2c')
			window.c2s = root.lookupType('ggj2021.c2s')

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
					state = s2c.decode(new Uint8Array(buffer))
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

	if (state !== null) {
		const { world, scorer, players } = state
		if (scorer !== '') {
			lastScorer = scorer
		}

		if (players.length > 0) {
			leaderboard = players
			console.log(leaderboard)
			leaderboard.sort((a, b) => b.score - a.score)
		}

		let i = 0 - (height / 2 - 40)
		if (font !== null) {
			fill(255)
			textFont(font, 18)
			text(`Last scorer: ${lastScorer}`, 0 - (width / 2 - 30), i)
		}

		for (let index = 0; index < leaderboard.length; index++) {
			if (i === 3) {
				break
			}
			const { name, score } = leaderboard[index]
			i += 20
			text(`${name}: ${score}`, 0 - (width / 2 - 30), i)
		}

		if (world !== null) {
			// console.log(world)
			world.circles.forEach(({ center, radius }) => {
				const { x, y } = center
				ellipse(x * 10, y * 10, 20 * radius)
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
					const { x, y } = vertice
					vertex(x * 10, y * 10)
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
}

function sendEvent(playerName, keyboard, mouse) {
	socket.send(
		c2s
			.encode({
				playerName,
				keyboard,
				mouse: {
					position: {
						x: map(mouseX, 0, width, (0 - width / 2) / 10, width / 2 / 10),
						y: map(mouseY, 0, height, (0 - height / 2) / 10, height / 2 / 10),
					},
					left: mouse,
					right: false,
				},
			})
			.finish()
	)
}

function mousePressed() {
	if (mouseX < 0 || width < mouseX || mouseY < 0 || height < mouseY) {
		return
	}
	sendEvent('', '', true)
}

function keyPressed({ code, target }) {
	if (target !== document.body) {
		return
	}
	sendEvent('', code, false)
}

function updatePlayerName(event) {
	sendEvent(document.getElementById('playerName').value, '', false)
}
