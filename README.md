# Hazel Game Engine

Hazel is a 3D game engine similar to Unity, built with C++ and designed for high-performance game development.

## Projects

### HazelEditor (Unity-like Editor) 🎨
A fully-featured Unity-like editor with dockable windows and a familiar interface layout:
- **Scene Hierarchy**: Tree view of all game objects in the scene
- **Inspector**: Property editor for selected objects with component views
- **Console**: Real-time log viewer with filtering (Trace/Info/Warn/Error/Fatal)
- **Scene View**: 3D scene editor with tool gizmos (Hand/Move/Rotate/Scale)
- **Game View**: Runtime game preview viewport
- **Asset Browser**: Project asset management with folder structure
- **Toolbar**: Play/Pause/Stop controls for game testing

### Sandbox (Basic Example)
A minimal example application demonstrating the engine's core features.

## Features

### 1. Logging System
The engine includes a comprehensive logging system with multiple log levels:
- **Trace**: Detailed diagnostic information
- **Info**: General informational messages
- **Warn**: Warning messages
- **Error**: Error messages
- **Fatal**: Critical errors

Usage:
```cpp
HZ_TRACE("Detailed debug information");
HZ_INFO("Application started");
HZ_WARN("Warning: Resource not found");
HZ_ERROR("Error loading asset");
HZ_FATAL("Critical system failure");
```

### 2. Modular Dockable Windows
Built on ImGui, Hazel provides a flexible docking system for editor windows:
- **Scene Hierarchy**: View and manage scene objects
- **Inspector/Properties**: Edit component properties
- **Console**: View log messages in real-time
- **Asset Browser**: Manage project assets
- **Game View**: Runtime game preview
- **Scene View**: 3D scene editor

The ImGui integration supports:
- Full docking and undocking of windows
- Multi-viewport support
- Customizable layouts
- Persistent window configurations

### 3. C# Scripting Support
Hazel integrates the Mono runtime for C# scripting:

**Features:**
- Hot-reloadable scripts
- Access to engine API from C#
- Component-based scripting system
- Field inspection and modification

**Example C# Script:**
```csharp
using Hazel;

public class PlayerController : ScriptComponent
{
    public float speed = 5.0f;
    public float jumpForce = 10.0f;

    void OnCreate()
    {
        Log.Info("Player created!");
    }

    void OnUpdate(float deltaTime)
    {
        // Game logic here
        if (Input.IsKeyPressed(KeyCode.Space))
        {
            Jump();
        }
    }

    void OnDestroy()
    {
        Log.Info("Player destroyed!");
    }

    void Jump()
    {
        // Apply jump force
    }
}
```

**Using Scripts in C++:**
```cpp
// Create a script component
Hazel::ScriptComponent playerScript("PlayerController");

// Initialize
playerScript.OnCreate();

// Update in game loop
playerScript.OnUpdate(deltaTime);

// Cleanup
playerScript.OnDestroy();
```

## Architecture

### Layer System
Hazel uses a layer-based architecture for organizing game logic:
- Layers are updated in order
- ImGui rendering happens separately
- Easy to add/remove layers at runtime

### Event System
Event-driven architecture for handling input and system events:
- Window events (close, resize, focus)
- Input events (keyboard, mouse)
- Application events (tick, update, render)

## Building

### Prerequisites
- Visual Studio 2022 (v143 toolset)
- Windows 10 SDK
- C++17 or later

### Build Instructions
1. Open `Hazel.sln` in Visual Studio
2. Build the solution (Ctrl+Shift+B)
3. Run the **Sandbox** application (basic example) OR
4. Run the **HazelEditor** application (Unity-like editor interface)

### Project Structure
```
Hazel/
├── Hazel/              # Engine core
│   └── src/
│       ├── Hazel/
│       │   ├── Application.h/cpp
│       │   ├── Log.h/cpp
│       │   ├── Layer.h/cpp
│       │   ├── LayerStack.h/cpp
│       │   ├── Events/
│       │   │   └── Event.h
│       │   ├── ImGui/
│       │   │   └── ImGuiLayer.h/cpp
│       │   └── Scripting/
│       │       ├── ScriptEngine.h/cpp
│       │       └── ScriptComponent.h/cpp
│       └── Hazel.h
├── Sandbox/            # Example application
│   └── src/
│       └── SandboxApp.cpp
└── HazelEditor/        # Unity-like editor (NEW!)
    └── src/
        ├── EditorApp.cpp
        ├── EditorLayer.h/cpp
        └── Panels/
            ├── Scene Hierarchy
            ├── Inspector
            ├── Console
            ├── Game/Scene Views
            └── Asset Browser
```

## Future Development

### Phase 1: Core Infrastructure ✓
- [x] Logging system
- [x] Event system
- [x] Layer system

### Phase 2: Window and UI System ✓
- [x] ImGui integration
- [x] Dockable window framework
- [x] Full Unity-like editor interface
- [x] Scene Hierarchy panel
- [x] Inspector/Properties panel
- [x] Console panel with log filtering
- [x] Game View and Scene View panels
- [x] Asset Browser panel
- [x] Toolbar with Play/Pause/Stop controls

### Phase 3: 3D Rendering Foundation
- [ ] OpenGL/DirectX rendering context
- [ ] Camera system (perspective and orthographic)
- [ ] Basic 3D mesh rendering
- [ ] Shader system
- [ ] Material system

### Phase 4: Scene and Entity System
- [ ] Entity-Component-System (ECS) architecture
- [ ] Scene graph for 3D objects
- [ ] Transform components (position, rotation, scale)
- [ ] Scene serialization

### Phase 5: C# Scripting Support ✓
- [x] Basic scripting framework
- [x] Script component system
- [ ] Full Mono runtime integration
- [ ] Script hot-reloading implementation
- [ ] Complete C++ to C# interop layer

### Phase 6: Editor Features
- [ ] Gizmos for transform manipulation
- [ ] Object picking/selection
- [ ] Asset browser
- [ ] Play/pause/stop controls

## Contributing
This is a demonstration project showing the foundational architecture of a 3D game engine. Contributions are welcome!

## License
MIT License - see LICENSE file for details
