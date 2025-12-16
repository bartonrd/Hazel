#include "EditorLayer.h"

// Note: This is a demonstration showing the UI structure
// ImGui would need to be properly integrated for actual rendering

namespace HazelEditor {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_INFO("EditorLayer::OnAttach - Unity-like editor interface initialized");

		// Create sample scene hierarchy
		m_Entities.emplace_back("Camera", 1);
		m_Entities.emplace_back("Directional Light", 2);
		m_Entities.emplace_back("Player", 3);
		m_Entities.emplace_back("Ground", 4);
		m_Entities.emplace_back("Environment", 5);

		// Add some children to demonstrate hierarchy
		m_Entities[4].Children.push_back(&m_Entities[3]); // Player under Environment
	}

	void EditorLayer::OnDetach()
	{
		HZ_INFO("EditorLayer::OnDetach");
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		// Update editor logic
		if (m_IsPlaying && !m_IsPaused)
		{
			// Update game logic when playing
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		// Console-based visualization of the Unity-like editor interface
		// In a full ImGui implementation, this would render actual GUI windows
		
		static int renderCount = 0;
		renderCount++;
		
		if (renderCount == 1)
		{
			// First frame - show the complete layout
			HZ_INFO("");
			HZ_INFO("╔══════════════════════════════════════════════════════════════╗");
			HZ_INFO("║              HAZEL EDITOR - CONSOLE SIMULATION               ║");
			HZ_INFO("╚══════════════════════════════════════════════════════════════╝");
			HZ_INFO("");
			HZ_INFO("Rendering Unity-like Editor Layout:");
			HZ_INFO("");
			HZ_INFO("┌────────────────────────────────────────────────────────────┐");
			HZ_INFO("│ File  Edit  Assets  GameObject  Component  Window   [▷][||]│ ← Menu & Toolbar");
			HZ_INFO("├───────────┬────────────────────────┬───────────────────────┤");
			HZ_INFO("│ HIERARCHY │    SCENE / GAME        │     INSPECTOR         │");
			HZ_INFO("│           │                        │                       │");
			HZ_INFO("│ ▼ Camera  │   [3D Viewport]        │  Selected: None       │");
			HZ_INFO("│ ▼ Light   │                        │                       │");
			HZ_INFO("│ ▼ Player  │   Scene Tools:         │  Transform            │");
			HZ_INFO("│ ▼ Ground  │   Q W E R              │  Position: 0,0,0      │");
			HZ_INFO("│ ▼ Environ │                        │  Rotation: 0,0,0      │");
			HZ_INFO("│           │                        │  Scale: 1,1,1         │");
			HZ_INFO("├───────────┴────────────────────────┴───────────────────────┤");
			HZ_INFO("│ CONSOLE                                                    │");
			HZ_INFO("│ [Clear] [✓]Trace [✓]Info [✓]Warn [✓]Error                │");
			HZ_INFO("│ Displaying filtered log messages...                        │");
			HZ_INFO("└────────────────────────────────────────────────────────────┘");
			HZ_INFO("");
		}
		
		// Show active panels being rendered
		DrawMenuBar();
		DrawToolbar();
		DrawSceneHierarchy();
		DrawInspector();
		DrawConsole();
		DrawGameView();
		DrawSceneView();
		DrawAssetBrowser();
		
		if (renderCount > 1)
		{
			// Subsequent frames - show update activity
			HZ_INFO("Editor panels active - Frame {0}", renderCount);
		}
	}

	void EditorLayer::DrawMenuBar()
	{
		// Menu bar rendering
		// In full ImGui: ImGui::BeginMenuBar() with File, Edit, Assets, etc.
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Menu Bar: File | Edit | Assets | GameObject | Component | Window | Help");
		}
	}

	void EditorLayer::DrawToolbar()
	{
		// Toolbar with Play/Pause/Stop buttons
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Toolbar: Play [{0}] | Pause [{1}] | Step", m_IsPlaying ? "■" : "▶", m_IsPaused ? "■" : "||");
		}
	}

	void EditorLayer::DrawSceneHierarchy()
	{
		// Scene Hierarchy panel
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Hierarchy Panel: {0} entities - Camera, Light, Player, Ground, Environment", m_Entities.size());
		}
	}

	void EditorLayer::DrawEntityNode(Entity* entity)
	{
		// Draw a tree node for an entity
		// ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		// if (entity->IsSelected)
		//     flags |= ImGuiTreeNodeFlags_Selected;
		// if (entity->Children.empty())
		//     flags |= ImGuiTreeNodeFlags_Leaf;
		// 
		// bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->ID, flags, entity->Name.c_str());
		// 
		// if (ImGui::IsItemClicked())
		// {
		//     ClearSelection();
		//     entity->IsSelected = true;
		//     m_SelectedEntity = entity;
		//     HZ_INFO("Selected: {0}", entity->Name);
		// }
		// 
		// if (opened)
		// {
		//     for (auto* child : entity->Children)
		//     {
		//         DrawEntityNode(child);
		//     }
		//     ImGui::TreePop();
		// }
	}

	void EditorLayer::DrawInspector()
	{
		// Inspector panel
		static int callCount = 0;
		if (callCount++ == 0)
		{
			const char* selected = m_SelectedEntity ? m_SelectedEntity->Name.c_str() : "None";
			HZ_TRACE("Inspector Panel: Selected object = {0}", selected);
		}
	}

	void EditorLayer::DrawConsole()
	{
		// Console panel
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Console Panel: Filters [Trace:{0}] [Info:{1}] [Warn:{2}] [Error:{3}] [Fatal:{4}]",
				m_ShowTrace, m_ShowInfo, m_ShowWarn, m_ShowError, m_ShowFatal);
		}
	}

	void EditorLayer::DrawGameView()
	{
		// Game View panel
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Game View: Rendering runtime viewport");
		}
	}

	void EditorLayer::DrawSceneView()
	{
		// Scene View panel
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Scene View: 3D editor viewport with tools [Q:Hand W:Move E:Rotate R:Scale]");
		}
	}

	void EditorLayer::DrawAssetBrowser()
	{
		// Asset Browser panel
		static int callCount = 0;
		if (callCount++ == 0)
		{
			HZ_TRACE("Asset Browser: Current directory = {0}", m_CurrentDirectory);
		}
	}

	void EditorLayer::ClearSelection()
	{
		for (auto& entity : m_Entities)
		{
			entity.IsSelected = false;
		}
		m_SelectedEntity = nullptr;
	}

}
