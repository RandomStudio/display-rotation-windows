const addon = require('bindings')('addon.node');

if (process.argv.includes('getRotation')) {
	console.log('rotation:', addon.getRotation());
}

if (process.argv.includes('rotateCW')) {
	console.log('rotation:', addon.rotateCW());
}

if (process.argv.includes('rotateCCW')) {
	console.log('rotation:', addon.rotateCCW());
}

if (process.argv.includes('rotate180')) {
	console.log('rotation:', addon.rotate180());
}
