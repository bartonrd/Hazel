# HazelEditor - Unity-like Editor Interface

## Console Simulation Mode

The HazelEditor currently runs in **console simulation mode**, which demonstrates the complete editor architecture and panel structure without requiring external dependencies like ImGui library or a rendering backend.

### What You'll See

When you run HazelEditor, you'll see:
1. A visual ASCII art representation of the Unity-like layout
2. All 7 editor panels being initialized and rendered
3. The application running for several frames to show it's active
4. Detailed panel information in the console output

### Sample Output

```
╔══════════════════════════════════════════════════════════════╗
║          HAZEL EDITOR - Console Simulation Mode              ║
╚══════════════════════════════════════════════════════════════╝

Hazel Editor Application Created
Initializing Unity-like Editor Interface...

NOTE: This is a console-based simulation of the editor.
For full GUI rendering with windows, integrate:
  1. ImGui library (github.com/ocornut/imgui)
  2. Rendering backend (OpenGL/DirectX)
  3. Window system (GLFW/Win32)

Editor initialized with 7 panels:
  - Scene Hierarchy (left): Tree view of scene objects
  - Inspector (right): Properties of selected object
  - Scene View (center): 3D scene editor with gizmos
  - Game View (center tab): Runtime game preview
  - Console (bottom): Filtered log output
  - Project Browser (bottom tab): Asset management
  - Toolbar (top): Play/Pause/Stop controls

┌────────────────────────────────────────────────────────────┐
│ File  Edit  Assets  GameObject  Component  Window   [▷][||]│ ← Menu & Toolbar
├───────────┬────────────────────────┬───────────────────────┤
│ HIERARCHY │    SCENE / GAME        │     INSPECTOR         │
│           │                        │                       │
│ ▼ Camera  │   [3D Viewport]        │  Selected: None       │
│ ▼ Light   │                        │                       │
│ ▼ Player  │   Scene Tools:         │  Transform            │
│ ▼ Ground  │   Q W E R              │  Position: 0,0,0      │
│ ▼ Environ │                        │  Rotation: 0,0,0      │
│           │                        │  Scale: 1,1,1         │
├───────────┴────────────────────────┴───────────────────────┤
│ CONSOLE                                                    │
│ [Clear] [✓]Trace [✓]Info [✓]Warn [✓]Error                │
│ Displaying filtered log messages...                        │
└────────────────────────────────────────────────────────────┘
```

## Upgrading to Full GUI

To add actual graphical windows, you would need to:

### 1. Add ImGui Library
Download ImGui from https://github.com/ocornut/imgui and add it to the project:
- Copy ImGui source files to `vendor/imgui/`
- Add ImGui files to HazelEditor.vcxproj
- Include ImGui headers in ImGuiLayer.cpp

### 2. Add Window/Rendering Backend
Choose a backend combination:
- **Option A**: GLFW + OpenGL
- **Option B**: Win32 + DirectX 11
- **Option C**: SDL2 + Vulkan

### 3. Uncomment ImGui Calls
The code contains all the ImGui calls commented out with examples of what would be rendered:
- See `EditorLayer.cpp` - All panel methods have commented ImGui code
- See `ImGuiLayer.cpp` - ImGui initialization and rendering code
- Simply uncomment these sections after adding the library

### 4. Initialize Window and Graphics
Add window creation and graphics context initialization:
```cpp
// In ImGuiLayer::OnAttach()
m_Window = CreateWindow(...);
m_GraphicsContext = CreateGraphicsContext(...);
ImGui::CreateContext();
ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
ImGui_ImplOpenGL3_Init("#version 410");
```

## Architecture

The editor is fully structured with:
- ✅ 7 Unity-like panels (Hierarchy, Inspector, Console, Scene View, Game View, Asset Browser, Toolbar)
- ✅ Entity selection system
- ✅ Play/Pause state management
- ✅ Console log filtering
- ✅ Menu system
- ✅ Docking framework ready

All the logic is in place - it just needs the visual rendering layer!

## Current Features

- **Console-based visualization** showing the editor structure
- **Multi-frame execution** demonstrating the main loop
- **All 7 panels** implemented with proper state management
- **Entity hierarchy** with sample objects (Camera, Light, Player, Ground, Environment)
- **Complete menu system** (File, Edit, Assets, GameObject, Component, Window, Help)
- **Toolbar** with Play/Pause/Step controls

## Documentation

See the main repository documentation:
- `EDITOR.md` - Complete user guide
- `EDITOR_MOCKUP.md` - Visual interface mockup
- `EDITOR_IMPLEMENTATION.md` - Technical details
- `IMPLEMENTATION_CHECKLIST.md` - Development checklist
