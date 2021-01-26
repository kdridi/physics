let socket = null
let player = null

function setup() {
	createCanvas(800, 600)
	player = null

	protobuf.load('proto/box2d.proto', (error, root) => {
		if (error === null) {
			window.Player = root.lookupType('ggj2021.Player')

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
					const { x, y } = Player.decode(new Uint8Array(buffer))
					player = player || {}
					player.x = x * 10 + 400
					player.y = 600 - 10 * y
				})
			})
			socket.addEventListener('open', (...args) => {
				console.log(args)
			})
		}
	})
}

function draw() {
	background(255, 0, 0)

	if (player !== null) {
		// console.log(player)
		ellipse(player.x, player.y, 10)
	}
}

function mousePressed(event) {
	console.log(socket)
	socket.send('message from the client')
}

function keyPressed({ code }) {
	socket.send(`keyPressed: ${code}`)
}
