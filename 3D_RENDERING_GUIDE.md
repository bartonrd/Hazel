# 3D Rendering System Guide

This guide explains how to use Hazel's 3D rendering system to create 3D objects with materials and lighting.

## Table of Contents
1. [Overview](#overview)
2. [Core Components](#core-components)
3. [Quick Start Example](#quick-start-example)
4. [Advanced Usage](#advanced-usage)
5. [API Reference](#api-reference)

## Overview

Hazel's 3D rendering system provides a modern, OpenGL 3.3-based rendering pipeline with support for:
- **Camera System**: Perspective and Orthographic projection
- **Mesh Rendering**: Vertex/Index buffers with flexible vertex layouts
- **Shader Management**: GLSL shader compilation and uniform management
- **Material System**: PBR-ready material properties
- **Lighting**: Directional, Point, and Spot lights with Phong shading

## Core Components

### 1. Renderer
The main rendering interface that manages the rendering pipeline.

```cpp
// Initialize (done automatically by Application)
Renderer::Init();

// In your render loop
Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
Renderer::Clear();

Renderer::BeginScene(camera);
// Submit draw calls here
Renderer::Submit(vertexArray, material, transform);
Renderer::EndScene();

// Shutdown (done automatically by Application)
Renderer::Shutdown();
```

### 2. Camera
Controls the view and projection matrices.

```cpp
// Create a perspective camera
auto camera = std::make_shared<Camera>(Camera::ProjectionType::Perspective);
camera->SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
camera->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

// Or create an orthographic camera
auto orthoCamera = std::make_shared<Camera>(Camera::ProjectionType::Orthographic);
orthoCamera->SetOrthographic(10.0f, 16.0f / 9.0f, -1.0f, 1.0f);
```

### 3. Buffers
Manage vertex data and indices.

```cpp
// Define vertex data
float vertices[] = {
    // positions       // normals
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

// Create buffers
auto vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
vertexBuffer->SetLayout({
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" }
});

auto indexBuffer = std::make_shared<IndexBuffer>(indices, 6);

// Create vertex array
auto vertexArray = std::make_shared<VertexArray>();
vertexArray->AddVertexBuffer(vertexBuffer.get());
vertexArray->SetIndexBuffer(indexBuffer.get());
```

### 4. Shaders
GLSL shaders for rendering.

```cpp
std::string vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec3 a_Normal;

    uniform mat4 u_ViewProjection;
    uniform mat4 u_Transform;

    out vec3 v_Normal;
    out vec3 v_FragPos;

    void main() {
        v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
        v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
        gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    }
)";

std::string fragmentSrc = R"(
    #version 330 core
    layout(location = 0) out vec4 color;

    in vec3 v_Normal;
    in vec3 v_FragPos;

    uniform vec4 u_Material.color;

    void main() {
        color = u_Material.color;
    }
)";

auto shader = std::make_shared<Shader>(vertexSrc, fragmentSrc);
```

### 5. Materials
Define surface properties.

```cpp
auto material = std::make_shared<Material>(shader);
material->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
material->SetShininess(32.0f);      // Specular power
material->SetMetallic(0.0f);        // Metallic property (0-1)
material->SetRoughness(0.5f);       // Roughness property (0-1)
```

### 6. Lights
Three types of lights are available:

#### Directional Light (Sun-like)
```cpp
auto dirLight = std::make_shared<DirectionalLight>();
dirLight->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
dirLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
dirLight->SetIntensity(1.0f);

Renderer::AddLight(dirLight);
```

#### Point Light (Light bulb)
```cpp
auto pointLight = std::make_shared<PointLight>();
pointLight->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
pointLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
pointLight->SetIntensity(1.0f);
pointLight->SetAttenuation(1.0f, 0.09f, 0.032f); // constant, linear, quadratic

Renderer::AddLight(pointLight);
```

#### Spot Light (Flashlight)
```cpp
auto spotLight = std::make_shared<SpotLight>();
spotLight->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
spotLight->SetDirection(glm::vec3(0.0f, 0.0f, -1.0f));
spotLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
spotLight->SetIntensity(1.0f);
spotLight->SetCutOff(12.5f, 17.5f); // inner, outer angles in degrees

Renderer::AddLight(spotLight);
```

## Quick Start Example

Here's a complete example of rendering a rotating cube:

```cpp
class Render3DLayer : public Hazel::Layer
{
public:
    Render3DLayer() : Layer("Render3D") {}

    virtual void OnAttach() override
    {
        // Create camera
        m_Camera = std::make_shared<Camera>(Camera::ProjectionType::Perspective);
        m_Camera->SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

        // Create cube geometry (see Sandbox for full vertex data)
        // ... vertex and index data setup ...
        
        // Create shader
        auto shader = std::make_shared<Shader>(vertexSrc, fragmentSrc);
        
        // Create material
        m_Material = std::make_shared<Material>(shader);
        m_Material->SetColor(glm::vec4(0.3f, 0.6f, 0.9f, 1.0f));
        
        // Create light
        m_Light = std::make_shared<DirectionalLight>();
        m_Light->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
        m_Light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    virtual void OnUpdate(float deltaTime) override
    {
        // Rotate cube
        m_Rotation += deltaTime * 50.0f;

        // Setup lights
        Renderer::ClearLights();
        Renderer::AddLight(m_Light);

        // Render
        Renderer::BeginScene(*m_Camera);
        
        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), 
            glm::radians(m_Rotation), glm::vec3(0.5f, 1.0f, 0.0f));
        
        Renderer::Submit(m_VertexArray, m_Material, transform);
        
        Renderer::EndScene();
    }

private:
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<Material> m_Material;
    std::shared_ptr<DirectionalLight> m_Light;
    float m_Rotation = 0.0f;
};
```

## Advanced Usage

### Multiple Objects
You can submit multiple objects in a single frame:

```cpp
Renderer::BeginScene(*m_Camera);

// Draw cube 1
glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
Renderer::Submit(cubeVAO, redMaterial, transform1);

// Draw cube 2
glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
Renderer::Submit(cubeVAO, blueMaterial, transform2);

Renderer::EndScene();
```

### Multiple Lights
You can add up to 4 lights of each type:

```cpp
Renderer::ClearLights();
Renderer::AddLight(sunLight);        // Directional
Renderer::AddLight(lampLight);       // Point
Renderer::AddLight(flashlight);      // Spot
```

### Custom Vertex Layouts
Define any vertex layout you need:

```cpp
vertexBuffer->SetLayout({
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" },
    { ShaderDataType::Float2, "a_TexCoord" },
    { ShaderDataType::Float4, "a_Color" }
});
```

## API Reference

### Renderer
- `static void Init()` - Initialize the renderer
- `static void Shutdown()` - Shutdown the renderer
- `static void BeginScene(const Camera& camera)` - Begin rendering a scene
- `static void EndScene()` - End rendering a scene
- `static void Submit(vertexArray, material, transform)` - Submit a draw call
- `static void AddLight(light)` - Add a light to the scene
- `static void ClearLights()` - Clear all lights
- `static void SetClearColor(color)` - Set the clear color
- `static void Clear()` - Clear the screen

### Camera
- `SetPerspective(fov, aspect, near, far)` - Set perspective projection
- `SetOrthographic(size, aspect, near, far)` - Set orthographic projection
- `SetPosition(position)` - Set camera position
- `SetRotation(rotation)` - Set camera rotation
- `GetViewProjectionMatrix()` - Get combined view-projection matrix

### Material
- `SetColor(color)` - Set base color (RGBA)
- `SetShininess(value)` - Set specular shininess
- `SetMetallic(value)` - Set metallic property (0-1)
- `SetRoughness(value)` - Set roughness property (0-1)

### Lights
All lights share these methods:
- `SetColor(color)` - Set light color (RGB)
- `SetIntensity(intensity)` - Set light intensity

DirectionalLight:
- `SetDirection(direction)` - Set light direction

PointLight:
- `SetPosition(position)` - Set light position
- `SetAttenuation(constant, linear, quadratic)` - Set attenuation factors

SpotLight:
- `SetPosition(position)` - Set light position
- `SetDirection(direction)` - Set light direction
- `SetCutOff(inner, outer)` - Set cutoff angles in degrees

## Shader Uniforms

When creating custom shaders, these uniforms are automatically set by the renderer:

### Transform Uniforms
- `uniform mat4 u_ViewProjection` - Combined view-projection matrix
- `uniform mat4 u_Transform` - Model transformation matrix

### Material Uniforms
- `uniform vec4 u_Material.color` - Material base color
- `uniform float u_Material.shininess` - Specular shininess
- `uniform float u_Material.metallic` - Metallic property
- `uniform float u_Material.roughness` - Roughness property

### Light Uniforms
```glsl
struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};
uniform DirectionalLight u_DirectionalLights[4];
uniform int u_DirectionalLightCount;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight u_PointLights[4];
uniform int u_PointLightCount;

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float innerCutOff;
    float outerCutOff;
};
uniform SpotLight u_SpotLights[4];
uniform int u_SpotLightCount;
```

## Performance Tips

1. **Batch Similar Objects**: Group objects with the same material together
2. **Minimize State Changes**: Avoid switching shaders/materials frequently
3. **Use Index Buffers**: Always use indexed rendering for better GPU cache utilization
4. **Limit Lights**: Stay within the 4 lights per type limit for best performance
5. **Frustum Culling**: Only submit objects visible to the camera (future enhancement)

## Next Steps

- Add texture support for materials
- Implement framebuffer rendering for viewports
- Add support for more complex geometry (OBJ/FBX loading)
- Implement PBR (Physically Based Rendering)
- Add shadow mapping
- Implement deferred rendering for many lights

## See Also

- [README.md](README.md) - Main project documentation
- [Sandbox/src/SandboxApp.cpp](Sandbox/src/SandboxApp.cpp) - Complete working example
- [ARCHITECTURE.md](ARCHITECTURE.md) - Engine architecture overview
