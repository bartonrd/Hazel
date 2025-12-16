# Hazel Editor Docking Implementation

## Overview

This document describes the implementation of Unity-like draggable and dockable editor windows in the Hazel Editor. The implementation allows all editor panels to be freely rearranged, docked within the main window, docked to each other, and detached to separate OS windows.

## Changes Made

### 1. Upgraded ImGui to Docking Branch

**What Changed:**
- Replaced the standard ImGui library with the docking branch version
- Updated all core ImGui files (imgui.cpp, imgui.h, imgui_internal.h, etc.)
- Updated backend files (imgui_impl_glfw.cpp, imgui_impl_opengl3.cpp)

**Why:**
The standard ImGui release doesn't include docking functionality. The docking branch is a separate version maintained by the ImGui team that includes:
- Full docking support for windows
- Multi-viewport support (windows can be detached to separate OS windows)
- Enhanced window management features

**Files Updated:**
```
vendor/imgui/imgui.cpp
vendor/imgui/imgui.h
vendor/imgui/imgui_demo.cpp
vendor/imgui/imgui_draw.cpp
vendor/imgui/imgui_internal.h
vendor/imgui/imgui_tables.cpp
vendor/imgui/imgui_widgets.cpp
vendor/imgui/imstb_rectpack.h
vendor/imgui/imstb_textedit.h
vendor/imgui/imstb_truetype.h
vendor/imgui/backends/imgui_impl_glfw.cpp
vendor/imgui/backends/imgui_impl_glfw.h
vendor/imgui/backends/imgui_impl_opengl3.cpp
vendor/imgui/backends/imgui_impl_opengl3.h
vendor/imgui/backends/imgui_impl_opengl3_loader.h
```

### 2. Enabled Docking and Multi-Viewport Features

**File:** `Hazel/src/Hazel/ImGui/ImGuiLayer.cpp`

**Changes:**
```cpp
// Before (with #ifdef checks):
#ifdef IMGUI_HAS_DOCK_SPACE
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

// After (always enabled):
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport
```

**Why:**
- `ImGuiConfigFlags_DockingEnable`: Enables the docking system, allowing windows to be docked together
- `ImGuiConfigFlags_ViewportsEnable`: Allows windows to be pulled outside the main window and rendered in separate OS windows

### 3. Implemented Central DockSpace

**File:** `HazelEditor/src/EditorLayer.cpp`

**Changes:**
Added a fullscreen dockspace that serves as the main container for all editor panels.

**Key Features:**
- **Fullscreen Dockspace**: Covers the entire viewport, providing maximum docking area
- **No Background**: Uses `ImGuiDockNodeFlags_PassthruCentralNode` to allow rendering behind the dockspace
- **Persistent**: Remains active even when collapsed to maintain docking relationships
- **Menu Bar Integration**: Menu bar is integrated into the dockspace window

**Code Structure:**
```cpp
void EditorLayer::OnImGuiRender()
{
    // 1. Create fullscreen dockspace window
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    // 2. Set window flags (no title bar, no resize, etc.)
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    
    // 3. Create the dockspace
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    
    // 4. Draw menu bar
    DrawMenuBar();
    
    ImGui::End();
    
    // 5. Draw all panels - they will dock into the dockspace
    DrawToolbar();
    DrawSceneHierarchy();
    DrawInspector();
    DrawConsole();
    DrawGameView();
    DrawSceneView();
    DrawAssetBrowser();
}
```

### 4. Updated Menu Bar

**Changes:**
- Replaced `ImGui::BeginMainMenuBar()` with `ImGui::BeginMenuBar()`
- Menu bar now renders inside the dockspace window
- Uses the `ImGuiWindowFlags_MenuBar` flag on the dockspace window

**Why:**
The main menu bar is a global UI element that can't be part of the docking system. By integrating it into the dockspace window, it becomes part of the editor's main container while still remaining fixed at the top.

### 5. Made Toolbar Dockable

**Changes:**
```cpp
// Before (fixed window):
ImGui::Begin("Toolbar", nullptr, 
    ImGuiWindowFlags_NoTitleBar | 
    ImGuiWindowFlags_NoResize | 
    ImGuiWindowFlags_NoMove | 
    ImGuiWindowFlags_NoScrollbar);

// After (dockable window):
ImGui::Begin("Toolbar");
```

**Why:**
Removed window flags that prevented docking. The toolbar is now a regular dockable window that can be:
- Docked anywhere in the editor
- Resized by the user
- Moved freely
- Detached to a separate window

## How Docking Works

### Basic Docking Operations

1. **Dock a Window:**
   - Drag a window by its title bar
   - Move it near the edge of another window or the main viewport
   - Drop indicators will appear showing where the window will dock
   - Release to dock

2. **Undock a Window:**
   - Drag a docked window by its title bar
   - Pull it away from its docked position
   - It becomes a floating window

3. **Detach to Separate OS Window:**
   - Drag a window outside the main editor window
   - A new OS window will be created
   - The window can now be moved to another monitor

4. **Dock in Detached Windows:**
   - Windows can be docked within detached OS windows
   - Same docking behavior applies
   - Detached windows maintain their own dockspace

5. **Tab Windows:**
   - Dock a window exactly on top of another window
   - They will become tabs in the same docking node
   - Click tabs to switch between windows

### Docking Indicators

When dragging a window, ImGui shows visual indicators:
- **Center**: Dock as tabs in the current node
- **Top/Bottom/Left/Right**: Split the current node and dock to that side
- **Outer edges**: Dock to the edge of the dockspace

### Keyboard Shortcuts

- **Hold SHIFT while dragging**: Temporarily disable docking (window stays floating)
- **Drag from window menu button** (upper-left): Undock an entire docking node

## Benefits

### Unity-like Experience
The implementation provides a familiar experience for users coming from Unity:
- Flexible window arrangement
- Persistent layouts (can be saved/loaded)
- Multi-monitor support via detached windows
- Tab grouping for related panels

### Improved Workflow
- Customize layout to match workflow needs
- Save different layouts for different tasks
- Quick rearrangement without coding
- Multi-monitor support for more screen real estate

### Maintainability
- Standard ImGui docking API (well-documented)
- No custom docking implementation to maintain
- Regular updates from ImGui team
- Community support and examples

## Technical Details

### DockSpace vs Regular Windows

**DockSpace:**
- A container that can hold docked windows
- Created with `ImGui::DockSpace()`
- Manages the layout of docked windows
- Handles docking indicators and operations

**Regular Windows:**
- Created with `ImGui::Begin()`/`ImGui::End()`
- Can be docked into a dockspace
- Can also be floating or detached

### Multi-Viewport Support

When `ImGuiConfigFlags_ViewportsEnable` is enabled:
- Windows can be dragged outside the main window
- New OS windows are created automatically
- Each viewport has its own OpenGL context
- Rendering happens across all viewports
- Platform-specific window management is handled by ImGui backends

**Rendering Loop:**
```cpp
// In ImGuiLayer::End()
ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// Render platform windows
if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
{
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
}
```

### Window Flags

Important window flags for docking:

- `ImGuiWindowFlags_NoDocking`: Prevents window from being used as a docking target
- `ImGuiWindowFlags_NoMove`: Prevents window from being moved (also disables docking)
- `ImGuiWindowFlags_NoTitleBar`: Hides title bar (can still dock by dragging window)
- `ImGuiDockNodeFlags_PassthruCentralNode`: Makes dockspace background transparent

## Future Enhancements

### Planned Features
1. **Layout Saving/Loading**
   - Save custom layouts to files
   - Load layouts on startup
   - Preset layouts for different tasks

2. **Default Layout**
   - Initialize with a Unity-like default layout
   - First-time setup guide

3. **Layout Presets**
   - Animation layout
   - Debugging layout
   - Design layout
   - Quick-switch between layouts

4. **Window Management**
   - "Window" menu to show/hide panels
   - Reset to default layout option
   - Lock layout option

### Implementation Examples

**Save Layout:**
```cpp
// Save current docking layout to file
ImGui::DockBuilderCopyDockSpace(dockspace_id, &source_data);
// Serialize source_data to file
```

**Load Layout:**
```cpp
// Load docking layout from file
// Deserialize to source_data
ImGui::DockBuilderCopyDockSpace(source_data, &dockspace_id);
```

## Troubleshooting

### Windows Won't Dock
- Check that `ImGuiConfigFlags_DockingEnable` is set
- Ensure window doesn't have `ImGuiWindowFlags_NoMove` or `ImGuiWindowFlags_NoDocking` flags
- Verify ImGui docking branch is being used (not standard branch)

### Detached Windows Don't Show
- Check that `ImGuiConfigFlags_ViewportsEnable` is set
- Ensure platform backend supports multi-viewport
- Verify `ImGui::UpdatePlatformWindows()` is called in render loop

### Layout Doesn't Persist
- Implement layout saving/loading (not yet implemented)
- ImGui can save layouts to .ini file automatically if enabled

### Performance Issues
- Disable `ImGuiConfigFlags_ViewportsEnable` if not needed
- Reduce number of active viewports
- Check GPU/driver compatibility

## References

- [ImGui Docking Branch](https://github.com/ocornut/imgui/tree/docking)
- [ImGui Docking Documentation](https://github.com/ocornut/imgui/wiki/Docking)
- [ImGui Multi-Viewport Documentation](https://github.com/ocornut/imgui/wiki/Multi-Viewports)
- [ImGui Demo Window](https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp) - Shows docking examples

## Conclusion

The Hazel Editor now supports Unity-like docking functionality, allowing users to:
- Drag and dock windows anywhere in the main editor
- Dock windows to each other creating tabs or splits
- Detach windows to separate OS windows
- Dock windows within detached windows
- Arrange their workspace to match their workflow

This implementation uses the official ImGui docking branch, ensuring compatibility, maintainability, and access to future improvements from the ImGui team.
