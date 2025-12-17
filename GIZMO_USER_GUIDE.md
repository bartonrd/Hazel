# Transform Gizmo User Guide

## Quick Start

Transform gizmos allow you to visually manipulate objects in the Scene view by clicking and dragging interactive 3D handles.

## How to Use Gizmos

### Step 1: Select an Object

There are two ways to select an object:

1. **From Hierarchy Panel**:
   - Click on any game object in the Hierarchy panel (left side)
   - Objects with meshes (Cube, Sphere, Capsule) can have gizmos

2. **From Scene View**:
   - Left-click in the Scene view to cycle through selectable objects
   - Only objects with mesh components can be selected this way

### Step 2: Choose Gizmo Mode

Use keyboard shortcuts or toolbar buttons:

| Key | Mode      | Description                    | Handle Colors      |
|-----|-----------|--------------------------------|-------------------|
| Q/W | Translate | Move object along axes         | Red, Green, Blue  |
| E   | Rotate    | Rotate object around axes      | Red, Green, Blue  |
| R   | Scale     | Scale object along axes        | Red, Green, Blue  |

**Toolbar Buttons**: Located on the left side of the toolbar
- "Q Move" / "[Q] Move" - Translation mode
- "E Rotate" / "[E] Rotate" - Rotation mode  
- "R Scale" / "[R] Scale" - Scale mode
- Active mode is shown with brackets, e.g., "[Q] Move"

### Step 3: Manipulate the Transform

#### Translation (Move)
- **Red Arrow**: Drag to move along X-axis (left/right)
- **Green Arrow**: Drag to move along Y-axis (up/down)
- **Blue Arrow**: Drag to move along Z-axis (forward/back)
- **Planes**: Click the colored squares between axes to move along two axes at once

#### Rotation (Rotate)
- **Red Circle**: Drag to rotate around X-axis
- **Green Circle**: Drag to rotate around Y-axis
- **Blue Circle**: Drag to rotate around Z-axis

#### Scale (Scale)
- **Red Handle**: Drag to scale along X-axis
- **Green Handle**: Drag to scale along Y-axis
- **Blue Handle**: Drag to scale along Z-axis
- **Center Handle**: Drag to scale uniformly on all axes

## Camera Controls

While working with gizmos, you can still control the camera:

### Mouse Controls
- **Right-click + Drag**: Rotate camera around scene
- **Left-click**: Select objects (when not clicking on gizmo)

### Keyboard Controls
- **Hold Shift + W/A/S/D**: Move camera forward/left/back/right
- **Hold Shift + Q/E**: Move camera down/up

**Note**: When Shift is held, W/A/S/D/Q/E control camera movement instead of gizmo mode.

## Tips and Tricks

### Precise Positioning
- Use the Inspector panel to type exact values for Position, Rotation, and Scale
- The Inspector updates in real-time as you drag gizmo handles
- Changes made in Inspector also update the gizmo position

### Multiple Objects
- To work on different objects, select them one at a time
- Currently selected object is highlighted in Hierarchy panel
- Gizmo appears at the center of the selected object

### Focus on Object
- **Double-click** an object in the Hierarchy to focus the camera on it
- Camera moves to frame the object nicely in the Scene view

### Viewing Objects
- Create multiple objects using GameObject menu → 3D Object → Cube/Sphere/Capsule
- Position them apart using translate gizmo to see all objects
- Example positions:
  - Cube at (0, 0, 0)
  - Sphere at (2, 0, 0)
  - Capsule at (-2, 0, 0)

## Common Workflows

### Arranging a Scene
1. Create objects via GameObject → 3D Object menu
2. Select first object in Hierarchy
3. Press Q to enter Move mode
4. Drag handles to position the object
5. Repeat for other objects

### Duplicating Object Layout
1. Select an object
2. Note its transform values in Inspector
3. Select another object
4. Type the same transform values to match position/rotation/scale

### Building a Structure
1. Create a base object (e.g., Cube at Y=0)
2. Create another cube
3. Use Green handle (Y-axis) to position it on top
4. Use Red/Blue handles to align horizontally
5. Repeat to build upward or outward

## Troubleshooting

### Gizmo Not Appearing
- **Check selection**: Make sure an object with a mesh is selected
- **Check object type**: Camera and Light objects don't show gizmos
- **Check Scene view**: Gizmo only appears in Scene view, not Game view

### Can't Click Objects
- **Right-click might be held**: Release right mouse button
- **Gizmo might be in the way**: Switch gizmo mode with Q/E/R
- **Try Hierarchy**: Select objects from Hierarchy panel instead

### Camera Moving Instead of Object
- **Check Shift key**: Release Shift to use normal controls
- **Check Scene focus**: Click in Scene view to focus it
- **Check gizmo mode**: Press Q to ensure you're in Move mode

### Wrong Axis Moving
- **Check camera angle**: Rotate camera to see axes more clearly
- **Color coding**: Red = X, Green = Y, Blue = Z
- **Use Inspector**: Type values directly if gizmo is confusing

## Keyboard Reference

| Key       | Action                          |
|-----------|---------------------------------|
| Q         | Translate (Move) mode          |
| W         | Translate (Move) mode          |
| E         | Rotate mode                    |
| R         | Scale mode                     |
| Shift+W   | Move camera forward            |
| Shift+S   | Move camera backward           |
| Shift+A   | Move camera left               |
| Shift+D   | Move camera right              |
| Shift+Q   | Move camera down               |
| Shift+E   | Move camera up                 |

## Scene View Reference

```
+----------------------------------------------------------+
|  Scene                                            [X][_]  |
+----------------------------------------------------------+
|                                                           |
|                         [Gizmo Here]                      |
|                              |                            |
|                         Green Arrow (Y)                   |
|                              |                            |
|                              |                            |
|             Red Arrow -------+------- Blue Arrow          |
|                (X)       Object       (Z)                 |
|                                                           |
|                                                           |
|   [Left-click to select objects]                          |
|   [Right-click + drag to rotate camera]                   |
|                                                           |
+----------------------------------------------------------+
```

## Integration with Other Panels

### Hierarchy Panel
- Selected object is highlighted
- Click to select different objects
- Double-click to focus camera on object

### Inspector Panel  
- Shows transform values for selected object
- Edit Position/Rotation/Scale fields directly
- Values update as you drag gizmo handles
- Changes in Inspector move the gizmo

### Toolbar
- Shows current gizmo mode with brackets
- Click mode buttons to change gizmo
- Play/Pause/Step controls remain in center

## Future Features (Planned)

The following features are planned for future releases:

- **Snapping**: Align objects to grid or specific increments
- **Local vs World Space**: Toggle between object-relative and world-relative axes
- **Multi-Select**: Manipulate multiple objects at once
- **Gizmo Size**: Adjust gizmo size for better visibility
- **Outline Selection**: Highlight selected objects with colored outline
- **Ray Picking**: Click directly on objects to select them
- **Undo/Redo**: Undo transform changes with Ctrl+Z

## Need Help?

For more detailed technical information, see:
- `GIZMO_IMPLEMENTATION.md` - Implementation details for developers
- `README.md` - General Hazel Engine documentation
- `EDITOR.md` - Editor panel descriptions
