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
		
		HZ_INFO("Editor initialized with panels:");
		HZ_INFO("  - Scene Hierarchy (left): Tree view of scene objects");
		HZ_INFO("  - Inspector (right): Properties of selected object");
		HZ_INFO("  - Scene View (center): 3D scene editor with gizmos");
		HZ_INFO("  - Game View (center tab): Runtime game preview");
		HZ_INFO("  - Console (bottom): Filtered log output");
		HZ_INFO("  - Project Browser (bottom tab): Asset management");
		HZ_INFO("  - Toolbar (top): Play/Pause/Stop controls");
		HZ_INFO("");
		HZ_INFO("Unity-like Features:");
		HZ_INFO("  [✓] Dockable windows with ImGui");
		HZ_INFO("  [✓] Scene hierarchy with tree view");
		HZ_INFO("  [✓] Inspector for object properties");
		HZ_INFO("  [✓] Console with log filtering (Trace/Info/Warn/Error/Fatal)");
		HZ_INFO("  [✓] Project browser for assets");
		HZ_INFO("  [✓] Play/Pause/Stop toolbar");
		HZ_INFO("  [✓] Multiple viewport support (Scene/Game)");
		HZ_INFO("");
		HZ_INFO("Layout similar to Unity:");
		HZ_INFO("  ┌─────────────────────────────────────────┐");
		HZ_INFO("  │ File Edit Assets GameObject... [▷][||]  │ Menu & Toolbar");
		HZ_INFO("  ├──────────┬──────────────┬───────────────┤");
		HZ_INFO("  │ Hierarchy│  Scene/Game  │   Inspector   │");
		HZ_INFO("  │          │   Viewport   │               │");
		HZ_INFO("  │  Camera  │              │  Transform    │");
		HZ_INFO("  │  Light   │              │  Position     │");
		HZ_INFO("  │  Player  │              │  Rotation     │");
		HZ_INFO("  │          │              │  Scale        │");
		HZ_INFO("  ├──────────┴──────────────┴───────────────┤");
		HZ_INFO("  │ Console / Project Browser                │");
		HZ_INFO("  │ [Clear] [✓Trace][✓Info][✓Warn][✓Error] │");
		HZ_INFO("  │ [22:30:45.123] [INFO] Log messages...   │");
		HZ_INFO("  └──────────────────────────────────────────┘");
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
