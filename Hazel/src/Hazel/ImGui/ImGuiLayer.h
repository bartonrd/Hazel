#pragma once

#include "../Layer.h"

// Forward declare ImGui context
struct ImGuiContext;

namespace Hazel {

	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		// Get the ImGui context for sharing across DLL boundary
		static ImGuiContext* GetContext();

	private:
		float m_Time = 0.0f;
	};

}
