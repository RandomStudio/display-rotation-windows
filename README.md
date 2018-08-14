# Display Rotation Windows

Utility for retrieving and changing the display rotation on Windows

## Usage

```
const displayRotationWindows = require('display-rotation-windows');

// Get the current display rotation (0, 90, 180 or 270)
displayRotationWindows.getRotation();

// Rotate the display clockwise
displayRotationWindows.rotateCW();

// Rotate the display counterclockwise
displayRotationWindows.rotateCCW();

// Rotate the display 180 degrees
displayRotationWindows.rotate180();
```
