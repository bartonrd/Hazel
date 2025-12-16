# 3D Rendering Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                      Application Layer                          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │ HazelEditor  │  │   Sandbox    │  │  Game App    │          │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘          │
│         │                 │                  │                   │
│         └─────────────────┴──────────────────┘                   │
│                           │                                      │
└───────────────────────────┼──────────────────────────────────────┘
                            │
┌───────────────────────────┼──────────────────────────────────────┐
│                      Layer System                                │
│                           │                                      │
│  ┌────────────────────────▼────────────────────────┐            │
│  │           Your Game/Render Layer                │            │
│  │  (Contains Camera, Lights, Materials, Objects)  │            │
│  └────────────────────────┬────────────────────────┘            │
│                           │                                      │
└───────────────────────────┼──────────────────────────────────────┘
                            │
┌───────────────────────────┼──────────────────────────────────────┐
│                    Renderer API                                  │
│                           │                                      │
│  ┌────────────────────────▼────────────────────────┐            │
│  │  Renderer::BeginScene(camera)                   │            │
│  │  Renderer::AddLight(light)                       │            │
│  │  Renderer::Submit(vertexArray, material, xform) │            │
│  │  Renderer::EndScene()                            │            │
│  └────────────────────────┬────────────────────────┘            │
│                           │                                      │
│         ┌─────────────────┼─────────────────┐                   │
│         │                 │                 │                   │
│  ┌──────▼──────┐  ┌──────▼──────┐  ┌──────▼──────┐            │
│  │   Camera    │  │  Material   │  │    Light    │            │
│  │             │  │             │  │             │            │
│  │ - Position  │  │ - Color     │  │ - Color     │            │
│  │ - Rotation  │  │ - Shininess │  │ - Intensity │            │
│  │ - Proj Type │  │ - Metallic  │  │ - Direction │            │
│  │             │  │ - Roughness │  │   /Position │            │
│  └─────────────┘  └──────┬──────┘  └─────────────┘            │
│                          │                                      │
│                   ┌──────▼──────┐                               │
│                   │   Shader    │                               │
│                   │             │                               │
│                   │ - Compile   │                               │
│                   │ - Uniforms  │                               │
│                   └─────────────┘                               │
└───────────────────────────────────────────────────────────────┘
                            │
┌───────────────────────────┼──────────────────────────────────────┐
│                   Geometry System                                │
│                           │                                      │
│  ┌────────────────────────▼────────────────────────┐            │
│  │           VertexArray (VAO)                      │            │
│  └────────────┬──────────────────┬──────────────────┘            │
│               │                  │                               │
│      ┌────────▼────────┐  ┌─────▼──────┐                        │
│      │  VertexBuffer   │  │IndexBuffer │                        │
│      │     (VBO)       │  │   (IBO)    │                        │
│      │                 │  │            │                        │
│      │ - Vertices      │  │ - Indices  │                        │
│      │ - Normals       │  │            │                        │
│      │ - TexCoords     │  │            │                        │
│      └─────────────────┘  └────────────┘                        │
└───────────────────────────────────────────────────────────────┘
                            │
┌───────────────────────────┼──────────────────────────────────────┐
│                    OpenGL 3.3 Core                               │
│                           │                                      │
│  ┌────────────────────────▼────────────────────────┐            │
│  │  glDrawElements, glBufferData, glUseProgram     │            │
│  │  glUniform*, glVertexAttribPointer, etc.        │            │
│  └─────────────────────────────────────────────────┘            │
└───────────────────────────────────────────────────────────────┘
                            │
                            ▼
                    ┌───────────────┐
                    │  GPU Hardware │
                    └───────────────┘


Data Flow for Rendering a Frame:
═══════════════════════════════════

1. Application Layer
   └─> Creates game/render layer with Camera, Lights, Objects

2. Update Loop
   ├─> Layer::OnUpdate(deltaTime)
   │   ├─> Update object transforms
   │   ├─> Update camera position
   │   └─> Update light properties
   │
   └─> Renderer::BeginScene(camera)
       ├─> Store view-projection matrix
       └─> Clear lights from previous frame

3. Rendering
   ├─> For each object:
   │   ├─> Renderer::Submit(vertexArray, material, transform)
   │   │   ├─> Bind shader
   │   │   ├─> Set uniforms (MVP matrix, material props, lights)
   │   │   ├─> Bind vertex array
   │   │   └─> glDrawElements()
   │   │
   │   └─> Renderer::EndScene()

4. Display
   └─> Swap buffers (handled by Application)


Shader Uniform Flow:
═══════════════════

Camera ----------> u_ViewProjection (mat4)
                   
Transform -------> u_Transform (mat4)
                   
Material --------> u_Material.color (vec4)
                   u_Material.shininess (float)
                   u_Material.metallic (float)
                   u_Material.roughness (float)
                   
Lights ----------> u_DirectionalLights[0-3] (struct array)
                   u_PointLights[0-3] (struct array)
                   u_SpotLights[0-3] (struct array)
                   u_DirectionalLightCount (int)
                   u_PointLightCount (int)
                   u_SpotLightCount (int)


Memory Layout:
═════════════

CPU Side:                          GPU Side:
┌──────────────────┐              ┌──────────────────┐
│ VertexBuffer     │  ──upload──> │ GPU Buffer (VBO) │
│ (float array)    │              │                  │
└──────────────────┘              └──────────────────┘

┌──────────────────┐              ┌──────────────────┐
│ IndexBuffer      │  ──upload──> │ GPU Buffer (IBO) │
│ (uint array)     │              │                  │
└──────────────────┘              └──────────────────┘

┌──────────────────┐              ┌──────────────────┐
│ Shader Source    │  ──compile─> │ GPU Program      │
│ (GLSL string)    │              │                  │
└──────────────────┘              └──────────────────┘

Note: Data is uploaded once at creation, then stays on GPU
      Only transforms and uniforms are updated per frame


Example Scene Graph:
═══════════════════

Scene
├── Camera (position: 0, 0, 5)
├── Lights
│   ├── DirectionalLight (Sun)
│   ├── PointLight (Lamp 1)
│   ├── PointLight (Lamp 2)
│   └── SpotLight (Flashlight)
└── Objects
    ├── Cube 1
    │   ├── VertexArray (geometry)
    │   ├── Material (red, shiny)
    │   └── Transform (position: -2, 0, 0)
    ├── Cube 2
    │   ├── VertexArray (geometry - shared with Cube 1)
    │   ├── Material (blue, rough)
    │   └── Transform (position: 2, 0, 0)
    └── Plane
        ├── VertexArray (geometry)
        ├── Material (white, matte)
        └── Transform (position: 0, -1, 0)
```
