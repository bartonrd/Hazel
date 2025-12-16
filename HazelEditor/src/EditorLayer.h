#pragma once

#include "Hazel/Layer.h"
#include "Hazel/Log.h"
#include "Hazel/Renderer/EditorCamera.h"
#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Material.h"
#include "Hazel/Renderer/Shader.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace HazelEditor {

	// Mesh type enumeration
	enum class MeshType
	{
		None = 0,
		Cube,
		Sphere,
		Capsule
	};

	// Transform component
	struct Transform
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransformMatrix() const;
	};

	// Simple entity for scene hierarchy
	struct Entity
	{
		std::string Name;
		int ID;
		bool IsSelected = false;
		std::vector<Entity*> Children;
		
		// Components
		Transform EntityTransform;
		MeshType Mesh = MeshType::None;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Entity(const std::string& name, int id) : Name(name), ID(id) {}
	};

	class EditorLayer : public Hazel::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnImGuiRender() override;

	private:
		void DrawMenuBar();
		void DrawSceneHierarchy();
		void DrawInspector();
		void DrawConsole();
		void DrawGameView();
		void DrawSceneView();
		void DrawAssetBrowser();
		void DrawToolbar();

		// Helper functions
		void DrawEntityNode(Entity* entity);
		void ClearSelection();
		void CreateEntity(const std::string& name, MeshType meshType);
		void RenderScene();
		
		// Mesh management
		void InitializeMeshBuffers();
		std::shared_ptr<Hazel::VertexArray> GetMeshVertexArray(MeshType type);

	private:
		// Scene data
		std::vector<Entity> m_Entities;
		Entity* m_SelectedEntity = nullptr;

		// Editor state
		bool m_IsPlaying = false;
		bool m_IsPaused = false;

		// Console state
		bool m_ShowTrace = true;
		bool m_ShowInfo = true;
		bool m_ShowWarn = true;
		bool m_ShowError = true;
		bool m_ShowFatal = true;
		bool m_AutoScroll = true;

		// Asset browser state
		std::string m_CurrentDirectory = "Assets";
		
		// Scene rendering
		std::unique_ptr<Hazel::EditorCamera> m_EditorCamera;
		std::unique_ptr<Hazel::Framebuffer> m_SceneFramebuffer;
		std::shared_ptr<Hazel::Shader> m_SceneShader;
		std::shared_ptr<Hazel::Material> m_DefaultMaterial;
		std::shared_ptr<Hazel::DirectionalLight> m_SceneLight;
		
		// Mesh buffers
		std::shared_ptr<Hazel::VertexArray> m_CubeMesh;
		std::shared_ptr<Hazel::VertexBuffer> m_CubeVertexBuffer;
		std::shared_ptr<Hazel::IndexBuffer> m_CubeIndexBuffer;
		
		std::shared_ptr<Hazel::VertexArray> m_SphereMesh;
		std::shared_ptr<Hazel::VertexBuffer> m_SphereVertexBuffer;
		std::shared_ptr<Hazel::IndexBuffer> m_SphereIndexBuffer;
		
		std::shared_ptr<Hazel::VertexArray> m_CapsuleMesh;
		std::shared_ptr<Hazel::VertexBuffer> m_CapsuleVertexBuffer;
		std::shared_ptr<Hazel::IndexBuffer> m_CapsuleIndexBuffer;
		
		// Scene view state
		glm::vec2 m_ViewportSize = { 1280.0f, 720.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_LastMousePos = { 0.0f, 0.0f };
		
		// Camera control
		bool m_CameraRotating = false;
		int m_NextEntityID = 100;
	};

}
