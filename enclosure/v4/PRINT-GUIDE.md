# FlightRadar desk enclosure v4

Prototype for the E32R35T-style 3.5-inch landscape ESP32 display. Units are millimeters. The flat face is 93.0 mm wide, with rounded shoulders widening to 108.6 mm over the PCB. The chamfered face opening is 79.4 × 53.2 mm and narrows to 75.4 × 50.5 mm at the display. The curved wedge retains a nominal 21° backward tilt and is 32 mm front-to-back along the display normal, down from 43 mm in v2. The replaceable rear shell uses the same seam, mounting points and USB-C opening as v3.

## Files

- **FlightRadar-fit-check.stl** checks LCD edge clearance, hole alignment and the right-side connector with a smaller print.
- **FlightRadar-bezel.stl** is the front housing and PCB mount.
- **FlightRadar-shell.stl** is the removable, vented rear wedge.
- **FlightRadar-Desk-v4.scad** contains the editable dimensions and part selector.
- **FlightRadar-CAD-preview.png** shows the exported geometry assembled.

## Fit and print

1. Print the fit-check piece flat as exported. Start with PETG, a 0.4 mm nozzle, 0.20 mm layers and three walls. Place the board in the fixture in its intended landscape orientation without forcing it. Verify all four holes, LCD stack height and that the USB-C plug seats fully. The real cable housing should pass through the opening with room to spare.
2. Print the bezel face-down as exported. Use a smooth build plate if you prefer a smooth visible bezel. Check the slicer's bridge preview over the LCD ledge and use supports if needed.
3. Print the shell seam-side down as exported. Use removable supports as needed under its rear panel. Dry-fit the parts and check that the cable, buttons and board components clear the case.
4. Try short M2.5 plastic screws through the PCB (about 4 mm) and through the rear shell (about 8 mm). Check actual engagement and tip clearance before tightening. Keep pressure off the touch glass. Thin stick-on feet can help keep the case planted.

## Reference and assumptions

The [E32R35T drawing](https://www.lcdwiki.com/res/E32R35T/E32R35T_Size.pdf) gives a 101.50 × 55.50 mm PCB, 94.50 × 47.90 mm hole spacing, and 3.20 mm PCB mounting holes. The photos establish that the USB-C socket faces outward on the right edge with the board installed as intended, behind the LCD. The socket's exact offsets, the display stack clearance and fastener engagement still need a physical fit check. The 15 × 9 mm nominal plug relief is a prototype allowance. Adjust the source parameters for your board revision and cable.

The STL meshes were checked for closed, manifold geometry. The nominal board outline, LCD outline and a centered 12 × 7 mm plug envelope were checked for interference in CAD. The four board-mount pilot holes each retain at least 1.08 mm of surrounding plastic across a 3.7 mm depth interval. This is a geometry check and does not measure printed screw strength. The case has yet to be physically printed or fitted to the pictured board.
