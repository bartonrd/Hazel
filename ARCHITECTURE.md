# Hazel Engine Architecture

## System Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                        HAZEL GAME ENGINE                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │                   Application Layer                       │ │
│  │  • Main Loop                                              │ │
│  │  • Layer Stack Management                                 │ │
│  │  • System Initialization/Shutdown                         │ │
│  └───────────────────────────────────────────────────────────┘ │
│                              │                                  │
│              ┌───────────────┼───────────────┐                 │
│              ▼               ▼               ▼                 │
│  ┌─────────────────┐ ┌─────────────┐ ┌─────────────────┐     │
│  │  Logging System │ │ Event System│ │  Layer System   │     │
│  ├─────────────────┤ ├─────────────┤ ├─────────────────┤     │
│  │ • Trace         │ │ • Window    │ │ • PushLayer     │     │
│  │ • Info          │ │ • Input     │ │ • PushOverlay   │     │
│  │ • Warn          │ │ • App       │ │ • PopLayer      │     │
│  │ • Error         │ │ • Keyboard  │ │ • Update Loop   │     │
│  │ • Fatal         │ │ • Mouse     │ │                 │     │
│  │ • Timestamps    │ │ • Dispatch  │ │                 │     │
│  └─────────────────┘ └─────────────┘ └─────────────────┘     │
│                                                                 │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │                     Layer Stack                           │ │
│  │  ┌────────────────────────────────────────────────────┐  │ │
│  │  │            ImGui Layer (Overlay)                    │  │ │
│  │  │  ┌──────────────┐  ┌──────────────┐               │  │ │
│  │  │  │   Scene      │  │  Inspector   │               │  │ │
│  │  │  │  Hierarchy   │  │  Properties  │  ┌─────────┐  │  │ │
│  │  │  └──────────────┘  └──────────────┘  │ Console │  │  │ │
│  │  │  ┌──────────────┐  ┌──────────────┐  └─────────┘  │  │ │
│  │  │  │  Asset       │  │   Game       │               │  │ │
│  │  │  │  Browser     │  │   View       │               │  │ │
│  │  │  └──────────────┘  └──────────────┘               │  │ │
│  │  └────────────────────────────────────────────────────┘  │ │
│  │  ┌────────────────────────────────────────────────────┐  │ │
│  │  │              Game Layer(s)                          │  │ │
│  │  │  • Scene Management                                 │  │ │
│  │  │  • Entity Updates                                   │  │ │
│  │  │  • Physics Simulation                               │  │ │
│  │  └────────────────────────────────────────────────────┘  │ │
│  └───────────────────────────────────────────────────────────┘ │
│                              │                                  │
│                              ▼                                  │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │                  C# Scripting Engine                      │ │
│  │  ┌─────────────────────────────────────────────────────┐ │ │
│  │  │             Mono Runtime                             │ │ │
│  │  │  • Assembly Loading                                  │ │ │
│  │  │  • Script Instance Management                        │ │ │
│  │  │  │  • Hot Reloading                                  │ │ │
│  │  └─────────────────────────────────────────────────────┘ │ │
│  │  ┌─────────────────────────────────────────────────────┐ │ │
│  │  │         Script Components                            │ │ │
│  │  │  ┌──────────────┐  ┌──────────────┐                │ │ │
│  │  │  │   Player     │  │   Camera     │                │ │ │
│  │  │  │  Controller  │  │  Controller  │  ┌──────────┐  │ │ │
│  │  │  │   (.cs)      │  │   (.cs)      │  │ Enemy AI │  │ │ │
│  │  │  │              │  │              │  │  (.cs)   │  │ │ │
│  │  │  └──────────────┘  └──────────────┘  └──────────┘  │ │ │
│  │  └─────────────────────────────────────────────────────┘ │ │
│  └───────────────────────────────────────────────────────────┘ │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Data Flow

```
User Input → Events → Layers → Game Objects → Scripts → Update → Render
     │                                                               │
     └───────────────── Feedback Loop ─────────────────────────────┘
```

## Layer Update Cycle

```
1. Application::Run()
   │
   ├─→ Calculate DeltaTime
   │
   ├─→ For each Layer:
   │   ├─→ Layer::OnUpdate(deltaTime)
   │   │   └─→ Update game logic
   │   │       └─→ ScriptComponent::OnUpdate(deltaTime)
   │   │           └─→ C# OnUpdate method
   │   │
   │   └─→ Layer::OnImGuiRender()
   │       └─→ Render dockable windows
   │
   └─→ Repeat
```

## Script Lifecycle

```
┌──────────────────────────────────────────────┐
│          C# Script Creation                  │
└──────────────────┬───────────────────────────┘
                   │
                   ▼
┌──────────────────────────────────────────────┐
│    ScriptEngine::CreateScriptInstance()      │
│         • Load Mono assembly                 │
│         • Find class in assembly             │
│         • Create object instance             │
└──────────────────┬───────────────────────────┘
                   │
                   ▼
┌──────────────────────────────────────────────┐
│       ScriptInstance::OnCreate()             │
│         • Initialize script state            │
│         • Setup references                   │
└──────────────────┬───────────────────────────┘
                   │
                   ▼
┌──────────────────────────────────────────────┐
│           Game Loop                          │
│    ScriptInstance::OnUpdate(deltaTime)       │
│         • Called every frame                 │
│         • Contains game logic                │
└──────────────────┬───────────────────────────┘
                   │
                   ▼
┌──────────────────────────────────────────────┐
│      ScriptInstance::OnDestroy()             │
│         • Cleanup resources                  │
│         • Release references                 │
└──────────────────────────────────────────────┘
```

## Logging Flow

```
HZ_INFO("Message")
    │
    ▼
Log::Info(message)
    │
    ▼
Log::LogMessage(LogLevel::Info, message)
    │
    ├─→ Generate timestamp
    │
    ├─→ Store in message history
    │    └─→ Available for Console window
    │
    └─→ Output to console
         └─→ [HH:MM:SS.mmm] [INFO] Message
```

## Event Flow

```
System Event (e.g., Key Press)
    │
    ▼
Event Created (KeyPressedEvent)
    │
    ▼
Application::OnEvent()
    │
    ▼
For each Layer (reverse order):
    │
    ├─→ Layer::OnEvent(event)
    │   │
    │   └─→ EventDispatcher::Dispatch<T>()
    │       │
    │       └─→ Handler function
    │           │
    │           └─→ Return true if handled
    │
    └─→ If handled, stop propagation
```

## Component Hierarchy

```
Application
    │
    ├─→ LayerStack
    │   │
    │   ├─→ Layers
    │   │   └─→ Game Layer
    │   │       └─→ Game Objects
    │   │           └─→ ScriptComponent
    │   │               └─→ ScriptInstance (C#)
    │   │
    │   └─→ Overlays
    │       └─→ ImGuiLayer
    │           └─→ Dockable Windows
    │
    ├─→ Log System
    │
    ├─→ Event System
    │
    └─→ Script Engine
        └─→ Mono Runtime
```

## Key Design Patterns

1. **Singleton Pattern**
   - `Application::Get()` - Global application access
   - `Log` - Static logging interface

2. **Observer Pattern**
   - Event system for decoupled communication

3. **Strategy Pattern**
   - Layer system - different behaviors per layer

4. **Factory Pattern**
   - `ScriptEngine::CreateScriptInstance()`

5. **Component Pattern**
   - `ScriptComponent` attached to game objects

## Thread Safety Notes

- Logging system: Thread-safe message storage
- Event system: Single-threaded dispatch
- Script engine: Main thread only
- ImGui: Main thread rendering only

## Memory Management

- Application owns LayerStack
- LayerStack owns Layers (deletes on destruction)
- ScriptEngine manages Mono runtime lifetime
- ScriptComponent uses shared_ptr for instances
- RAII pattern throughout

This architecture provides a solid foundation for building a complete 3D game engine similar to Unity!
