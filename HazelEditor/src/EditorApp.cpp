#include <Hazel.h>
#include "EditorLayer.h"

class HazelEditorApp : public Hazel::Application
{
public:
	HazelEditorApp()
	{
		HZ_INFO("╔══════════════════════════════════════════════════════════════╗");
		HZ_INFO("║          HAZEL EDITOR - Console Simulation Mode              ║");
		HZ_INFO("╚══════════════════════════════════════════════════════════════╝");
		HZ_INFO("");
		HZ_INFO("Hazel Editor Application Created");
		HZ_INFO("Initializing Unity-like Editor Interface...");
		HZ_INFO("");
		HZ_INFO("NOTE: This is a console-based simulation of the editor.");
		HZ_INFO("For full GUI rendering with windows, integrate:");
		HZ_INFO("  1. ImGui library (github.com/ocornut/imgui)");
		HZ_INFO("  2. Rendering backend (OpenGL/DirectX)");
		HZ_INFO("  3. Window system (GLFW/Win32)");
		HZ_INFO("");
		
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
		HZ_INFO("");
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
