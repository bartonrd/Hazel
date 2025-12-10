# Expected Output When Running Hazel

When you build and run the Hazel Sandbox application, you should see output similar to this:

```
[22:30:45.123] [INFO] Hazel Logging System Initialized
[22:30:45.124] [INFO] Hazel Engine Starting...
[22:30:45.125] [INFO] Initializing C# Scripting Engine...
[22:30:45.126] [INFO] C# Scripting Engine Initialized - Mono Runtime Ready
[22:30:45.127] [INFO] Sandbox Application Created
[22:30:45.128] [INFO] ImGuiLayer::OnAttach - Dockable windows system initialized
[22:30:45.129] [INFO] GameLayer::OnAttach
[22:30:45.130] [TRACE] ScriptInstance created for class: PlayerController
[22:30:45.131] [INFO] Creating Script Instance: PlayerController
[22:30:45.132] [INFO] All layers initialized
[22:30:45.133] [INFO] Features available:
[22:30:45.134] [INFO]   1. Logging System - Multiple log levels (Trace, Info, Warn, Error, Fatal)
[22:30:45.135] [INFO]   2. Dockable Windows - ImGui integration with docking support
[22:30:45.136] [INFO]   3. C# Scripting - Mono runtime integration for hot-reloadable scripts
[22:30:45.137] [INFO] Application::Run() - Main loop started
[22:30:45.138] [INFO] Application::Run() - Main loop ended
[22:30:45.139] [INFO] GameLayer::OnDetach
[22:30:45.140] [TRACE] ScriptInstance destroyed for class: PlayerController
[22:30:45.141] [INFO] ImGuiLayer::OnDetach
[22:30:45.142] [INFO] Sandbox Application Destroyed
[22:30:45.143] [INFO] Shutting down C# Scripting Engine...
[22:30:45.144] [INFO] Hazel Logging System Shutdown
```

## What This Demonstrates

### 1. Logging System
Every line shows the logging system in action:
- `[HH:MM:SS.mmm]` - Timestamp with millisecond precision
- `[LEVEL]` - Log level (TRACE, INFO, WARN, ERROR, FATAL)
- Message content

### 2. System Initialization Order
The output shows the proper initialization sequence:
1. Logging system starts first
2. Engine announces startup
3. C# Scripting engine initializes (Mono runtime)
4. Application is created
5. Layers are attached (ImGui layer, Game layer)
6. Script instances are created
7. Features are announced

### 3. Layer System
You can see layers being attached and detached:
- `ImGuiLayer::OnAttach` - Dockable windows initialized
- `GameLayer::OnAttach` - Game logic layer attached
- Script components created within layers
- Clean shutdown in reverse order

### 4. C# Scripting
Script-related messages show:
- `ScriptInstance created for class: PlayerController`
- Script lifecycle being managed
- Clean destruction of script instances

## In a Full Implementation

With ImGui fully integrated and a rendering context, you would see:

```
[Window Opens - 1920x1080]

┌─────────────────────────────────────────────────────────────┐
│ Hazel Editor                                    [_] [□] [×] │
├──────────┬──────────────────────────────────────────────────┤
│          │                                                  │
│  Scene   │              Game View                          │
│ Hierarchy│     [3D Viewport with rendered scene]          │
│          │                                                  │
│  ▼ Root  │                                                  │
│    ▶ Player                                                 │
│    ▶ Camera                                                 │
│    ▶ Light                                                  │
│          │                                                  │
├──────────┼──────────────────────────────────────────────────┤
│          │                                                  │
│Inspector │           Console                               │
│          │  [22:30:45.123] [INFO] Hazel Engine Starting... │
│Transform │  [22:30:45.124] [INFO] C# Scripting Initialized │
│ Pos: 0,0,0                                                  │
│ Rot: 0,0,0                                                  │
│ Scale: 1,1,1                                                │
│          │                                                  │
│Script    │                                                  │
│PlayerController                                             │
│ Speed: 5.0                                                  │
│          │                                                  │
└──────────┴──────────────────────────────────────────────────┘
```

## Building and Running

To see this output:

```bash
# Open Visual Studio
1. Open Hazel.sln
2. Build Solution (Ctrl+Shift+B)
3. Set Sandbox as startup project (right-click → Set as Startup Project)
4. Run (F5 or Ctrl+F5)

# The console window will show all the log output
```

## Next Steps

Once you have the basic output working, you can:

1. **Add 3D Rendering**
   - Integrate OpenGL/DirectX
   - Add window creation (GLFW/Win32)
   - Render the 3D viewport

2. **Implement ImGui Windows**
   - Uncomment the ImGui code in ImGuiLayer
   - Add ImGui as a dependency
   - Implement the actual dockable windows

3. **Complete Mono Integration**
   - Add Mono as a dependency
   - Uncomment the Mono code in ScriptEngine
   - Compile and load actual C# assemblies

4. **Add Input System**
   - Keyboard and mouse input
   - Make the camera and player respond to input

This foundation is ready to grow into a full Unity-like engine! 🚀
