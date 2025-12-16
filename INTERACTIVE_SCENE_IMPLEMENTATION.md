# Interactive 3D Scene View Implementation Guide

## Overview

This implementation adds a fully interactive 3D Scene view to the Hazel Editor, similar to Unity's Scene view. Users can now navigate a 3D environment, create and manipulate primitive objects, and edit their properties in real-time.

## Key Features Implemented

### 1. 3D Primitive Mesh Generation (`MeshGenerator`)

**Location**: `Hazel/src/Hazel/Renderer/MeshGenerator.h/cpp`

The `MeshGenerator` class provides static methods to generate common 3D primitive meshes:

- **CreateCube(size)**: Generates a cube mesh with proper normals for lighting
- **CreateSphere(radius, segments)**: Generates a UV sphere with adjustable detail
- **CreateCapsule(height, radius, segments)**: Generates a capsule (cylinder with hemisphere caps)

Each method returns a `MeshData` struct containing:
- `std::vector<float> Vertices`: Interleaved position (xyz) and normal (xyz) data
- `std::vector<unsigned int> Indices`: Triangle indices for indexed rendering

**Usage Example**:
```cpp
MeshData cubeData = MeshGenerator::CreateCube(1.0f);
MeshData sphereData = MeshGenerator::CreateSphere(0.5f, 32);
MeshData capsuleData = MeshGenerator::CreateCapsule(1.0f, 0.5f, 32);
```

### 2. Framebuffer Rendering (`Framebuffer`)

**Location**: `Hazel/src/Hazel/Renderer/Framebuffer.h/cpp`

The `Framebuffer` class enables off-screen rendering to textures:

- Creates a color attachment texture for rendering
- Creates a depth/stencil renderbuffer for proper 3D depth testing
- Supports runtime resizing for responsive viewports
- Provides texture ID for display in ImGui windows

**Key Methods**:
- `Bind()`: Activate framebuffer for rendering
- `Unbind()`: Deactivate framebuffer (return to screen)
- `Resize(width, height)`: Resize framebuffer textures
- `GetColorAttachment()`: Get texture ID for display

### 3. Editor Camera (`EditorCamera`)

**Location**: `Hazel/src/Hazel/Renderer/EditorCamera.h/cpp`

The `EditorCamera` class extends the base `Camera` class with interactive controls:

**Camera Controls**:
- **Mouse**: Right-click and drag to rotate view (FPS-style)
- **W/S**: Move forward/backward
- **A/D**: Move left/right
- **Q/E**: Move down/up
- **Mouse Scroll**: Zoom in/out (adjusts FOV)

**Features**:
- Automatic yaw/pitch calculation with pitch clamping (-89° to +89°)
- Configurable movement speed and mouse sensitivity
- Smooth movement with delta time integration
- Proper vector calculations for 3D navigation

### 4. Enhanced Entity System

**Location**: `HazelEditor/src/EditorLayer.h`

Entities now have full 3D support:

**Transform Component**:
```cpp
struct Transform {
    glm::vec3 Position;  // World position
    glm::vec3 Rotation;  // Euler angles (degrees)
    glm::vec3 Scale;     // Local scale
    glm::mat4 GetTransformMatrix() const;
};
```

**Entity Properties**:
- `Name`: Display name in hierarchy
- `ID`: Unique identifier
- `IsSelected`: Selection state
- `EntityTransform`: Position, rotation, and scale
- `Mesh`: Type of mesh (None, Cube, Sphere, Capsule)
- `Color`: RGBA color for rendering

### 5. Scene View Rendering

**Location**: `HazelEditor/src/EditorLayer.cpp - DrawSceneView()`

The Scene view now renders an interactive 3D viewport:

**Rendering Pipeline**:
1. Check viewport size and resize framebuffer if needed
2. Bind framebuffer for off-screen rendering
3. Clear with background color
4. Set up lighting (directional light)
5. Begin scene with editor camera
6. Render all entities with meshes
7. End scene
8. Unbind framebuffer
9. Display framebuffer texture in ImGui window

**Input Handling**:
- Detects when viewport is focused/hovered
- Processes mouse input for camera rotation
- Processes keyboard input for camera movement
- Hides cursor during camera rotation

### 6. GameObject Creation Menu

**Location**: `HazelEditor/src/EditorLayer.cpp - DrawMenuBar()`

The GameObject menu now creates actual 3D objects:

- **Create Empty**: Empty game object with no mesh
- **3D Object → Cube**: Creates a 1x1x1 cube (red color)
- **3D Object → Sphere**: Creates a sphere with 0.5 radius (green color)
- **3D Object → Capsule**: Creates a capsule 1 unit tall (blue color)

Each created object:
- Appears in the hierarchy immediately
- Has a unique ID
- Has default transform (0,0,0 position, identity rotation, unit scale)
- Has a default color based on mesh type
- Can be selected and edited

### 7. Inspector Transform Editing

**Location**: `HazelEditor/src/EditorLayer.cpp - DrawInspector()`

The Inspector now shows real, editable properties:

**Transform Section**:
- **Position**: DragFloat3 with 0.1 step size
- **Rotation**: DragFloat3 with 1.0° step size
- **Scale**: DragFloat3 with 0.1 step size

**Mesh Renderer Section** (if entity has mesh):
- **Mesh Type**: Dropdown to change mesh (Cube/Sphere/Capsule)
- **Color**: RGBA color picker

All changes are **immediately reflected** in the Scene view rendering.

## Technical Implementation Details

### Buffer Management

The editor maintains shared pointers to vertex/index buffers to ensure proper lifetime:

```cpp
// Member variables in EditorLayer
std::shared_ptr<Hazel::VertexArray> m_CubeMesh;
std::shared_ptr<Hazel::VertexBuffer> m_CubeVertexBuffer;
std::shared_ptr<Hazel::IndexBuffer> m_CubeIndexBuffer;
// Similar for sphere and capsule
```

This ensures buffers aren't destroyed while still referenced by VertexArray.

### Mesh Initialization

Meshes are created once during `OnAttach()` and reused:

```cpp
void InitializeMeshBuffers() {
    // Generate mesh data
    MeshData cubeData = MeshGenerator::CreateCube(1.0f);
    
    // Create vertex buffer
    m_CubeVertexBuffer = std::make_shared<VertexBuffer>(
        cubeData.Vertices.data(), 
        cubeData.Vertices.size() * sizeof(float));
        
    // Set layout (position + normal)
    m_CubeVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Normal" }
    });
    
    // Create and configure vertex array
    m_CubeMesh = std::make_shared<VertexArray>();
    m_CubeMesh->AddVertexBuffer(m_CubeVertexBuffer.get());
    m_CubeMesh->SetIndexBuffer(m_CubeIndexBuffer.get());
}
```

### Scene Rendering Loop

```cpp
void RenderScene() {
    // Render to framebuffer
    m_SceneFramebuffer->Bind();
    Renderer::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
    Renderer::Clear();
    
    // Setup lighting
    Renderer::ClearLights();
    Renderer::AddLight(m_SceneLight);
    
    // Render all entities
    Renderer::BeginScene(*m_EditorCamera);
    for (auto& entity : m_Entities) {
        if (entity.Mesh != MeshType::None) {
            auto meshVA = GetMeshVertexArray(entity.Mesh);
            m_DefaultMaterial->SetColor(entity.Color);
            glm::mat4 transform = entity.EntityTransform.GetTransformMatrix();
            Renderer::Submit(meshVA, m_DefaultMaterial, transform);
        }
    }
    Renderer::EndScene();
    
    m_SceneFramebuffer->Unbind();
}
```

## Build Configuration

### Updated Project Files

**Hazel/Hazel.vcxproj**:
Added new source files:
- `src\Hazel\Renderer\MeshGenerator.cpp`
- `src\Hazel\Renderer\Framebuffer.cpp`
- `src\Hazel\Renderer\EditorCamera.cpp`

Added new header files:
- `src\Hazel\Renderer\MeshGenerator.h`
- `src\Hazel\Renderer\Framebuffer.h`
- `src\Hazel\Renderer\EditorCamera.h`

**Hazel/src/Hazel.h**:
Added includes for new renderer classes.

## Usage Guide

### Creating Objects

1. Use **GameObject → 3D Object** menu
2. Select Cube, Sphere, or Capsule
3. Object appears at origin (0,0,0)
4. Object is automatically added to hierarchy

### Navigating the Scene

1. **Focus the Scene window** (click on it)
2. **Right-click and drag** to rotate camera
3. **Hold W** to move forward
4. **Hold S** to move backward
5. **Hold A** to move left
6. **Hold D** to move right
7. **Hold Q** to move down
8. **Hold E** to move up

### Editing Objects

1. **Select object** in Hierarchy
2. **Inspector** shows object properties
3. **Drag** Position/Rotation/Scale values
4. **Changes apply immediately** in Scene view
5. **Color picker** changes object color
6. **Mesh dropdown** changes object mesh type

### Best Practices

- **Start with Scene focused**: Camera controls only work when Scene is focused
- **Use right-click for camera**: Prevents accidental clicks in the scene
- **Adjust camera speed**: Default is 5 units/second, modify in EditorCamera if needed
- **Multiple objects**: Create several objects and arrange them in 3D space
- **Color coding**: Use different colors to distinguish objects

## Performance Characteristics

### Memory Usage
- Meshes created once and shared across instances
- Each entity stores only transform and metadata (~100 bytes)
- Framebuffer texture uses: width × height × 3 bytes (RGB)

### Rendering Performance
- One draw call per visible entity
- Efficient indexed rendering (6 indices per cube face)
- Depth testing prevents overdraw
- Default clear color: dark gray (0.2, 0.2, 0.2)

### Optimization Tips
- Sphere/Capsule segments default to 32 (balanced quality/performance)
- Reduce segments for faster rendering: `CreateSphere(0.5f, 16)`
- Increase segments for smoother appearance: `CreateSphere(0.5f, 64)`

## Known Limitations

1. **No Click Selection**: Objects cannot be selected by clicking in Scene view yet
   - Workaround: Use Hierarchy to select objects
   - Future: Implement ray-casting for mouse picking

2. **No Gizmos**: No visual transform gizmos (move/rotate/scale handles)
   - Workaround: Use Inspector drag controls
   - Future: Implement ImGuizmo integration

3. **No Grid**: No reference grid in Scene view
   - Future: Add grid rendering overlay

4. **Fixed Light**: Directional light is fixed, not editable
   - Future: Make light an editable entity

5. **No Textures**: Materials only support solid colors
   - Future: Add texture support to Material class

## Future Enhancements

### Short Term
- [ ] Click-to-select objects in Scene view (ray-casting)
- [ ] Visual gizmos for transform manipulation
- [ ] Scene grid and axes
- [ ] Multiple selection (Shift+Click)
- [ ] Delete key to remove objects

### Medium Term
- [ ] Object duplication (Ctrl+D)
- [ ] Undo/Redo system
- [ ] Scene save/load
- [ ] More primitives (plane, cylinder, cone)
- [ ] Editable lights in hierarchy

### Long Term
- [ ] Texture support
- [ ] Model import (OBJ, FBX)
- [ ] Physics integration
- [ ] Prefab system
- [ ] Scene-to-scene references

## Testing Checklist

When building and testing this implementation:

- [ ] HazelEditor compiles without errors
- [ ] Scene view displays a 3D viewport
- [ ] Camera responds to WASD+QE keys when focused
- [ ] Camera rotates with right-click drag
- [ ] GameObject menu creates visible objects
- [ ] Objects appear in Hierarchy
- [ ] Selecting object in Hierarchy updates Inspector
- [ ] Inspector shows Position/Rotation/Scale
- [ ] Changing transform updates Scene rendering
- [ ] Color picker changes object color
- [ ] Creating multiple objects works correctly
- [ ] Framebuffer resizes with window

## Troubleshooting

### Scene View is Black
- Check that Renderer::Init() was called
- Verify framebuffer creation succeeded
- Ensure entities have valid meshes
- Check that camera is positioned correctly

### Camera Won't Move
- Ensure Scene window is focused (click on it)
- Check that WASD keys are not captured elsewhere
- Verify EditorCamera OnUpdate is being called

### Objects Don't Appear
- Check entity Mesh type is not MeshType::None
- Verify mesh buffers were initialized
- Ensure entity is in camera view (default camera at 0,5,10)
- Check that material shader compiled successfully

### Inspector Changes Don't Apply
- Verify m_SelectedEntity is set correctly
- Check that RenderScene() is called each frame
- Ensure transform matrix calculation is correct

## Code Review Notes

This implementation follows Hazel's existing patterns:

- Uses shared_ptr for resource management
- Follows DLL export patterns (HAZEL_API)
- Integrates with existing Renderer API
- Maintains separation between engine and editor
- Uses ImGui for all UI rendering
- Follows existing naming conventions

## Summary

This implementation successfully adds Unity-like 3D Scene editing to the Hazel Editor. Users can now:

1. ✅ Navigate the scene freely (fly camera)
2. ✅ Add primitive 3D objects (cube, sphere, capsule)
3. ✅ Select objects (via Hierarchy)
4. ✅ Modify transforms in Inspector
5. ✅ See changes in real-time

The foundation is now in place for more advanced features like gizmos, click-selection, and scene persistence.
