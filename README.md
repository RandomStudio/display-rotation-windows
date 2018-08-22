# Display Rotation Windows

Utility for retrieving and changing the display rotation on Windows.

Rotation values are expressed as numbers representing the following display rotations in Windows:
- 0: Landscape
- 90: Portrait
- 180: Landscape (flipped)
- 270: Portrait (flipped)

Every function returns the current rotation value, after applying any modifications.

## Usage

```
const displayRotationWindows = require('display-rotation-windows');

// Assuming the current display rotation is 0:

// Returns 0
displayRotationWindows.getRotation();

// Rotate the display clockwise, returns 270
displayRotationWindows.rotateCW();

// Rotate the display counterclockwise, returns 90
displayRotationWindows.rotateCCW();

// Rotate the display 180 degrees, returns 180
displayRotationWindows.rotate180();
```
