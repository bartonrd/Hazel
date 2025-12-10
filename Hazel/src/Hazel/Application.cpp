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

		while (m_Running)
		{
			// Calculate delta time
			float time = 0.0f; // Would get from platform timer
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

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

			// Simple exit for now - would be event-driven in full implementation
			// For demonstration, we'll break after one iteration
			break;
		}

		HZ_INFO("Application::Run() - Main loop ended");
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
