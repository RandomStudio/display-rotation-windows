const addon = require('bindings')('addon.node');

const validRotations = [0, 90, 180, 270];

const errorCodes = new Map([
	[-1, 'Failed to enumerate display settings'],
	[-10, 'Failed to change display settings: The settings change was unsuccessful because the system is DualView capable.'],
	[-11, 'Failed to change display settings: An invalid set of flags was passed in.'],
	[-12, 'Failed to change display settings: The graphics mode is not supported.'],
	[-13, 'Failed to change display settings: An invalid parameter was passed in. This can include an invalid flag or combination of flags.'],
	[-14, 'Failed to change display settings: The display driver failed the specified graphics mode.'],
	[-15, 'Failed to change display settings: Unable to write settings to the registry.'],
	[-16, 'Failed to change display settings: The computer must be restarted for the graphics mode to work.'],
	[-17, 'Failed to change display settings: Unknown error'],
	[-100, 'Ended up with invalid rotation result'],
]);

const unknownError = 'Result did not match with a valid rotation or known error code';

// FUNCTIONS FOR TESTING WITH COMMAND LINE ARGUMENTS

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

// EXPOSED FUNCTIONS

module.exports = {

	getRotation: () => {
		const result = addon.getRotation();
		if (!validRotations.includes(result)) {
			throw(new Error(errorCodes.get(result) || unknownError));
		}
		return result;
	},

	rotateCW: () => {
		const result = addon.rotateCW();
		if (!validRotations.includes(result)) {
			throw(new Error(errorCodes.get(result) || unknownError));
		}
		return result;
	},

	rotateCCW: () => {
		const result = addon.rotateCCW();
		if (!validRotations.includes(result)) {
			throw(new Error(errorCodes.get(result) || unknownError));
		}
		return result;
	},

	rotate180: () => {
		const result = addon.rotate180();
		if (!validRotations.includes(result)) {
			throw(new Error(errorCodes.get(result) || unknownError));
		}
		return result;
	},
};
