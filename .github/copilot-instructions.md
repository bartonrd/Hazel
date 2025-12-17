# Copilot Instructions for Hazel Game Engine

## Project Overview

Hazel is a 3D game engine similar to Unity, built with C++ and designed for high-performance game development. It features a fully functional graphical user interface built with ImGui and GLFW, providing a Unity-like editor experience.

## Key Technologies

- **Language**: C++17
- **Build System**: Visual Studio 2022 / MSBuild (v143 toolset)
- **GUI Framework**: Dear ImGui (docking branch)
- **Windowing**: GLFW 3
- **Graphics API**: OpenGL 3.3 Core Profile
- **Math Library**: GLM (OpenGL Mathematics)
- **Scripting**: C# with Mono runtime
- **Platform**: Windows 10+ with Windows 10 SDK

## Repository Structure

```
Hazel/
├── Hazel/              # Engine core (DLL)
│   └── src/Hazel/      # Core engine systems
├── HazelEditor/        # Unity-like editor application (EXE)
│   └── src/            # Editor UI and panels
├── Sandbox/            # Example/demo application (EXE)
├── vendor/             # Third-party libraries (ImGui, GLFW, GLM)
├── Scripts/            # C# game scripts
└── Hazel.sln          # Visual Studio solution
```

## Architecture Patterns

### Layer System
- Use `Layer` base class for organizing game logic and editor panels
- Layers are updated in order via `OnUpdate(float deltaTime)`
- ImGui rendering happens in `OnImGuiRender()`
- Event handling through `OnEvent(Event& event)`

### Event System
- Event-driven architecture for input and system events
- Use `EventDispatcher` to route events to appropriate handlers
- Events are processed by layers in reverse order (overlays first)

### Logging System
Use the following macros for consistent logging:
- `HZ_TRACE()` - Detailed debug information
- `HZ_INFO()` - General informational messages
- `HZ_WARN()` - Warning messages
- `HZ_ERROR()` - Error messages
- `HZ_FATAL()` - Critical errors

### Rendering Architecture
- Modern OpenGL 3.3 Core Profile
- Vertex Array Objects (VAO) for geometry
- Material system for shader parameters
- Scene graph with camera and lighting support
- Phong lighting model with multiple light types

## Build Instructions

### Prerequisites
- Visual Studio 2022 with v143 toolset
- Windows 10 SDK
- C++17 or later support

### Building the Project
1. Open `Hazel.sln` in Visual Studio 2022
2. Select configuration: Debug|x64 or Release|x64
3. Set startup project:
   - **HazelEditor** for the Unity-like editor
   - **Sandbox** for the 3D rendering demo
4. Build: Ctrl+Shift+B
5. Run: F5 (with debugging) or Ctrl+F5 (without)

### Common Build Issues
- See `BUILD_TROUBLESHOOTING.md` for detailed solutions to common errors
- ImGui context must be shared across DLL boundaries
- GLFW is compiled only in Hazel.dll, not in HazelEditor.exe
- Use `_CRT_SECURE_NO_WARNINGS` to suppress vendor code warnings

## Coding Conventions

### C++ Style
- Use C++17 features
- Follow existing naming conventions in the codebase:
  - Classes: PascalCase (e.g., `Application`, `ImGuiLayer`)
  - Member variables: m_PascalCase prefix (e.g., `m_Running`, `m_LayerStack`)
  - Functions: PascalCase (e.g., `OnUpdate`, `PushLayer`)
  - Namespaces: PascalCase (e.g., `Hazel`)
- Use smart pointers (`std::shared_ptr`, `std::unique_ptr`) for memory management
- Prefer const correctness

### API Export/Import
- Use `HAZEL_API` macro for symbols that need to be exported from Hazel.dll
- Defined in `Hazel/src/Hazel/Core.h`
- Applied to classes, functions that need to be accessible from HazelEditor/Sandbox

### ImGui Usage
- ImGui context is created and managed by `ImGuiLayer` in Hazel.dll
- HazelEditor must use `ImGui::SetCurrentContext()` to share the context
- Only include ImGui core files in HazelEditor, not backends
- Backends (imgui_impl_glfw, imgui_impl_opengl3) only in Hazel.dll

## Documentation

Always reference these files for detailed information:
- `README.md` - Overview and features
- `ARCHITECTURE.md` - System architecture details
- `DEVELOPER_GUIDE.md` - Developer guidelines and examples
- `BUILD_TROUBLESHOOTING.md` - Build error solutions
- `DOCKING_IMPLEMENTATION.md` - Docking system implementation
- `GUI_IMPLEMENTATION.md` - GUI implementation details
- `EDITOR.md` - Editor panel descriptions
- `3D_RENDERING_GUIDE.md` - 3D rendering system guide

## Testing and Validation

### Manual Testing
- Build and run HazelEditor to verify GUI functionality
- Verify all panels are dockable and interactive
- Check console for log messages (no errors on startup)
- Test Scene Hierarchy, Inspector, and other panels
- Build and run Sandbox to verify 3D rendering (rotating cube)

### Expected Behavior
- **HazelEditor**: 1920x1080 window with Unity-like interface, all panels functional
- **Sandbox**: Window with blue rotating cube using Phong lighting

### Critical Systems to Test
- Application initialization and main loop
- Layer stack updates
- ImGui rendering and docking
- Event system (keyboard, mouse, window events)
- Logging output to console panel
- 3D rendering pipeline (if modified)

## C# Scripting

### Script Structure
```csharp
using Hazel;

public class ScriptName : ScriptComponent
{
    public float fieldName = 1.0f;
    
    void OnCreate() { }
    void OnUpdate(float deltaTime) { }
    void OnDestroy() { }
}
```

### Script Integration
- Scripts are located in the `Scripts/` directory
- C++ code interacts with scripts via `ScriptComponent` class
- Mono runtime integration for C# execution
- Future: hot-reloading and field inspection

## Common Tasks

### Adding a New Editor Panel
1. Create panel class in HazelEditor/src/
2. Add ImGui window in EditorLayer::OnImGuiRender()
3. Use appropriate ImGui widgets (Begin/End, TreeNode, Button, etc.)
4. Handle user input and update panel state

### Adding a New Engine System
1. Create header/implementation in Hazel/src/Hazel/
2. Add HAZEL_API export if needed from DLL
3. Integrate with Application or Layer system
4. Update documentation in ARCHITECTURE.md

### Adding a New Renderer Feature
1. Implement in Hazel/src/Hazel/Renderer/
2. Follow existing patterns (Shader, Material, Buffer classes)
3. Use OpenGL 3.3 Core Profile features only
4. Test with Sandbox application

## Dependencies and Vendor Libraries

### ImGui
- Version: Docking branch
- Location: vendor/imgui/
- Used for all editor UI
- Compiled into both Hazel.dll and HazelEditor.exe (core only)

### GLFW
- Version: 3.x
- Location: vendor/glfw/
- Used for window creation and input
- Compiled only into Hazel.dll

### GLM
- Version: Latest
- Location: vendor/glm/
- Header-only math library
- Used for all vector/matrix operations

### Modifications to Vendor Code
- Avoid modifying vendor libraries directly
- Use preprocessor definitions to suppress warnings if needed
- Document any necessary changes in BUILD_TROUBLESHOOTING.md

## Important Notes for AI Assistance

1. **DLL Boundaries**: Be careful with symbols crossing Hazel.dll boundaries. Use HAZEL_API for exports.

2. **ImGui Context**: The ImGui context must be shared between Hazel.dll and HazelEditor.exe. Don't create multiple contexts.

3. **Build Configuration**: Always build for x64. The project doesn't support x86.

4. **Minimal Changes**: Prefer minimal, surgical changes over large refactorings.

5. **Existing Patterns**: Follow existing patterns in the codebase for consistency.

6. **Testing**: Always test changes with both HazelEditor and Sandbox projects when modifying core engine code.

7. **Documentation**: Update relevant documentation files when making architectural changes.

8. **Visual Studio Specific**: The build system is MSBuild/Visual Studio specific. Don't add CMake or other build systems without discussion.

## Future Development Phases

See README.md for detailed roadmap. Key upcoming features:
- Entity-Component-System (ECS) architecture
- Scene serialization and prefab system
- Gizmos for transform manipulation
- Physics integration
- Audio system
- Advanced editor features (undo/redo, asset importing)

## Getting Help

For questions about:
- **Architecture**: See ARCHITECTURE.md
- **Build Issues**: See BUILD_TROUBLESHOOTING.md
- **Development**: See DEVELOPER_GUIDE.md
- **3D Rendering**: See 3D_RENDERING_GUIDE.md
- **Editor Implementation**: See GUI_IMPLEMENTATION.md
