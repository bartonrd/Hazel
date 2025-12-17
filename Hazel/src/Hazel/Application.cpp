#include "Application.h"
#include "Log.h"
#include "Scripting/ScriptEngine.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Hazel
{
	Application* Application::s_Instance = nullptr;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::string msg = "GLFW Error (" + std::to_string(error) + "): " + std::string(description);
		HZ_ERROR(msg);
	}

	Application::Application()
	{
		s_Instance = this;

		// Initialize logging system
		Log::Init();
		HZ_INFO("Hazel Engine Starting...");

		// Initialize GLFW
		if (!glfwInit())
		{
			HZ_FATAL("Failed to initialize GLFW!");
			return;
		}

		glfwSetErrorCallback(GLFWErrorCallback);

		// Set OpenGL version hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create window
		m_Window = glfwCreateWindow(1920, 1080, "Hazel Editor", nullptr, nullptr);
		if (!m_Window)
		{
			HZ_FATAL("Failed to create GLFW window!");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1); // Enable vsync

		HZ_INFO("GLFW Window created successfully");

		// Initialize renderer (must be done after OpenGL context is created)
		Renderer::Init();

		// Initialize scripting engine
		ScriptEngine::Init();
	}

	Application::~Application()
	{
		// Shutdown renderer
		Renderer::Shutdown();

		// Shutdown scripting engine
		ScriptEngine::Shutdown();

		// Detach all layers before shutting down GLFW
		// This ensures ImGui can properly cleanup platform windows
		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
		}

		// CRITICAL: Delete all layers before destroying OpenGL context
		// This ensures OpenGL resources are freed while context is still valid
		for (Layer* layer : m_LayerStack)
		{
			delete layer;
		}
		m_LayerStack.Clear();

		// Cleanup GLFW
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
		}
		glfwTerminate();

		// Shutdown logging
		Log::Shutdown();
	}

	void Application::Run()
	{
		HZ_INFO("Application::Run() - Main loop started");

		// Find ImGuiLayer
		ImGuiLayer* imguiLayer = nullptr;
		for (Layer* layer : m_LayerStack)
		{
			imguiLayer = dynamic_cast<ImGuiLayer*>(layer);
			if (imguiLayer)
				break;
		}

		while (m_Running && !glfwWindowShouldClose(m_Window))
		{
			// Calculate delta time
			float time = (float)glfwGetTime();
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Poll events
			glfwPollEvents();

			// Clear the screen
			Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::Clear();

			// Update all layers
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			// Start ImGui frame
			if (imguiLayer)
				imguiLayer->Begin();

			// Render ImGui for all layers
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			// End ImGui frame
			if (imguiLayer)
				imguiLayer->End();

			// Swap buffers
			glfwSwapBuffers(m_Window);
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
