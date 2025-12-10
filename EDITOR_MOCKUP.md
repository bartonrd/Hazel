# HazelEditor - Unity-like Interface Mockup

This document shows what the HazelEditor interface looks like when fully rendered with ImGui.

## Complete Editor Layout

```
╔═══════════════════════════════════════════════════════════════════════════════════════╗
║ File  Edit  Assets  GameObject  Component  Window  Help                    [_][□][X] ║
╠═══════════════════════════════════════════════════════════════════════════════════════╣
║                                [▷ Play]  [|| Pause]  [▶▶ Step]                       ║
╠═════════════════╦═════════════════════════════════════════════╦═══════════════════════╣
║                 ║                                             ║                       ║
║   Hierarchy     ║          Scene          Game                ║     Inspector         ║
║                 ║    ┌─────────────────────────┐              ║                       ║
║ Create Empty    ║    │Q W E R                  │ [x] Tab      ║ GameObject            ║
║ ─────────────── ║    │                         │              ║ ───────────────────   ║
║                 ║    │                         │              ║ Name: Player          ║
║ ▼ Camera        ║    │                         │              ║ Tag: Player           ║
║ ▼ Directional   ║    │      3D VIEWPORT        │              ║ Layer: Default        ║
║    Light        ║    │                         │              ║                       ║
║ ▼ Player  [SEL] ║    │    [Scene Gizmos]       │              ║ ▼ Transform           ║
║   ▶ Model       ║    │                         │              ║   Position            ║
║   ▶ Collider    ║    │                         │              ║   X: 0.00  Y: 5.00   ║
║ ▶ Ground        ║    │                         │              ║   Z: 0.00             ║
║ ▼ Environment   ║    │                         │              ║   Rotation            ║
║   ▶ Skybox      ║    │                         │              ║   X: 0.00  Y: 0.00   ║
║   ▶ Terrain     ║    └─────────────────────────┘              ║   Z: 0.00             ║
║   ▶ Trees       ║                                             ║   Scale               ║
║                 ║                                             ║   X: 1.00  Y: 1.00   ║
║                 ║                                             ║   Z: 1.00             ║
║                 ║                                             ║                       ║
║                 ║                                             ║ ▼ Mesh Renderer       ║
║                 ║                                             ║   Material: Default   ║
║                 ║                                             ║                       ║
║                 ║                                             ║ ▼ Script              ║
║                 ║                                             ║   PlayerController.cs ║
║                 ║                                             ║   Speed: 5.0          ║
║                 ║                                             ║   Jump Force: 10.0    ║
║                 ║                                             ║                       ║
║                 ║                                             ║ Add Component         ║
║                 ║                                             ║                       ║
╠═════════════════╩═════════════════════════════════════════════╩═══════════════════════╣
║                                                                                       ║
║  Console                                    Project                     [x] Tab      ║
║  ─────────────────────────────────────────────────────────────────────────────────   ║
║  [Clear]  [✓]Trace  [✓]Info  [✓]Warn  [✓]Error  [✓]Fatal                            ║
║  ─────────────────────────────────────────────────────────────────────────────────   ║
║  [22:46:54.001] [INFO] Hazel Engine Starting...                                     ║
║  [22:46:54.002] [INFO] EditorLayer::OnAttach - Unity-like editor initialized        ║
║  [22:46:54.003] [TRACE] Menu Bar: File | Edit | Assets | GameObject | Component     ║
║  [22:46:54.004] [TRACE] Hierarchy Panel: Camera, Light, Player, Ground, Environment ║
║  [22:46:54.005] [INFO] Selected: Player                                             ║
║  [22:46:54.006] [TRACE] Inspector Panel: Transform, Components for selected object  ║
║  [22:46:54.007] [TRACE] Console Panel: Displaying filtered log messages             ║
║  [22:46:54.008] [TRACE] Game View: Rendering game viewport                          ║
║  [22:46:54.009] [TRACE] Scene View: Rendering scene with editor tools               ║
║  [22:46:54.010] [TRACE] Project Browser: Assets folder structure                    ║
║                                                                                       ║
╚═══════════════════════════════════════════════════════════════════════════════════════╝
```

## Panel Descriptions

### 1. Menu Bar (Top)
- **File**: New Scene, Open Scene, Save, Save As, Build Settings, Exit
- **Edit**: Undo, Redo, Cut, Copy, Paste, Duplicate, Delete
- **Assets**: Create folder, Import, Refresh
- **GameObject**: Create Empty, 3D Objects, Camera, Light
- **Component**: Add component to selected object
- **Window**: Panel visibility toggles, Layouts
- **Help**: About, Documentation

### 2. Toolbar (Below Menu)
Central controls for play mode testing:
- **▷ Play**: Start game simulation
- **|| Pause**: Pause the running game
- **▶▶ Step**: Advance one frame while paused

### 3. Hierarchy Panel (Left)
Tree view of all GameObjects in the scene:
```
▼ Camera                    (Expanded, showing no children)
▼ Directional Light        (Expanded, showing no children)
▼ Player                   (Expanded, selected, showing children)
  ▶ Model                  (Collapsed child)
  ▶ Collider               (Collapsed child)
▶ Ground                   (Collapsed)
▼ Environment              (Expanded, showing children)
  ▶ Skybox                 (Collapsed child)
  ▶ Terrain                (Collapsed child)
  ▶ Trees                  (Collapsed child)
```
Features:
- Click to select
- Click arrow to expand/collapse
- Right-click for context menu
- Drag to reparent

### 4. Scene/Game Views (Center)
**Scene View**: Editor view with manipulation tools
- Tool palette: Q, W, E, R (Hand, Move, Rotate, Scale)
- Grid, gizmos, and editor visuals
- Free camera navigation

**Game View**: Runtime preview (tabbed with Scene)
- Shows game as players see it
- Active only in play mode
- Aspect ratio controls

### 5. Inspector Panel (Right)
Shows properties of selected GameObject:

```
GameObject
───────────────────
Name: Player
Tag: Player
Layer: Default

▼ Transform
  Position:  X: 0.00  Y: 5.00  Z: 0.00
  Rotation:  X: 0.00  Y: 0.00  Z: 0.00
  Scale:     X: 1.00  Y: 1.00  Z: 1.00

▼ Mesh Renderer
  Material: Default
  Cast Shadows: On
  Receive Shadows: On

▼ Script Component
  PlayerController.cs
  Speed: 5.0
  Jump Force: 10.0

[Add Component]
```

### 6. Console Panel (Bottom)
Log message viewer:
- Filter buttons for each log level
- Clear button
- Color-coded messages:
  - TRACE (Gray): Detailed debug
  - INFO (White): General info
  - WARN (Yellow): Warnings
  - ERROR (Red): Errors
  - FATAL (Dark Red): Critical
- Auto-scroll option
- Timestamps on all messages

### 7. Project Panel (Bottom, tabbed with Console)
Asset browser:
```
Folders (Left)          Assets (Right)
───────────────         ──────────────────────
▼ Assets                PlayerController.cs
  ▶ Scripts             EnemyAI.cs
  ▶ Materials           DefaultMaterial.mat
  ▶ Scenes              MainScene.scene
  ▶ Textures            PlayerTexture.png
  ▶ Models              PlayerModel.fbx
  ▶ Audio               ...
```

## Interaction Examples

### Selecting an Object
1. Click "Player" in Hierarchy
2. Hierarchy highlights the selection
3. Inspector shows Player's components
4. Scene view shows Player with selection outline

### Editing Transform
1. Select object in Hierarchy
2. In Inspector, drag X/Y/Z sliders
3. Or use W/E/R tools in Scene view
4. Object updates in real-time

### Viewing Logs
1. Look at Console panel
2. Click filter checkboxes to show/hide levels
3. All engine activity is logged here
4. Click "Clear" to reset

### Play Mode
1. Click Play button (▷)
2. Game View becomes active
3. Console shows runtime logs
4. Click Pause (||) or Stop to exit

## Color Scheme (Unity-like Dark Theme)

```
Background:        Dark Gray (#383838)
Panel Headers:     Darker Gray (#2D2D2D)
Selected Item:     Blue Highlight (#2F5F9F)
Text:              Light Gray (#CCCCCC)
Borders:           Very Dark Gray (#1E1E1E)

Log Colors:
  TRACE:  Gray (#888888)
  INFO:   White (#FFFFFF)
  WARN:   Yellow (#FFFF00)
  ERROR:  Red (#FF0000)
  FATAL:  Dark Red (#AA0000)
```

## Keyboard Shortcuts

```
Q - Hand Tool (Pan camera)
W - Move Tool
E - Rotate Tool
R - Scale Tool
F - Frame Selected
Delete - Delete Selected

Ctrl+N - New Scene
Ctrl+O - Open Scene
Ctrl+S - Save
Ctrl+Shift+S - Save As

Ctrl+Z - Undo
Ctrl+Y - Redo
Ctrl+D - Duplicate
Ctrl+C - Copy
Ctrl+V - Paste

Space - Play/Stop
Pause - Pause
```

## Implementation Notes

The UI structure is implemented in **EditorLayer.cpp** with methods for each panel:
- `DrawMenuBar()` - Top menu
- `DrawToolbar()` - Play controls
- `DrawSceneHierarchy()` - Left panel
- `DrawInspector()` - Right panel
- `DrawConsole()` - Bottom-left tab
- `DrawGameView()` - Center-top tab
- `DrawSceneView()` - Center tab
- `DrawAssetBrowser()` - Bottom-right tab

All panels use ImGui for rendering (when fully integrated with ImGui library).

The layout mimics Unity's default layout with dockable windows powered by ImGui's docking system.
