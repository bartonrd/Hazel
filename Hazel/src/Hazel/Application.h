#pragma once
#include "Core.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include <memory>

struct GLFWwindow;

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		GLFWwindow* GetWindow() const { return m_Window; }
		
		// Helper function to set cursor position (wraps GLFW call)
		void SetCursorPosition(double x, double y);

	private:
		LayerStack m_LayerStack;
		bool m_Running = true;
		float m_LastFrameTime = 0.0f;
		GLFWwindow* m_Window = nullptr;

		static Application* s_Instance;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}


