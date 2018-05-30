const express = require('express');
const formidable = require('formidable');
const cors = require('cors');
const low = require('lowdb');
const FileAsync = require('lowdb/adapters/FileAsync');
const uuidv1 = require('uuid/v1');
const client = require('dgram').createSocket('udp4');
const ping = require('net-ping');

// UDP messaging to the players
const send = async (ip, msg) => {
	return new Promise((resolve, reject) => {
		client.send(msg, 0, msg.length, 8888, ip, err => {
			if (err) reject(err);
			else resolve();
		});
	});
};

// ping
const session = ping.createSession();
const scan = ip => {
	return new Promise((resolve, reject) => {
		session.pingHost(ip, (err, target) => {
			if (err) resolve(target);
			else resolve();
		});
	});
};

// Create server
const app = express();
app.use(cors());
app.use(express.json());
app.use(express.static(__dirname + '/public'));

// Create database instance and start server
const adapter = new FileAsync(__dirname + '/db.json');
low(adapter)
	.then(db => {
		// Routes
		// GET /api/player
		app.get('/api/player', (req, res) => {
			const players = db.get('players').value();
			res.send(players);
		});
		// GET /api/player/:id
		app.get('/api/player/:id', (req, res) => {
			const player = db
				.get('players')
				.find({ id: req.params.id })
				.value();
			res.send(player);
		});
		// DELETE /api/player/:id
		app.delete('/api/player/:id', (req, res) => {
			const player = db
				.get('players')
				.remove({ id: req.params.id })
				.write();
			res.send({ success: true });
		});
		// POST /api/player
		app.post('/api/player', (req, res) => {
			const form = new formidable.IncomingForm();
			form.parse(req, (err, fields, files) => {
				if (err) res.status(500).send(err);
				else {
					db.get('players')
						.push(fields)
						.last()
						.assign({ id: uuidv1() })
						.write()
						.then(post => res.send(post));
				}
			});
		});

		// GET /api/play
		app.get('/api/play', (req, res) => {
			const ips = db
				.get('players')
				.value()
				.map(p => p.ip);
			const promises = [];
			ips.forEach(ip => {
				promises.push(send(ip, 'loop'));
			});
			Promise.all(promises)
				.then(() => res.send({ success: true }))
				.catch(e => res.send({ success: false, error: e }));
		});

		// GET /api/stop
		app.get('/api/stop', (req, res) => {
			const ips = db
				.get('players')
				.value()
				.map(p => p.ip);
			const promises = [];
			ips.forEach(ip => {
				promises.push(send(ip, 'stop'));
			});
			Promise.all(promises)
				.then(() => res.send({ success: true }))
				.catch(e => res.send({ success: false, error: e }));
		});

		// GET /api/scan
		app.get('/api/scan', (req, res) => {
			const ips = db
				.get('players')
				.value()
				.map(p => p.ip);
			const promises = ips.map(ip => scan(ip));
			Promise.all(promises)
				.then(r => {
					if (r) res.send({ success: false, error: r });
					res.send({ success: true });
				})
				.catch(e => res.send({ success: false, error: 'Unknown error' }));
		});

		// Set db default values
		return db.defaults({ players: [] }).write();
	})
	.then(() => {
		app.listen(3000, () => console.log('listening on port 3000'));
	});
