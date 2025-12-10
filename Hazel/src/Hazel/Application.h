#pragma once
#include "Core.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include <memory>

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

	private:
		LayerStack m_LayerStack;
		bool m_Running = true;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}


