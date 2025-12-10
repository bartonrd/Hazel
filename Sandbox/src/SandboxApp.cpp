#include <Hazel.h>

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
		
		// Add game layer
		PushLayer(new GameLayer());
		
		HZ_INFO("All layers initialized");
		HZ_INFO("Features available:");
		HZ_INFO("  1. Logging System - Multiple log levels (Trace, Info, Warn, Error, Fatal)");
		HZ_INFO("  2. Dockable Windows - ImGui integration with docking support");
		HZ_INFO("  3. C# Scripting - Mono runtime integration for hot-reloadable scripts");
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