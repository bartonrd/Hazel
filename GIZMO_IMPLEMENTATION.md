# Transform Gizmo Implementation Guide

## Overview

This document describes the implementation of transform gizmos in the Hazel Editor, allowing users to visually manipulate game object transforms (position, rotation, scale) in the Scene view using interactive 3D handles.

## Features Implemented

### 1. ImGuizmo Integration

**Library**: ImGuizmo v1.92.5
**Location**: `vendor/ImGuizmo/`

ImGuizmo is a popular immediate-mode 3D gizmo library built on top of ImGui. It provides:
- Translation (move) gizmos with X, Y, Z axis handles
- Rotation gizmos with circular rotation handles
- Scale gizmos with axis-aligned scaling handles
- Unified gizmo with all three combined
- Local and world space transformation modes
- Snapping support for grid-aligned transformations

### 2. Gizmo Operation Modes

**Enum**: `GizmoOperation` in `EditorLayer.h`

Three gizmo modes are supported:
- **Translate**: Move objects along X, Y, Z axes (activated with Q or W key)
- **Rotate**: Rotate objects around X, Y, Z axes (activated with E key)
- **Scale**: Scale objects along X, Y, Z axes (activated with R key)

### 3. User Interface Integration

#### Toolbar Buttons
- Added three buttons to the left side of the toolbar showing current gizmo mode
- Buttons display keyboard shortcuts: "Q Move", "E Rotate", "R Scale"
- Active mode is highlighted with brackets, e.g., "[Q] Move"
- Clicking a button changes the gizmo mode

#### Keyboard Shortcuts
- **Q**: Switch to Translate mode
- **W**: Switch to Translate mode (Unity-compatible)
- **E**: Switch to Rotate mode
- **R**: Switch to Scale mode
- **Shift + WASD/QE**: Camera movement (overrides gizmo mode temporarily)

### 4. Scene View Interaction

#### Gizmo Rendering
Location: `EditorLayer::DrawSceneView()`

The gizmo is rendered in the Scene view using ImGuizmo's API:
1. Set up viewport bounds for correct mouse interaction
2. Get camera view and projection matrices from EditorCamera
3. Get selected entity's transform matrix
4. Call `ImGuizmo::Manipulate()` to render and handle gizmo interaction
5. If the gizmo is being manipulated, decompose the result matrix back to position/rotation/scale
6. Update the entity's transform components

#### Mouse Picking
Location: `EditorLayer::HandleSceneViewMousePicking()`

When clicking in the Scene view (and not on the gizmo):
- Left-click cycles through selectable entities (those with meshes)
- Only triggered when mouse is over viewport and not over gizmo handles
- Updates selection in both Scene Hierarchy and Inspector

### 5. Entity Selection

Entities can be selected in two ways:
1. **Hierarchy Panel**: Click on entity name in the tree view
2. **Scene View**: Left-click in the viewport (cycles through mesh entities)

When an entity is selected:
- It's highlighted in the Hierarchy panel
- Its properties appear in the Inspector panel
- Transform gizmo appears at the entity's position in the Scene view

## Implementation Details

### Key Code Changes

#### EditorLayer.h
```cpp
// Added GizmoOperation enum
enum class GizmoOperation
{
    Translate,
    Rotate,
    Scale
};

// Added member variables
GizmoOperation m_GizmoOperation = GizmoOperation::Translate;
bool m_GizmoEnabled = true;

// Added helper function
void HandleSceneViewMousePicking();
```

#### EditorLayer.cpp
```cpp
// Added ImGuizmo header
#include <ImGuizmo.h>

// Gizmo rendering in DrawSceneView()
if (m_SelectedEntity && m_GizmoEnabled && m_SelectedEntity->Mesh != MeshType::None)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(...);
    
    // Determine operation based on mode
    ImGuizmo::OPERATION operation = ...;
    
    // Render gizmo and get updated transform
    ImGuizmo::Manipulate(...);
    
    // Update entity transform if gizmo was used
    if (ImGuizmo::IsUsing())
    {
        ImGuizmo::DecomposeMatrixToComponents(...);
        // Update entity position, rotation, scale
    }
}

// Mouse picking
if (m_ViewportHovered && !ImGuizmo::IsOver() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
{
    HandleSceneViewMousePicking();
}
```

#### HazelEditor.vcxproj
```xml
<!-- Added ImGuizmo include directory -->
<AdditionalIncludeDirectories>
    ...;$(SolutionDir)vendor\ImGuizmo;...
</AdditionalIncludeDirectories>

<!-- Added ImGuizmo source file -->
<ClCompile Include="..\vendor\ImGuizmo\ImGuizmo.cpp" />
```

### Transform Matrix Flow

1. **Entity → Matrix**: `Transform::GetTransformMatrix()` creates a 4x4 matrix from position, rotation, scale
2. **Gizmo Manipulation**: `ImGuizmo::Manipulate()` modifies the matrix based on user input
3. **Matrix → Entity**: `ImGuizmo::DecomposeMatrixToComponents()` extracts position, rotation, scale back

### Viewport Coordinate System

ImGuizmo requires correct viewport bounds to map mouse coordinates:
```cpp
ImVec2 windowPos = ImGui::GetWindowPos();
ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

m_ViewportBounds[0] = { windowPos.x + contentMin.x, windowPos.y + contentMin.y };
m_ViewportBounds[1] = { windowPos.x + contentMax.x, windowPos.y + contentMax.y };

ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, 
                  m_ViewportBounds[1].x - m_ViewportBounds[0].x, 
                  m_ViewportBounds[1].y - m_ViewportBounds[0].y);
```

## Usage

### For Users

1. **Select an Object**:
   - Click on an entity in the Hierarchy panel, OR
   - Left-click in the Scene view to cycle through objects

2. **Choose Gizmo Mode**:
   - Press Q or W for Translate (move)
   - Press E for Rotate
   - Press R for Scale
   - Or click the toolbar buttons

3. **Manipulate Transform**:
   - **Translate**: Click and drag the X (red), Y (green), or Z (blue) axis arrows
   - **Rotate**: Click and drag the rotation circles around each axis
   - **Scale**: Click and drag the axis handles to scale along each dimension

4. **Camera Controls**:
   - Hold Shift while using WASD/QE for camera movement
   - Right-click and drag to rotate camera
   - Camera controls work independently of gizmo mode

### For Developers

To extend or modify gizmo functionality:

1. **Add New Gizmo Modes**:
   - Add to `GizmoOperation` enum
   - Update switch statement in `DrawSceneView()`
   - Add keyboard shortcut and toolbar button

2. **Customize Gizmo Appearance**:
   - ImGuizmo supports custom colors and styles
   - See ImGuizmo documentation for styling options

3. **Add Snapping**:
   ```cpp
   float snapValues[3] = { 0.5f, 0.5f, 0.5f }; // Grid snap
   ImGuizmo::Manipulate(..., snapValues);
   ```

4. **Add Local/World Space Toggle**:
   ```cpp
   ImGuizmo::MODE mode = m_UseLocalSpace ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
   ImGuizmo::Manipulate(..., operation, mode, ...);
   ```

## Future Enhancements

Potential improvements for the gizmo system:

1. **Advanced Mouse Picking**: 
   - Implement ray-casting against mesh geometry
   - Highlight hovered objects before selection
   - Multi-select with Ctrl+Click

2. **Gizmo Settings**:
   - Toggle between local and world space
   - Adjustable gizmo size
   - Snapping grid configuration
   - Gizmo color customization

3. **Visual Feedback**:
   - Outline selected objects in Scene view
   - Show axis-aligned bounding boxes
   - Display transform values during manipulation

4. **Undo/Redo**:
   - Track transform changes for undo system
   - Store original transform when starting manipulation
   - Push to undo stack when manipulation completes

5. **Multi-Selection**:
   - Manipulate multiple objects simultaneously
   - Maintain relative positions/rotations
   - Center gizmo at selection center

## Troubleshooting

### Gizmo Not Appearing
- Ensure an entity with a mesh is selected (not Camera or Light)
- Check that `m_GizmoEnabled` is true
- Verify entity transform is valid (no NaN values)

### Gizmo Not Responding to Mouse
- Check viewport bounds are correctly calculated
- Ensure ImGuizmo::SetRect() is called with correct coordinates
- Verify ImGui window is focused and hovered

### Transforms Not Updating
- Check that `ImGuizmo::IsUsing()` returns true during manipulation
- Verify DecomposeMatrixToComponents is extracting correct values
- Ensure entity transform is being updated in the if statement

### Keyboard Shortcuts Conflicting
- Camera movement uses Shift modifier to avoid conflicts
- Gizmo shortcuts only work when Scene view is focused
- Check ImGui key state with `ImGui::IsKeyPressed()` for debugging

## References

- [ImGuizmo GitHub Repository](https://github.com/CedricGuillemet/ImGuizmo)
- [ImGui Documentation](https://github.com/ocornut/imgui)
- [GLM Documentation](https://glm.g-truc.net/)
