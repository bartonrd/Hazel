# Interactive 3D Scene View - User Guide

## What's New

The Hazel Editor now has a fully functional 3D Scene view, similar to Unity! You can now:
- Navigate a 3D environment freely
- Create 3D objects (Cube, Sphere, Capsule)
- Select and edit objects in real-time
- Modify transforms (position, rotation, scale)
- Customize object colors

## Getting Started

### 1. Launch the Editor

Build and run the **HazelEditor** project. You'll see the editor interface with several panels:
- **Scene**: 3D viewport (this is the new feature!)
- **Hierarchy**: List of objects in your scene
- **Inspector**: Properties of selected object
- **Console**: Log messages
- **Game**: Game preview (placeholder)
- **Project**: Asset browser

### 2. Navigate the Scene View

**Focus the Scene window first** (click on it to make it active).

**Mouse Controls:**
- **Right-Click + Drag**: Rotate the camera (look around)
- **Mouse Scroll**: Zoom in/out

**Keyboard Controls:**
- **W**: Move forward
- **S**: Move backward  
- **A**: Move left
- **D**: Move right
- **E**: Move up
- **Q**: Move down

**Tips:**
- The cursor hides while rotating (right-click held)
- Camera only moves when Scene window is focused
- All keys can be held simultaneously for diagonal movement

### 3. Create Your First Object

**Using the Menu:**
1. Click **GameObject** in the menu bar
2. Select **3D Object**
3. Choose **Cube**, **Sphere**, or **Capsule**

**Using the Hierarchy:**
1. Click **Create Empty** button in Hierarchy panel
2. This creates an object with no mesh (invisible)

**What Happens:**
- Object appears at origin (0, 0, 0)
- Object is added to Hierarchy
- Object has default color (Red for Cube, Green for Sphere, Blue for Capsule)

### 4. Select and Edit Objects

**To Select:**
- Click on object name in **Hierarchy** panel
- Object becomes highlighted with white background
- **Inspector** shows object properties

**Edit Transform:**
- **Position**: Drag X/Y/Z values to move object
- **Rotation**: Drag X/Y/Z values to rotate (in degrees)
- **Scale**: Drag X/Y/Z values to resize

**Edit Appearance:**
- **Color**: Click color box to open color picker
- **Mesh**: Dropdown to change between Cube/Sphere/Capsule

**See Changes Live:**
All edits update the Scene view **immediately** - no need to press Apply or Save!

### 5. Build a Simple Scene

Let's create a simple scene with multiple objects:

1. **Create a Ground Plane:**
   - GameObject → 3D Object → Cube
   - Name it "Ground" (click in Hierarchy)
   - Set Scale: X=10, Y=0.1, Z=10
   - Set Position: Y=-1
   - Set Color: Gray

2. **Create a Character:**
   - GameObject → 3D Object → Capsule
   - Name it "Player"
   - Set Position: Y=0
   - Set Color: Blue

3. **Create Some Props:**
   - GameObject → 3D Object → Cube (name it "Box1")
   - Set Position: X=-2, Y=0
   - GameObject → 3D Object → Sphere (name it "Ball")
   - Set Position: X=2, Y=0.5

4. **Navigate Around:**
   - Use WASD to fly around your scene
   - Right-click drag to look at objects from different angles
   - Scroll to zoom in/out

## Common Workflows

### Arranging Objects

**Precise Positioning:**
1. Select object in Hierarchy
2. In Inspector, type exact Position values
3. Use small increments (0.1) for fine adjustments

**Rotating Objects:**
1. Use Rotation X/Y/Z to orient objects
2. Remember: X=pitch, Y=yaw, Z=roll
3. Try 90° increments for orthogonal rotations

**Scaling Objects:**
1. Uniform scale: Set X=Y=Z to same value
2. Non-uniform: Set different values for stretching
3. Negative scales flip objects (not recommended)

### Managing Multiple Objects

**Organization:**
- Use descriptive names for objects
- Group related objects conceptually
- Keep transforms at readable values (avoid 0.0001)

**Color Coding:**
- Use different colors to distinguish object types
- Example: Blue for interactive, Red for hazards, Green for pickups

### Performance Tips

**Sphere/Capsule Detail:**
Spheres and capsules use 32 segments by default (smooth but slower).
If you need better performance:
- Modify MeshGenerator::CreateSphere/CreateCapsule calls
- Reduce segments to 16 for simpler objects
- Increase to 64 for very smooth appearance

**Object Count:**
The editor can handle hundreds of objects, but start small:
- Test with 10-20 objects initially
- Add more if performance is good
- Each object is one draw call

## Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Create Empty Object | Ctrl+Shift+N (menu) |
| Move Forward | W |
| Move Backward | S |
| Move Left | A |
| Move Right | D |
| Move Up | E |
| Move Down | Q |
| Rotate Camera | Right Mouse Button + Drag |
| Zoom | Mouse Scroll Wheel |

## Troubleshooting

### "I can't see anything in the Scene view"

**Solution 1: Camera Position**
- The camera starts at (0, 5, 10) looking at origin
- If your objects are elsewhere, move camera with WASD
- Create objects at origin (0, 0, 0) to see them immediately

**Solution 2: Object is There but Wrong Color**
- Check object color isn't same as background (dark gray)
- Try changing color to bright red to make it visible

**Solution 3: Scene Window Not Rendering**
- Try resizing the Scene window
- Check console for error messages

### "Camera won't move"

**Solution 1: Focus**
- Click inside the Scene view window
- Window must be focused for input to work
- You'll see a highlight around focused window

**Solution 2: Keys Not Working**
- Make sure Scene view has focus (click it)
- Try pressing keys harder/longer
- Check if another window is capturing input

### "Objects appear but are black/dark"

**Solution:**
- Lighting is working, but objects may be facing away
- Rotate camera to see different angles
- Try changing object color to bright white
- Objects have Phong shading - they need light to be visible

### "Inspector changes don't show up"

**Solution 1: Update Frequency**
- Changes should be instant
- Try clicking in Scene view to refresh focus
- Check console for errors

**Solution 2: Object Not Selected**
- Make sure object is selected in Hierarchy
- Selected objects have white background
- Inspector should show "GameObject: [Name]"

### "Scene view is very small"

**Solution:**
- Drag the window edges to resize
- Framebuffer resizes automatically
- Can maximize Scene view by dragging it out

## Advanced Tips

### Camera Speed

Default camera speed is 5 units/second. To change it:
- Edit `EditorCamera.cpp`
- Find: `float m_MovementSpeed = 5.0f;`
- Change to your preferred speed (1.0 = slow, 10.0 = fast)

### Object Colors

Objects use RGBA colors:
- RGB: 0.0 to 1.0 (not 0-255)
- Alpha: Always 1.0 (fully opaque)
- Predefined colors in code can be customized

### Multiple Lights

Currently one directional light exists:
- Direction: (-0.2, -1.0, -0.3)
- Color: White
- Intensity: 1.0

To change lighting:
- Edit `EditorLayer.cpp`
- Find: `m_SceneLight->SetDirection(...)`
- Modify direction/color/intensity

## What's NOT Implemented Yet

The following features are **not yet available**:
- ❌ Click objects in Scene view to select them
- ❌ Transform gizmos (move/rotate/scale handles)
- ❌ Deleting objects (Delete key)
- ❌ Duplicating objects (Ctrl+D)
- ❌ Undo/Redo
- ❌ Scene save/load
- ❌ More primitives (plane, cylinder)
- ❌ Importing 3D models
- ❌ Textures on objects

**Current Workaround:**
- Select objects via Hierarchy instead of clicking
- Edit transforms via Inspector numbers
- Close editor to discard changes

## Next Steps

Now that you've mastered the basics:

1. **Experiment**: Create complex arrangements of objects
2. **Learn Transforms**: Practice positioning objects precisely
3. **Build Scenes**: Create meaningful layouts
4. **Share**: Show your scenes to the community!

## Getting Help

If you encounter issues:

1. **Check Console**: Look for error messages in Console panel
2. **Read Docs**: See INTERACTIVE_SCENE_IMPLEMENTATION.md for technical details
3. **Check Code**: The implementation is in HazelEditor/src/EditorLayer.cpp

## Summary

You now have a fully functional 3D Scene editor! Key points to remember:

✅ **Focus the Scene view** before using camera controls
✅ **Right-click drag** to rotate camera
✅ **WASD+QE** to fly around
✅ **Create objects** via GameObject menu
✅ **Select in Hierarchy** to edit in Inspector
✅ **Changes are instant** - no save needed

Have fun building your 3D scenes!
