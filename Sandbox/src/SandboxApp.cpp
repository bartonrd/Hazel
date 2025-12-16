#include <Hazel.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// Example game layer that demonstrates 3D rendering with materials and lighting
class Render3DLayer : public Hazel::Layer
{
public:
	Render3DLayer()
		: Layer("Render3DLayer")
	{
	}

	virtual void OnAttach() override
	{
		HZ_INFO("Render3DLayer::OnAttach - Initializing 3D rendering demo");
		
		// Create camera
		m_Camera = std::make_shared<Hazel::Camera>(Hazel::Camera::ProjectionType::Perspective);
		m_Camera->SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
		m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

		// Create cube vertex data
		float vertices[] = {
			// positions          // normals
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,       // front
			4, 5, 6, 6, 7, 4,       // back
			8, 9, 10, 10, 11, 8,    // left
			12, 13, 14, 14, 15, 12, // right
			16, 17, 18, 18, 19, 16, // bottom
			20, 21, 22, 22, 23, 20  // top
		};

		// Create vertex array
		m_VertexArray = std::make_shared<Hazel::VertexArray>();

		// Create vertex buffer
		m_VertexBuffer = std::make_shared<Hazel::VertexBuffer>(vertices, sizeof(vertices));
		m_VertexBuffer->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float3, "a_Normal" }
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer.get());

		// Create index buffer
		m_IndexBuffer = std::make_shared<Hazel::IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer.get());

		// Create shader
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_FragPos;
			out vec3 v_Normal;

			void main()
			{
				v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
				v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;

			struct Material {
				vec4 color;
				float shininess;
				float metallic;
				float roughness;
			};

			struct DirectionalLight {
				vec3 direction;
				vec3 color;
				float intensity;
			};

			in vec3 v_FragPos;
			in vec3 v_Normal;

			uniform Material u_Material;
			uniform DirectionalLight u_DirectionalLights[4];
			uniform int u_DirectionalLightCount;

			void main()
			{
				vec3 result = vec3(0.0);
				vec3 normal = normalize(v_Normal);
				vec3 viewDir = normalize(-v_FragPos);

				// Ambient
				vec3 ambient = 0.2 * u_Material.color.rgb;
				result += ambient;

				// Directional lights
				for(int i = 0; i < u_DirectionalLightCount; i++)
				{
					vec3 lightDir = normalize(-u_DirectionalLights[i].direction);
					
					// Diffuse
					float diff = max(dot(normal, lightDir), 0.0);
					vec3 diffuse = diff * u_DirectionalLights[i].color * u_DirectionalLights[i].intensity;
					
					// Specular
					vec3 reflectDir = reflect(-lightDir, normal);
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
					vec3 specular = spec * u_DirectionalLights[i].color * u_DirectionalLights[i].intensity;
					
					result += (diffuse + specular) * u_Material.color.rgb;
				}

				color = vec4(result, u_Material.color.a);
			}
		)";

		m_Shader = std::make_shared<Hazel::Shader>(vertexSrc, fragmentSrc);

		// Create material
		m_Material = std::make_shared<Hazel::Material>(m_Shader);
		m_Material->SetColor(glm::vec4(0.3f, 0.6f, 0.9f, 1.0f)); // Blue cube
		m_Material->SetShininess(32.0f);

		// Create directional light
		m_Light = std::make_shared<Hazel::DirectionalLight>();
		m_Light->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
		m_Light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		m_Light->SetIntensity(1.0f);

		HZ_INFO("3D rendering demo initialized successfully!");
		HZ_INFO("- Camera: Perspective, FOV=45°, Position=(0,0,5)");
		HZ_INFO("- Object: Cube with normals");
		HZ_INFO("- Material: Blue with Phong shading");
		HZ_INFO("- Lighting: Directional light");
	}

	virtual void OnDetach() override
	{
		HZ_INFO("Render3DLayer::OnDetach");
	}

	virtual void OnUpdate(float deltaTime) override
	{
		// Rotate the cube
		m_Rotation += deltaTime * 50.0f; // 50 degrees per second
		if (m_Rotation > 360.0f)
			m_Rotation -= 360.0f;

		// Clear lights from previous frame
		Hazel::Renderer::ClearLights();
		
		// Add lights
		Hazel::Renderer::AddLight(m_Light);

		// Begin scene
		Hazel::Renderer::BeginScene(*m_Camera);

		// Create transform matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0.5f, 1.0f, 0.0f));

		// Submit draw call
		Hazel::Renderer::Submit(m_VertexArray, m_Material, transform);

		// End scene
		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		// ImGui windows can be added here to control the 3D scene
	}

private:
	std::shared_ptr<Hazel::Camera> m_Camera;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::Material> m_Material;
	std::shared_ptr<Hazel::DirectionalLight> m_Light;
	float m_Rotation = 0.0f;
};

// Example game layer that demonstrates the engine features
class GameLayer : public Hazel::Layer
{
public:
	GameLayer()
		: Layer("GameLayer")
	{
	}

	virtual void OnAttach() override
	{
		HZ_INFO("GameLayer::OnAttach");
		
		// Example: Create a script component for a game object
		m_PlayerScript = Hazel::ScriptComponent("PlayerController");
		m_PlayerScript.OnCreate();
	}

	virtual void OnDetach() override
	{
		HZ_INFO("GameLayer::OnDetach");
		m_PlayerScript.OnDestroy();
	}

	virtual void OnUpdate(float deltaTime) override
	{
		// Update game logic
		m_PlayerScript.OnUpdate(deltaTime);
	}

	virtual void OnImGuiRender() override
	{
		// ImGui dockable windows would be rendered here
		// Example windows:
		// - Scene Hierarchy
		// - Inspector (showing m_PlayerScript properties)
		// - Game View (3D viewport)
	}

private:
	Hazel::ScriptComponent m_PlayerScript;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		HZ_INFO("Sandbox Application Created");
		
		// Add ImGui layer for dockable windows
		PushOverlay(new Hazel::ImGuiLayer());
		
		// Add 3D rendering demo layer
		PushLayer(new Render3DLayer());
		
		// Add game layer
		PushLayer(new GameLayer());
		
		HZ_INFO("All layers initialized");
		HZ_INFO("Features available:");
		HZ_INFO("  1. Logging System - Multiple log levels (Trace, Info, Warn, Error, Fatal)");
		HZ_INFO("  2. Dockable Windows - ImGui integration with docking support");
		HZ_INFO("  3. C# Scripting - Mono runtime integration for hot-reloadable scripts");
		HZ_INFO("  4. 3D Rendering - Rotating cube with materials and lighting");
	}

	~Sandbox()
	{
		HZ_INFO("Sandbox Application Destroyed");
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}