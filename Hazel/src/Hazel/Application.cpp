#include "Application.h"
#include "Log.h"
#include "Scripting/ScriptEngine.h"
#include <iostream>

namespace Hazel
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		// Initialize logging system
		Log::Init();
		HZ_INFO("Hazel Engine Starting...");

		// Initialize scripting engine
		ScriptEngine::Init();
	}

	Application::~Application()
	{
		// Shutdown scripting engine
		ScriptEngine::Shutdown();

		// Shutdown logging
		Log::Shutdown();
	}

	void Application::Run()
	{
		HZ_INFO("Application::Run() - Main loop started");
		HZ_INFO("Note: This is a console-based simulation showing editor structure");
		HZ_INFO("For full GUI rendering, integrate ImGui library with a rendering backend");
		HZ_INFO("");

		int frameCount = 0;
		const int maxFrames = 5; // Run a few frames to show the editor is active

		while (m_Running && frameCount < maxFrames)
		{
			// Calculate delta time (simulated)
			float deltaTime = 0.016f; // ~60 FPS
			m_LastFrameTime += deltaTime;

			// Frame separator for clarity
			if (frameCount > 0)
			{
				HZ_INFO("========================================");
				HZ_INFO("Frame " + std::to_string(frameCount + 1) + " - Editor Update");
				HZ_INFO("========================================");
			}

			// Update all layers
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			// Render ImGui for all layers
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			frameCount++;
			
			// In a real implementation, this would be:
			// - Poll events from window system
			// - Swap buffers
			// - Sleep to maintain target framerate
		}

		HZ_INFO("");
		HZ_INFO("Application::Run() - Main loop ended after " + std::to_string(frameCount) + " frames");
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}
