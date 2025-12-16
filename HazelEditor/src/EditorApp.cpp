#include <Hazel.h>
#include "EditorLayer.h"

class HazelEditorApp : public Hazel::Application
{
public:
	HazelEditorApp()
	{
		HZ_INFO("Hazel Editor Application Created");
		HZ_INFO("Initializing Unity-like Editor Interface...");
		
		// Add ImGui layer for dockable windows
		PushOverlay(new Hazel::ImGuiLayer());
		
		// Add editor layer with all Unity-like panels
		PushLayer(new HazelEditor::EditorLayer());
		
		HZ_INFO("Editor initialized with 7 panels:");
		HZ_INFO("  - Scene Hierarchy (left): Tree view of scene objects");
		HZ_INFO("  - Inspector (right): Properties of selected object");
		HZ_INFO("  - Scene View (center): 3D scene editor with gizmos");
		HZ_INFO("  - Game View (center tab): Runtime game preview");
		HZ_INFO("  - Console (bottom): Filtered log output");
		HZ_INFO("  - Project Browser (bottom tab): Asset management");
		HZ_INFO("  - Toolbar (top): Play/Pause/Stop controls");
	}

	~HazelEditorApp()
	{
		HZ_INFO("Hazel Editor Application Destroyed");
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new HazelEditorApp();
}
