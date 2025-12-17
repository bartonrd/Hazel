# Shutdown Access Violation Fix - Testing Guide

## Problem Fixed

Previously, when closing HazelEditor, an access violation crash occurred:
```
Exception thrown at 0x0000000000000000 in HazelEditor.exe: 0xC0000005: 
Access violation executing location 0x0000000000000000.
```

The crash happened in the IndexBuffer destructor at line 47 of `Hazel/src/Hazel/Renderer/Buffer.cpp`.

## Root Cause

The crash occurred due to the following sequence of events during shutdown:

1. **Application::~Application()** runs:
   - Lines 70-75: Calls `OnDetach()` on all layers
   - Lines 79-82: Deletes all layers (triggers EditorLayer destructor)
   - Lines 86-90: Destroys GLFW window and terminates GLFW (destroys OpenGL context)

2. **EditorLayer::~EditorLayer()** runs (at step 2 above):
   - The destructor is auto-generated and destroys all member variables
   - When `m_CubeIndexBuffer`, `m_SphereIndexBuffer`, etc. are destroyed, their `shared_ptr` destructors run
   - These call `IndexBuffer::~IndexBuffer()` which calls `glDeleteBuffers(1, &m_RendererID)`

3. **The Problem**:
   - By the time step 2 happens, the OpenGL context is still valid
   - However, when the EditorLayer destructor completes and returns, the Application destructor continues
   - Then GLFW is terminated, destroying the OpenGL context
   - **BUT** - the shared_ptr reference counting means the actual IndexBuffer destructor might be delayed
   - When it finally runs, `glDeleteBuffers` is a null function pointer because the OpenGL context is gone

## Solution Implemented

Modified `EditorLayer::OnDetach()` to explicitly release all OpenGL resources:

```cpp
void EditorLayer::OnDetach()
{
    // Release mesh buffers (must be done before VertexArrays)
    m_CubeIndexBuffer.reset();
    m_CubeVertexBuffer.reset();
    m_SphereIndexBuffer.reset();
    m_SphereVertexBuffer.reset();
    m_CapsuleIndexBuffer.reset();
    m_CapsuleVertexBuffer.reset();
    
    // Release vertex arrays
    m_CubeMesh.reset();
    m_SphereMesh.reset();
    m_CapsuleMesh.reset();
    
    // Release rendering resources
    m_SceneLight.reset();
    m_DefaultMaterial.reset();
    m_SceneShader.reset();
    m_SceneFramebuffer.reset();
    m_EditorCamera.reset();
}
```

This ensures OpenGL cleanup happens during `OnDetach()`, **before** the OpenGL context is destroyed.

## How to Test the Fix

### Prerequisites
- Visual Studio 2022
- Windows 10 SDK
- The project builds successfully

### Testing Steps

1. **Build the Project**:
   ```
   Open Hazel.sln in Visual Studio 2022
   Select configuration: Debug | x64
   Build Solution (Ctrl+Shift+B)
   ```

2. **Run HazelEditor**:
   ```
   Set HazelEditor as startup project
   Run with debugging (F5)
   ```

3. **Verify Normal Operation**:
   - The editor should launch successfully
   - You should see the Unity-like interface
   - Scene view should display the default cube and sphere
   - All panels should be functional

4. **Test Shutdown**:
   - Close the HazelEditor window (click X or Alt+F4)
   - **Expected Result**: Application closes cleanly without any errors
   - **Previous Behavior**: Access violation crash in IndexBuffer destructor

5. **Verify in Output Window**:
   Look for these log messages in the Visual Studio Output window:
   ```
   [INFO] EditorLayer::OnDetach
   [INFO] EditorLayer::OnDetach - All OpenGL resources released
   ```

6. **Test Multiple Times**:
   - Run and close the editor 3-5 times
   - Verify it closes cleanly every time
   - No access violations should occur

### Success Criteria

✅ **PASS**: Application closes without any errors or exceptions  
✅ **PASS**: Output window shows the OnDetach log messages  
✅ **PASS**: No access violation crashes occur  

❌ **FAIL**: If you still see the access violation error, please report:
- The exact error message
- The call stack
- The Visual Studio Output window log

## Technical Details

### Why reset() Instead of Destructor?

Calling `reset()` on shared_ptr immediately decrements the reference count and destroys the managed object if the count reaches zero. This gives us explicit control over **when** the OpenGL resources are destroyed, ensuring it happens while the OpenGL context is still valid.

### Order of Cleanup

The cleanup order matters:
1. **Index/Vertex Buffers first** - Must be released before VertexArrays that reference them
2. **Vertex Arrays** - Can be safely destroyed after buffers
3. **Materials and Shaders** - Can be destroyed after vertex arrays
4. **Framebuffers and Camera** - No dependencies, can be destroyed last

This order prevents any potential use-after-free issues during cleanup.

## Additional Notes

- This fix applies the same pattern used in other game engines (Unity, Unreal, etc.)
- OpenGL resources must always be destroyed before the OpenGL context is destroyed
- The `OnDetach()` method is the perfect place for this cleanup in the Hazel architecture
- This pattern should be followed by any other layers that create OpenGL resources

## Related Files Modified

- `HazelEditor/src/EditorLayer.cpp` - Added explicit resource cleanup in OnDetach()
