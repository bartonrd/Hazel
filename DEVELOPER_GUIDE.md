# Hazel Engine - Developer Guide

## Getting Started with Hazel

This guide will help you understand how to use and extend the Hazel game engine.

## Architecture Overview

Hazel follows a modular architecture with these key systems:

### 1. Application Layer
The `Application` class is the core of the engine. It manages:
- The main game loop
- Layer stack for organizing game logic
- Initialization and shutdown of engine systems

### 2. Logging System

#### Basic Usage
```cpp
#include <Hazel.h>

// Different log levels
HZ_TRACE("Detailed debug info");     // Most verbose
HZ_INFO("Application started");      // General info
HZ_WARN("Resource not found");       // Warnings
HZ_ERROR("Failed to load asset");    // Errors
HZ_FATAL("Critical system failure"); // Fatal errors
```

#### Accessing Log History
```cpp
// Get all logged messages
auto& messages = Hazel::Log::GetMessages();

// Iterate through messages
for (const auto& msg : messages)
{
    std::cout << msg.timestamp << " - " << msg.message << std::endl;
}

// Clear log history
Hazel::Log::ClearMessages();
```

### 3. Layer System

Layers help organize your game logic. Create custom layers by inheriting from `Layer`:

```cpp
class MyGameLayer : public Hazel::Layer
{
public:
    MyGameLayer() : Layer("MyGame") {}

    virtual void OnAttach() override
    {
        // Initialize resources when layer is added
        HZ_INFO("MyGameLayer attached");
    }

    virtual void OnDetach() override
    {
        // Cleanup when layer is removed
        HZ_INFO("MyGameLayer detached");
    }

    virtual void OnUpdate(float deltaTime) override
    {
        // Update game logic every frame
        // deltaTime = time since last frame in seconds
    }

    virtual void OnImGuiRender() override
    {
        // Render ImGui windows for this layer
        // ImGui::Begin("My Window");
        // ImGui::Text("Hello from MyGameLayer!");
        // ImGui::End();
    }

    virtual void OnEvent(Hazel::Event& event) override
    {
        // Handle input events, window events, etc.
    }
};

// Add to your application
class MyApp : public Hazel::Application
{
public:
    MyApp()
    {
        PushLayer(new MyGameLayer());
    }
};
```

### 4. Event System

The event system allows different parts of the engine to communicate:

```cpp
// Events are passed to layers through OnEvent()
void OnEvent(Hazel::Event& event) override
{
    // Dispatch events to handler functions
    Hazel::EventDispatcher dispatcher(event);
    
    // Example: Handle window close event
    // dispatcher.Dispatch<WindowCloseEvent>(
    //     [this](WindowCloseEvent& e) { 
    //         return OnWindowClose(e); 
    //     }
    // );
}
```

### 5. Dockable Windows (ImGui)

The `ImGuiLayer` provides the foundation for dockable editor windows:

```cpp
class EditorLayer : public Hazel::Layer
{
public:
    virtual void OnImGuiRender() override
    {
        // Create dockable windows
        
        // Scene Hierarchy
        ImGui::Begin("Scene Hierarchy");
        // Display scene objects here
        ImGui::End();

        // Inspector
        ImGui::Begin("Inspector");
        // Display selected object properties
        ImGui::End();

        // Console (showing logs)
        ImGui::Begin("Console");
        auto& messages = Hazel::Log::GetMessages();
        for (const auto& msg : messages)
        {
            ImGui::TextUnformatted(msg.message.c_str());
        }
        ImGui::End();

        // Game View
        ImGui::Begin("Game View");
        // Render game viewport
        ImGui::End();
    }
};
```

### 6. C# Scripting

#### Setting Up C# Scripts

1. **Create a C# class in the Scripts folder:**

```csharp
using System;

namespace Hazel
{
    public class EnemyAI
    {
        public float Speed = 3.0f;
        public float DetectionRange = 10.0f;
        
        public void OnCreate()
        {
            Console.WriteLine("Enemy AI initialized");
        }
        
        public void OnUpdate(float deltaTime)
        {
            // Update enemy behavior
        }
        
        public void OnDestroy()
        {
            Console.WriteLine("Enemy AI destroyed");
        }
    }
}
```

2. **Use the script in C++:**

```cpp
// Create a script component
Hazel::ScriptComponent enemyScript("EnemyAI");

// Initialize the script
enemyScript.OnCreate();

// In your game loop
void OnUpdate(float deltaTime)
{
    // Update the script
    enemyScript.OnUpdate(deltaTime);
}

// Cleanup
enemyScript.OnDestroy();
```

#### Script Component System

Scripts can be attached to game entities:

```cpp
class GameObject
{
private:
    Hazel::ScriptComponent m_Script;

public:
    GameObject(const std::string& scriptClass)
        : m_Script(scriptClass)
    {
        m_Script.OnCreate();
    }

    void Update(float deltaTime)
    {
        m_Script.OnUpdate(deltaTime);
    }

    ~GameObject()
    {
        m_Script.OnDestroy();
    }
};

// Usage
GameObject enemy("EnemyAI");
```

#### Accessing Script Fields (Future Feature)

```cpp
// Get field value from script
float speed = enemyScript.GetInstance()->GetFieldValue<float>("Speed");

// Set field value in script
enemyScript.GetInstance()->SetFieldValue<float>("Speed", 5.0f);
```

## Building Your First Hazel Application

### Step 1: Create Your Application Class

```cpp
// MyGame.cpp
#include <Hazel.h>

class MyGame : public Hazel::Application
{
public:
    MyGame()
    {
        HZ_INFO("MyGame Created");
        
        // Add ImGui layer for editor windows
        PushOverlay(new Hazel::ImGuiLayer());
        
        // Add your game layers
        PushLayer(new GameLayer());
    }

    ~MyGame()
    {
        HZ_INFO("MyGame Destroyed");
    }
};

// Entry point
Hazel::Application* Hazel::CreateApplication()
{
    return new MyGame();
}
```

### Step 2: Create Your Game Layer

```cpp
class GameLayer : public Hazel::Layer
{
private:
    Hazel::ScriptComponent m_PlayerScript;

public:
    GameLayer()
        : Layer("Game")
        , m_PlayerScript("PlayerController")
    {
    }

    virtual void OnAttach() override
    {
        HZ_INFO("Game Layer Attached");
        m_PlayerScript.OnCreate();
    }

    virtual void OnUpdate(float deltaTime) override
    {
        // Update game logic
        m_PlayerScript.OnUpdate(deltaTime);
    }

    virtual void OnImGuiRender() override
    {
        // Render debug UI
        ImGui::Begin("Game Stats");
        ImGui::Text("FPS: 60");
        ImGui::End();
    }

    virtual void OnDetach() override
    {
        m_PlayerScript.OnDestroy();
    }
};
```

## Next Steps

This implementation provides the foundation for a 3D game engine. Future enhancements will include:

1. **3D Rendering**
   - OpenGL/DirectX integration
   - Mesh rendering
   - Materials and shaders
   - Lighting system

2. **Scene System**
   - Entity-Component-System (ECS)
   - Scene serialization
   - Prefab system

3. **Physics**
   - 3D physics simulation
   - Collision detection
   - Rigidbody components

4. **Audio**
   - 3D spatial audio
   - Sound effects
   - Music playback

5. **Complete Editor**
   - Scene editor with gizmos
   - Asset browser
   - Material editor
   - Play/pause/stop controls

## Resources

- **Hazel Repository**: Check the source code for implementation details
- **ImGui Documentation**: https://github.com/ocornut/imgui
- **Mono Documentation**: https://www.mono-project.com/docs/

Happy game development with Hazel! 🎮
