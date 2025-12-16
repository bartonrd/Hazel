# Hazel Editor - Unity-like Interface

The Hazel Editor is a Unity-inspired game editor interface built on top of the Hazel engine. It provides a familiar, professional environment for game development with dockable windows and intuitive controls.

## Overview

The editor features a complete Unity-like layout with all essential panels:

```
┌──────────────────────────────────────────────────────────────┐
│ File  Edit  Assets  GameObject  Component  Window  Help  ▷▌▌ │ Menu Bar & Toolbar
├──────────────┬───────────────────────────┬───────────────────┤
│              │                           │                   │
│  Hierarchy   │     Scene / Game View     │    Inspector      │
│              │                           │                   │
│  ▼ Camera    │   ┌───────────────────┐   │  ┌──────────────┐ │
│  ▶ Light     │   │                   │   │  │  Transform   │ │
│  ▼ Player    │   │    3D Viewport    │   │  │  Position    │ │
│    ▶ Model   │   │                   │   │  │  Rotation    │ │
│  ▶ Ground    │   │                   │   │  │  Scale       │ │
│  ▶ Env       │   │                   │   │  ├──────────────┤ │
│              │   │                   │   │  │  Components  │ │
│              │   └───────────────────┘   │  │  + Add       │ │
│              │                           │  └──────────────┘ │
├──────────────┴───────────────────────────┴───────────────────┤
│  Console / Project                                           │
│  ┌──────────────────────────────────────────────────────────┐│
│  │ Clear [✓]Trace [✓]Info [✓]Warn [✓]Error [✓]Fatal         ││
│  │ [22:30:45.123] [INFO] Hazel Engine Starting...           ││
│  │ [22:30:45.124] [INFO] Editor initialized                 ││
│  └──────────────────────────────────────────────────────────┘│
└──────────────────────────────────────────────────────────────┘
```

## Docking System

The Hazel Editor features a Unity-like docking system that allows you to customize your workspace:

### How to Dock Windows

1. **Drag to Dock**: Click and drag any window by its title bar
2. **Drop Indicators**: Visual guides show where the window will dock
3. **Tab Grouping**: Drop on center to create tabs with other windows
4. **Split Docking**: Drop on edges to split the area
5. **Detach**: Drag windows outside to create separate OS windows

### Docking Operations

- **Dock within main window**: Drag panels to edges or center of other panels
- **Dock to other panels**: Create tabs or split existing panels
- **Detach to separate window**: Pull panels outside the main editor window
- **Dock in detached windows**: Detached windows also support docking
- **Undock**: Drag docked panels away to make them floating

### Keyboard Modifiers

- **Hold SHIFT while dragging**: Disable docking temporarily (keep window floating)
- **Drag from window menu** (⊞ button): Undock entire docking node

### Multi-Monitor Support

The docking system fully supports multi-monitor setups:
- Detach windows to secondary monitors
- Each detached window is a separate OS window
- Docking works the same in detached windows
- Perfect for spreading your workspace across multiple screens

For detailed information about the docking implementation, see [DOCKING_IMPLEMENTATION.md](DOCKING_IMPLEMENTATION.md).

## Panels

**Note**: All panel locations are defaults. You can dock panels anywhere you prefer!

### 1. Scene Hierarchy
**Default Location**: Left side  
**Function**: Tree view of all GameObjects in the current scene

Features:
- Hierarchical tree structure showing parent-child relationships
- Select objects by clicking
- Create new empty GameObjects
- Visual indication of selected objects
- Expand/collapse hierarchy nodes

Example usage:
```cpp
// The hierarchy automatically displays all entities in the scene
// Click on any entity to select it and view properties in Inspector
```

### 2. Inspector
**Default Location**: Right side  
**Function**: Display and edit properties of the selected GameObject

Features:
- Shows all components attached to selected object
- Transform component (Position, Rotation, Scale)
- Component property editing with drag controls
- "Add Component" button to attach new components
- Collapsible component headers

Example properties shown:
```
Transform
├── Position: (0.0, 0.0, 0.0)
├── Rotation: (0.0, 0.0, 0.0)
└── Scale: (1.0, 1.0, 1.0)

Mesh Renderer
└── Material: Default

Script Component
└── PlayerController.cs
```

### 3. Scene View
**Default Location**: Center (main viewport)  
**Function**: 3D editor view with manipulation tools

Features:
- 3D viewport showing the scene from editor camera
- Tool palette (Q: Hand, W: Move, E: Rotate, R: Scale)
- Gizmos for visual manipulation
- Grid and axis helpers
- Free camera navigation

Keyboard shortcuts:
- **Q**: Hand tool (camera navigation)
- **W**: Move tool
- **E**: Rotate tool  
- **R**: Scale tool
- **F**: Frame selected object

### 4. Game View
**Default Location**: Center (tabbed with Scene View)
**Function**: Runtime game preview

Features:
- Shows game as it would appear to players
- Active only when game is playing
- Aspect ratio controls
- Resolution preview options

### 5. Console
**Default Location**: Bottom  
**Function**: Display engine and game log messages

Features:
- Real-time log message display
- Color-coded message levels:
  - **Trace**: Gray (detailed debug info)
  - **Info**: White (general information)
  - **Warn**: Yellow (warnings)
  - **Error**: Red (errors)
  - **Fatal**: Dark red (critical errors)
- Filter toggles for each log level
- Clear button to remove all messages
- Auto-scroll to latest messages
- Timestamps for each message

Usage:
```cpp
HZ_INFO("Player spawned at position (0, 5, 0)");
HZ_WARN("Asset not found, using default");
HZ_ERROR("Failed to load texture");
```

### 6. Project Browser
**Default Location**: Bottom (tabbed with Console)  
**Function**: Asset and file management

Features:
- Folder tree navigation (left side)
- Asset grid/list view (right side)
- Common asset folders:
  - Scripts (C# files)
  - Materials
  - Scenes
  - Textures
  - Models
- Asset selection and preview
- Right-click context menu for operations

### 7. Toolbar
**Default Location**: Top (below menu bar)  
**Function**: Play mode controls

Features:
- **Play button (▶)**: Start game simulation
- **Pause button (||)**: Pause game simulation  
- **Step button (▶▶)**: Advance one frame while paused
- Visual indication of play/edit mode

## Editor Workflow

### Basic Scene Editing
1. Select object in Hierarchy
2. View/edit properties in Inspector
3. Use Scene View tools to manipulate transform
4. Test by clicking Play in Toolbar
5. View runtime behavior in Game View
6. Monitor output in Console

### Creating GameObjects
```cpp
// In Hierarchy panel, click "Create Empty"
// This creates a new GameObject with Transform component
// Use Inspector to add components
```

### Working with Components
```cpp
// Select a GameObject
// In Inspector, click "Add Component"
// Choose component type to add
// Edit component properties using drag controls
```

### Using the Console
```cpp
// Click filter checkboxes to show/hide log levels
// Click "Clear" to remove all messages
// Messages auto-scroll as new logs arrive
// Each message shows timestamp and severity level
```

## Editor Architecture

### EditorLayer Class
The main editor logic is contained in `EditorLayer`:

```cpp
class EditorLayer : public Hazel::Layer
{
    // Panel rendering methods
    void DrawMenuBar();
    void DrawToolbar();
    void DrawSceneHierarchy();
    void DrawInspector();
    void DrawConsole();
    void DrawGameView();
    void DrawSceneView();
    void DrawAssetBrowser();
    
    // Editor state
    bool m_IsPlaying;
    bool m_IsPaused;
    Entity* m_SelectedEntity;
};
```

### Integration with Hazel Engine

The editor is built on top of Hazel's Layer system:

```cpp
// EditorApp.cpp
class HazelEditorApp : public Hazel::Application
{
public:
    HazelEditorApp()
    {
        // Add ImGui layer for docking support
        PushOverlay(new Hazel::ImGuiLayer());
        
        // Add editor layer with all panels
        PushLayer(new HazelEditor::EditorLayer());
    }
};
```

## Customization

### Adding New Panels
To add a custom panel:

1. Add a new method in `EditorLayer.h`:
```cpp
void DrawMyCustomPanel();
```

2. Implement the method in `EditorLayer.cpp`:
```cpp
void EditorLayer::DrawMyCustomPanel()
{
    // ImGui::Begin("My Custom Panel");
    // ... panel contents ...
    // ImGui::End();
}
```

3. Call it from `OnImGuiRender()`:
```cpp
void EditorLayer::OnImGuiRender()
{
    // ... existing panels ...
    DrawMyCustomPanel();
}
```

### Modifying Layout
The docking layout can be customized in `OnImGuiRender()`:
```cpp
// Set up custom dockspace configuration
// Adjust window sizes and positions
// Create custom tab groups
```

## Future Enhancements

Planned features for the editor:

1. **Visual Gizmo Rendering**
   - 3D manipulation handles
   - Rotation circles
   - Scale boxes

2. **Asset Import**
   - Drag-and-drop asset importing
   - Asset preview thumbnails
   - Material editor

3. **Scene Serialization**
   - Save/Load scene files
   - Prefab system
   - Scene management

4. **Play Mode**
   - Full game simulation
   - Hot-reload C# scripts
   - Debug tools during play

5. **Enhanced Console**
   - Stack trace on errors
   - Click to jump to code
   - Search and filtering

## Technical Details

### Dependencies
- **Hazel Engine**: Core engine functionality
- **ImGui**: UI rendering (when fully integrated)
- **Visual Studio 2022**: Build toolchain

### Building HazelEditor
```bash
1. Open Hazel.sln in Visual Studio 2022
2. Select "HazelEditor" as startup project
3. Build (Ctrl+Shift+B)
4. Run (F5 or Ctrl+F5)
```

### File Structure
```
HazelEditor/
├── src/
│   ├── EditorApp.cpp          # Application entry point
│   ├── EditorLayer.h          # Editor layer header
│   └── EditorLayer.cpp        # All panel implementations
└── HazelEditor.vcxproj        # Visual Studio project
```

## Comparison with Unity

| Feature | Unity | Hazel Editor | Status |
|---------|-------|--------------|--------|
| Scene Hierarchy | ✓ | ✓ | Implemented |
| Inspector | ✓ | ✓ | Implemented |
| Console | ✓ | ✓ | Implemented |
| Scene View | ✓ | ✓ | Implemented |
| Game View | ✓ | ✓ | Implemented |
| Project Browser | ✓ | ✓ | Implemented |
| Toolbar | ✓ | ✓ | Implemented |
| Gizmos | ✓ | ○ | Planned |
| Asset Import | ✓ | ○ | Planned |
| Play Mode | ✓ | ○ | Planned |

✓ = Implemented, ○ = Planned

## Getting Started

1. **Build the editor**:
   - Open Hazel.sln
   - Set HazelEditor as startup project
   - Build and run

2. **Explore the interface**:
   - Check out each panel
   - Review the log output in Console
   - Examine the sample scene hierarchy

3. **Customize**:
   - Modify EditorLayer to add features
   - Extend with new panels
   - Integrate rendering for visual feedback

The Hazel Editor provides a solid foundation for a Unity-like development environment. With the ImGui framework and modular architecture, it's ready to grow into a full-featured game editor! 🎮
