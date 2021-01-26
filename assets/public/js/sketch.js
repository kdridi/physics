let socket = null
let player = null

function setup() {
	createCanvas(800, 600)
	player = { x: 0, y: 0 }

	socket = new WebSocket(`ws://${window.location.hostname}:9091`)

	socket.addEventListener('close', (...args) => {
		console.log(args)
	})
	socket.addEventListener('error', (...args) => {
		console.log(args)
	})
	socket.addEventListener('message', (...args) => {
		const { data } = args.shift()
		const [x, y] = data
			.trim()
			.split('\n')
			.map((str) => parseFloat(str))
		player.x = x
		player.y = height - y
		// console.log(args)
	})
	socket.addEventListener('open', (...args) => {
		console.log(args)
	})
}

function draw() {
	background(255, 0, 0)

	ellipse(player.x, player.y, 10)
}

function mousePressed(event) {
	console.log(socket)
	socket.send('message from the client')
}

function keyPressed({ code }) {
	socket.send(`keyPressed: ${code}`)
}
