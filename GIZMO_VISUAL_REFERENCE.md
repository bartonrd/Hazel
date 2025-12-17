# Transform Gizmo Visual Reference

## Gizmo Appearance

### Translation Gizmo (Q or W key)
```
                    Green Arrow (Y-axis)
                           ↑
                           |
                           |
                           |
   Red Arrow ←-------------+-------------→ Blue Arrow
     (X-axis)          Object              (Z-axis)
                         
```

**How to Use:**
- Drag RED arrow → Move LEFT/RIGHT (X-axis)
- Drag GREEN arrow → Move UP/DOWN (Y-axis)  
- Drag BLUE arrow → Move FORWARD/BACK (Z-axis)

### Rotation Gizmo (E key)
```
              Green Circle
                (Y-axis)
                   ___
                 ,'   `.
     Red        /       \      Blue
    Circle     |  Object |     Circle
   (X-axis)     \       /     (Z-axis)
                 `.___,'
                   
```

**How to Use:**
- Drag RED circle → Rotate around X-axis (pitch)
- Drag GREEN circle → Rotate around Y-axis (yaw)
- Drag BLUE circle → Rotate around Z-axis (roll)

### Scale Gizmo (R key)
```
                    Green Handle
                         ■
                         |
                         |
   Red Handle            |
        ■----------------+----------------■ Blue Handle
                      Object
                         
```

**How to Use:**
- Drag RED handle → Scale along X-axis (width)
- Drag GREEN handle → Scale along Y-axis (height)
- Drag BLUE handle → Scale along Z-axis (depth)
- Drag CENTER → Scale uniformly on all axes

## Editor Layout

```
┌─────────────────────────────────────────────────────────────────────┐
│ File  Edit  Assets  GameObject  Component  Window  Help            │
├─────────────────────────────────────────────────────────────────────┤
│ [Q] Move    E Rotate    R Scale      Play  Pause  Step             │
├────────────┬───────────────────────────────────────┬────────────────┤
│            │                                       │                │
│ Hierarchy  │          Scene View                  │   Inspector    │
│            │                                       │                │
│ ▼ Main     │                                       │ GameObject:    │
│   Camera   │         ┌───── Gizmo ─────┐          │ Cube           │
│            │         │                  │          │                │
│ ▶ Light    │         │    Y             │          │ Transform      │
│            │         │    ↑             │          │ Position       │
│ ▼ Cube ←───┼─────────┤ X←-+→ Z          │          │  X: 0.0       │
│            │         │    Object        │          │  Y: 0.0       │
│ ▶ Sphere   │         │                  │          │  Z: 0.0       │
│            │         └──────────────────┘          │                │
│            │                                       │ Rotation       │
│            │  Left-click: Select object            │  X: 0.0       │
│            │  Right-drag: Rotate camera            │  Y: 0.0       │
│            │                                       │  Z: 0.0       │
│            │                                       │                │
│            │                                       │ Scale          │
│            │                                       │  X: 1.0       │
├────────────┴───────────────────────────────────────┤  Y: 1.0       │
│                  Console                           │  Z: 1.0       │
│                                                    │                │
│ [INFO] Entity selected: Cube                      │                │
└────────────────────────────────────────────────────┴────────────────┘
```

## Interaction Flow

### Selecting an Object

```
Option 1: Click in Hierarchy          Option 2: Click in Scene View
                                      
┌─────────────┐                       ┌───────────────────┐
│ Hierarchy   │                       │   Scene View      │
│             │                       │                   │
│ ▶ Camera    │                       │   [Cube]          │
│ ▶ Light     │                       │                   │
│ ▶ Cube  ←───┼─── Click              │   [Sphere]  ←─────┼─── Left-click
│             │                       │                   │
└─────────────┘                       └───────────────────┘
        ↓                                      ↓
        └──────────────┬───────────────────────┘
                       ↓
              ┌────────────────┐
              │ Object Selected│
              └────────────────┘
                       ↓
        ┌──────────────┼──────────────┐
        ↓              ↓              ↓
   Highlighted    Shows in      Gizmo appears
   in Hierarchy   Inspector     in Scene View
```

### Using a Gizmo

```
1. Select Mode                 2. Position Mouse              3. Drag Handle
   
   Press Q/W/E/R                   Hover over handle             Click and drag
        ↓                               ↓                             ↓
   ┌──────────┐                   ┌──────────┐                  ┌──────────┐
   │ Toolbar  │                   │  Gizmo   │                  │  Gizmo   │
   │ [Q] Move │                   │    ↑     │                  │    ↑     │
   └──────────┘                   │ ←─+─→    │                  │ ←─+──→   │
                                  │   Cube   │                  │  Cube    │
                                  │  (red    │                  │  (moved) │
                                  │  glows)  │                  │          │
                                  └──────────┘                  └──────────┘
                                                                      ↓
                                                              ┌───────────────┐
                                                              │ Inspector     │
                                                              │ Position      │
                                                              │  X: 2.5 ←New! │
                                                              └───────────────┘
```

## Keyboard Shortcuts

```
┌─────────────────────────────────────────────────────────────────┐
│                     GIZMO MODE SHORTCUTS                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│   Q or W  →  Translate Mode  →  ┌──[Q] Move ──┐                │
│                                 │              │                │
│      E    →  Rotate Mode     →  │  E Rotate    │                │
│                                 │              │                │
│      R    →  Scale Mode      →  │  R Scale     │                │
│                                 └──────────────┘                │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│                    CAMERA SHORTCUTS                             │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│   Hold SHIFT + W/A/S/D  →  Move camera (Forward/Left/Back/Right)│
│   Hold SHIFT + Q/E      →  Move camera (Down/Up)               │
│   Right-click + Drag    →  Rotate camera                       │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Transform Components

```
┌──────────────────────────────────────────────────────────────────┐
│  Entity Transform = Position + Rotation + Scale                 │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│  Position (Translation):                                         │
│    X: Left(-) ←──→ Right(+)                                     │
│    Y: Down(-) ←──→ Up(+)                                        │
│    Z: Back(-) ←──→ Forward(+)                                   │
│                                                                  │
│  Rotation (Euler Angles):                                        │
│    X: Pitch (look up/down)          0° to 360°                  │
│    Y: Yaw (look left/right)         0° to 360°                  │
│    Z: Roll (tilt left/right)        0° to 360°                  │
│                                                                  │
│  Scale (Multiplier):                                             │
│    X: Width      (1.0 = original)   0.1 to 10.0                 │
│    Y: Height     (2.0 = double)     0.1 to 10.0                 │
│    Z: Depth      (0.5 = half)       0.1 to 10.0                 │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

## Color Coding

```
┌─────────────────────────────────────────────────────────────────┐
│                    AXIS COLOR REFERENCE                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│   🔴 RED    = X-axis  = Left/Right  = Width                    │
│                                                                 │
│   🟢 GREEN  = Y-axis  = Up/Down     = Height                   │
│                                                                 │
│   🔵 BLUE   = Z-axis  = Forward/Back = Depth                   │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│                     COORDINATE SYSTEM                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│                      +Y (Up)                                    │
│                       ↑                                         │
│                       |                                         │
│                       |                                         │
│       -X ←────────────+────────────→ +X (Right)                │
│     (Left)           /|                                         │
│                     / |                                         │
│                    /  |                                         │
│                   ↙   |                                         │
│                 +Z    |                                         │
│             (Forward) ↓                                         │
│                      -Y (Down)                                  │
│                                                                 │
│                   -Z (Back)                                     │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Usage Examples

### Example 1: Position a Cube at (2, 1, 0)

```
Step 1: Select Cube           Step 2: Press Q              Step 3: Drag Handles
  
Hierarchy                     Scene View                   Result
▶ Camera                          ↑                        Position
▶ Light                           |                        X: 2.0 ✓
▼ Cube  ← Click                ←─+─→                      Y: 1.0 ✓
                                Cube                       Z: 0.0 ✓
                                            
   Drag RED arrow right (to X=2)
   Drag GREEN arrow up (to Y=1)
   Leave BLUE at Z=0
```

### Example 2: Rotate Cube 45° around Y-axis

```
Step 1: Select Cube           Step 2: Press E              Step 3: Drag Green Circle
  
Hierarchy                     Scene View                   Result
▶ Camera                        ___                        Rotation
▶ Light                       ,'   `.                      X: 0.0
▼ Cube  ← Click              |  □  |   ← Green circle     Y: 45.0 ✓
                              `.___,'                      Z: 0.0
                              
   Drag green circle 45 degrees clockwise
```

### Example 3: Scale Cube to Double Size

```
Step 1: Select Cube           Step 2: Press R              Step 3: Drag Center
  
Hierarchy                     Scene View                   Result
▶ Camera                         ■                         Scale
▶ Light                          |                         X: 2.0 ✓
▼ Cube  ← Click              ■───+───■                    Y: 2.0 ✓
                                                           Z: 2.0 ✓
                                            
   Drag center handle outward to scale uniformly
```

## Workflow Tips

### Tip 1: Quick Mode Switching
```
Q → Move mode    E → Rotate mode    R → Scale mode

[Q] Move         E Rotate           R Scale
  ↕                 ↕                  ↕
Active           Inactive           Inactive
```

### Tip 2: Fine Control
```
For precise positioning:
1. Use gizmo for rough placement
2. Type exact values in Inspector
3. Fine-tune with gizmo again

Gizmo:        Inspector:      Final:
X ≈ 2.3  →    X = 2.5    →    Perfect!
```

### Tip 3: Multi-Axis Movement
```
Click between arrows to move on two axes at once:

     ↑
     |
     □ ← Click colored square
  ←──+──→
     |
     
Moves on two axes simultaneously
```

## Common Patterns

### Center an Object
```
Position: (0, 0, 0)
Scale:    (1, 1, 1)
Rotation: (0, 0, 0)

Result: Object at world center, normal size, no rotation
```

### Stack Objects Vertically
```
Cube 1:     Position (0, 0, 0)    Scale (1, 1, 1)
Cube 2:     Position (0, 2, 0)    Scale (1, 1, 1)  ← 2 units up
Cube 3:     Position (0, 4, 0)    Scale (1, 1, 1)  ← 4 units up
```

### Arrange Objects in a Row
```
Sphere 1:   Position (-2, 0, 0)   ← Left
Sphere 2:   Position (0, 0, 0)    ← Center
Sphere 3:   Position (2, 0, 0)    ← Right
```

## Troubleshooting Diagram

```
Problem: Gizmo not appearing?
           ↓
    Is object selected?
      /            \
    No              Yes
     ↓               ↓
   Select         Does it have
   object         a mesh?
   first           /      \
                  No      Yes
                  ↓        ↓
              Can't use   Gizmo should
              gizmo on    be visible!
              Camera/     
              Light       Check Scene
                          view focus
```

---

This visual reference should help users understand how to use the transform gizmo system effectively!
