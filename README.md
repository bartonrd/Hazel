# Hazel Game Engine

Hazel is a 3D game engine similar to Unity, built with C++ and designed for high-performance game development. It features a **fully functional graphical user interface** built with ImGui and GLFW.

## 🎮 HazelEditor - Unity-like Editor

The Hazel Editor provides a professional, Unity-inspired interface with **real graphical windows** and interactive panels:

### Core Panels
- **Scene Hierarchy**: Interactive tree view of all game objects with selection and parent-child relationships
- **Inspector**: Real-time property editor with drag controls for Transform (Position, Rotation, Scale) and component fields
- **Console**: Color-coded log viewer with filters (Trace/Info/Warn/Error/Fatal) and auto-scroll
- **Scene View**: 3D scene editor viewport with **interactive transform gizmos** and tool selection (Q/W/E/R for Move/Rotate/Scale)
- **Game View**: Runtime game preview viewport
- **Asset Browser**: Project asset management with folder tree and file grid
- **Toolbar**: Play/Pause/Step controls for game testing, plus gizmo mode selection
- **Menu Bar**: Complete menu system (File, Edit, Assets, GameObject, Component, Window, Help)

### GUI Features
- **Full Docking System**: Unity-like drag and drop panels anywhere in the editor
  - Dock windows within the main editor window
  - Dock windows to each other (tabs or splits)
  - Detach windows to separate OS windows
  - Dock windows within detached windows
- **Multi-Viewport**: Full multi-monitor support with detached panels
- **Dark Theme**: Unity-like color scheme
- **Keyboard Navigation**: Full keyboard support
- **60 FPS**: Smooth VSync rendering
- **1920x1080**: Default window resolution

### Projects

**HazelEditor**: The main Unity-like editor application with GUI  
**Sandbox**: Basic example demonstrating engine features  

## 🚀 Features

### 1. Real GUI Interface (NEW!)
Built with ImGui and GLFW:
- **GLFW Window System**: Native OpenGL windows with event handling
- **ImGui Rendering**: Immediate mode GUI with docking support
- **OpenGL 3.3**: Core Profile for modern graphics
- **Interactive Widgets**: Buttons, sliders, tree views, text inputs, drag controls
- **Event Handling**: Mouse clicks, keyboard input, window events

### 2. Transform Gizmos (NEW!)
Built with ImGuizmo library for professional 3D manipulation:
- **Visual Transform Handles**: Drag colored arrows, circles, and handles to transform objects
- **Three Gizmo Modes**: 
  - **Translate** (Q/W): Move objects along X/Y/Z axes with red/green/blue arrows
  - **Rotate** (E): Rotate objects around axes with circular handles
  - **Scale** (R): Scale objects with axis-aligned handles
- **Mouse Selection**: Left-click in Scene view to select objects
- **Real-time Updates**: Transform changes instantly reflected in Inspector panel
- **Keyboard Shortcuts**: Unity-compatible shortcuts for quick mode switching
- **Visual Feedback**: Active mode shown in toolbar with brackets
- **Camera Integration**: Gizmo-aware camera controls with Shift modifier

Usage:
- Select object in Hierarchy or Scene view
- Press Q (Move), E (Rotate), or R (Scale)
- Drag colored handles to transform the object
- See [GIZMO_USER_GUIDE.md](GIZMO_USER_GUIDE.md) for detailed instructions

### 3. Logging System
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

### 4. Modular Dockable Windows
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

### 5. C# Scripting Support
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

### 5. 3D Rendering System
Hazel includes a modern 3D rendering pipeline with support for materials and lighting:

**Features:**
- Camera system (Perspective and Orthographic projection)
- Shader management with GLSL 3.3
- Material system with PBR-ready properties
- Multiple light types (Directional, Point, Spot)
- Vertex/Index buffer management
- Phong lighting model

**Example 3D Scene:**
```cpp
using namespace Hazel;

// Create a camera
auto camera = std::make_shared<Camera>(Camera::ProjectionType::Perspective);
camera->SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

// Create a cube
float vertices[] = {
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // ... more vertices
};

unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,  // front face
    // ... more indices
};

// Create vertex array
auto vertexArray = std::make_shared<VertexArray>();
auto vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
vertexBuffer->SetLayout({
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" }
});
vertexArray->AddVertexBuffer(vertexBuffer.get());
auto indexBuffer = std::make_shared<IndexBuffer>(indices, 36);
vertexArray->SetIndexBuffer(indexBuffer.get());

// Create shader with vertex and fragment source
auto shader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

// Create material
auto material = std::make_shared<Material>(shader);
material->SetColor(glm::vec4(0.3f, 0.6f, 0.9f, 1.0f));
material->SetShininess(32.0f);

// Create directional light
auto light = std::make_shared<DirectionalLight>();
light->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
light->SetIntensity(1.0f);

// In your update loop:
Renderer::ClearLights();
Renderer::AddLight(light);
Renderer::BeginScene(*camera);

glm::mat4 transform = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.5f, 1.0f, 0.0f));
Renderer::Submit(vertexArray, material, transform);

Renderer::EndScene();
```

**Available Light Types:**
- `DirectionalLight` - Simulates sunlight with parallel rays
- `PointLight` - Light source that emits in all directions with attenuation
- `SpotLight` - Cone-shaped light with inner and outer cutoff angles

**Material Properties:**
- Color (RGBA)
- Shininess (specular intensity)
- Metallic (0.0 to 1.0)
- Roughness (0.0 to 1.0)

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

### Rendering Architecture
Modern OpenGL 3.3 Core Profile rendering:
- Vertex Array Objects (VAO) for geometry
- Vertex Buffer Objects (VBO) and Index Buffer Objects (IBO)
- Shader programs with uniform management
- Material system for shader parameters
- Scene graph with camera and lighting

## Building the Hazel Editor

### Prerequisites
- **Visual Studio 2022** (v143 toolset)
- **Windows 10 SDK** 
- **C++17** or later
- **OpenGL** support (included with Windows)
- **GLM** (included in vendor)

### Build Instructions
1. Open `Hazel.sln` in Visual Studio 2022
2. Set **HazelEditor** as the startup project (Right-click → Set as Startup Project)
3. Select **Debug|x64** or **Release|x64** configuration
4. Build the solution (Ctrl+Shift+B)
5. Run the editor (F5 or Ctrl+F5)

### Expected Output
When you run HazelEditor, you will see:
- A 1920x1080 window titled "Hazel Editor"
- Full Unity-like interface with interactive panels
- Dark theme matching Unity's design
- Dockable panels that can be arranged freely
- Working hierarchy, inspector, console, and other panels

### Alternative: Run Sandbox
For a 3D rendering demo with rotating cube:
1. Right-click **Sandbox** project → Set as Startup Project
2. Build and run (F5)
3. You'll see a blue rotating cube with Phong lighting

### Project Structure
```
Hazel/
├── Hazel/                  # Engine core (DLL)
│   ├── src/
│   │   ├── Hazel/
│   │   │   ├── Application.h/cpp      # GLFW window & main loop
│   │   │   ├── Log.h/cpp               # Logging system
│   │   │   ├── Layer.h/cpp             # Layer base class
│   │   │   ├── LayerStack.h/cpp        # Layer management
│   │   │   ├── Core.h                  # Core macros (HAZEL_API)
│   │   │   ├── Events/
│   │   │   │   └── Event.h             # Event system
│   │   │   ├── ImGui/
│   │   │   │   └── ImGuiLayer.h/cpp    # ImGui integration
│   │   │   ├── Renderer/
│   │   │   │   ├── Renderer.h/cpp      # Main renderer
│   │   │   │   ├── Shader.h/cpp        # Shader management
│   │   │   │   ├── Buffer.h/cpp        # VBO/IBO/VAO
│   │   │   │   ├── Camera.h/cpp        # Camera system
│   │   │   │   ├── Material.h/cpp      # Material system
│   │   │   │   └── Light.h/cpp         # Lighting system
│   │   │   └── Scripting/
│   │   │       ├── ScriptEngine.h/cpp
│   │   │       └── ScriptComponent.h/cpp
│   │   └── Hazel.h                     # Main include file
│   └── Hazel.vcxproj                   # VS project with ImGui/GLFW
├── HazelEditor/            # Unity-like editor (EXE)
│   ├── src/
│   │   ├── EditorApp.cpp               # Editor entry point
│   │   └── EditorLayer.h/cpp           # All GUI panels
│   └── HazelEditor.vcxproj
├── Sandbox/                # Basic example (EXE)
│   ├── src/
│   │   └── SandboxApp.cpp
│   └── Sandbox.vcxproj
├── vendor/                 # Third-party libraries
│   ├── imgui/              # Dear ImGui (GUI)
│   │   ├── imgui.cpp
│   │   ├── imgui.h
│   │   └── backends/
│   │       ├── imgui_impl_glfw.cpp
│   │       └── imgui_impl_opengl3.cpp
│   ├── glfw/               # GLFW (windowing)
│   │   ├── include/
│   │   └── src/
│   └── glm/                # GLM (math library)
│       └── glm/
├── Scripts/                # C# game scripts
│   └── PlayerController.cs
└── Hazel.sln               # Visual Studio solution
```

## Documentation

- **[DOCKING_IMPLEMENTATION.md](DOCKING_IMPLEMENTATION.md)** - Unity-like docking system implementation
- **[GUI_IMPLEMENTATION.md](GUI_IMPLEMENTATION.md)** - Detailed GUI implementation guide
- **[EDITOR.md](EDITOR.md)** - Editor panel descriptions  
- **[EDITOR_IMPLEMENTATION.md](EDITOR_IMPLEMENTATION.md)** - Original editor implementation notes
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Engine architecture overview
- **[DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md)** - Developer guide

## Future Development

### Phase 1: Core Infrastructure ✅
- [x] Logging system
- [x] Event system
- [x] Layer system

### Phase 2: Window and GUI System ✅
- [x] GLFW window creation (1920x1080)
- [x] OpenGL 3.3 context setup
- [x] ImGui integration with backends
- [x] Docking system
- [x] Full Unity-like interface
- [x] Scene Hierarchy panel (tree view with selection)
- [x] Inspector panel (transform drag controls)
- [x] Console panel (color-coded, filtered logging)
- [x] Game View and Scene View panels
- [x] Asset Browser panel (folder tree + file grid)
- [x] Toolbar (Play/Pause/Step controls)
- [x] Menu bar (File, Edit, Assets, etc.)

### Phase 3: 3D Rendering Foundation ✅
- [x] Camera system (perspective and orthographic)
- [x] Basic 3D mesh rendering
- [x] Shader system
- [x] Material system
- [x] Lighting (directional, point, spot)
- [x] Vertex/Index buffer management
- [x] Vertex Array Objects
- [x] Phong lighting model
- [ ] Framebuffer rendering to viewports
- [ ] Texture support

### Phase 4: Scene and Entity System
- [ ] Entity-Component-System (ECS) architecture
- [ ] Scene graph for 3D objects
- [ ] Component serialization
- [ ] Scene save/load system
- [ ] Prefab system

### Phase 5: Editor Advanced Features
- [ ] Gizmos for transform manipulation (ImGuizmo)
- [ ] 3D object picking/selection
- [ ] Play mode with state save/restore
- [ ] Undo/Redo system
- [ ] Asset importing (FBX, OBJ, PNG, etc.)
- [ ] Drag-and-drop support

### Phase 6: C# Scripting Enhancement
- [x] Basic scripting framework
- [x] Script component system  
- [ ] Full Mono runtime integration
- [ ] Script hot-reloading
- [ ] Complete C++ to C# interop
- [ ] Script debugging support

### Phase 7: Advanced Features
- [ ] Animation system
- [ ] Particle effects
- [ ] Physics integration (PhysX)
- [ ] Audio system
- [ ] Profiler and performance tools

## Tech Stack

- **Language**: C++17
- **GUI**: ImGui (docking branch)
- **Windowing**: GLFW 3
- **Graphics**: OpenGL 3.3 Core
- **Math**: GLM (OpenGL Mathematics)
- **Scripting**: C# (Mono runtime)
- **Build System**: Visual Studio 2022 / MSBuild

## Contributing
This is a demonstration project showing the architecture of a Unity-like game engine. Contributions are welcome!

## License
MIT License - see LICENSE file for details

## Screenshots

The Hazel Editor features a professional Unity-like interface:
- **Dockable panels** that can be arranged freely
- **Interactive hierarchy** with tree nodes and selection
- **Property inspector** with drag controls
- **Filtered console** with color-coded messages
- **Multiple viewports** for scene and game views
- **Asset browser** with folder navigation

*Build and run the HazelEditor project to see the full interface!*
