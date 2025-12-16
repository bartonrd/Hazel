#include "EditorLayer.h"
#include <imgui.h>

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
		// Setup dockspace
		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// Draw all panels
		DrawMenuBar();
		DrawToolbar();
		DrawSceneHierarchy();
		DrawInspector();
		DrawConsole();
		DrawGameView();
		DrawSceneView();
		DrawAssetBrowser();

		ImGui::End();
	}

	void EditorLayer::DrawMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				if (ImGui::MenuItem("Duplicate", "Ctrl+D")) {}
				if (ImGui::MenuItem("Delete", "Del")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Assets"))
			{
				if (ImGui::MenuItem("Create")) {}
				if (ImGui::MenuItem("Import New Asset...")) {}
				if (ImGui::MenuItem("Refresh", "Ctrl+R")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::MenuItem("Create Empty", "Ctrl+Shift+N")) {}
				if (ImGui::BeginMenu("3D Object"))
				{
					if (ImGui::MenuItem("Cube")) {}
					if (ImGui::MenuItem("Sphere")) {}
					if (ImGui::MenuItem("Capsule")) {}
					if (ImGui::MenuItem("Cylinder")) {}
					if (ImGui::MenuItem("Plane")) {}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Camera")) {}
				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Directional Light")) {}
					if (ImGui::MenuItem("Point Light")) {}
					if (ImGui::MenuItem("Spot Light")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Component"))
			{
				if (ImGui::MenuItem("Add...")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Scene", nullptr, true)) {}
				if (ImGui::MenuItem("Game", nullptr, true)) {}
				if (ImGui::MenuItem("Inspector", nullptr, true)) {}
				if (ImGui::MenuItem("Hierarchy", nullptr, true)) {}
				if (ImGui::MenuItem("Console", nullptr, true)) {}
				if (ImGui::MenuItem("Project", nullptr, true)) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About Hazel Editor")) {}
				if (ImGui::MenuItem("Documentation")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::DrawToolbar()
	{
		ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		
		ImGui::SameLine((ImGui::GetWindowWidth() - 150) * 0.5f);
		
		if (ImGui::Button(m_IsPlaying ? "Stop" : "Play", ImVec2(50, 0)))
		{
			m_IsPlaying = !m_IsPlaying;
			if (!m_IsPlaying)
				m_IsPaused = false;
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button(m_IsPaused ? "Resume" : "Pause", ImVec2(50, 0)))
		{
			if (m_IsPlaying)
				m_IsPaused = !m_IsPaused;
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Step", ImVec2(50, 0)))
		{
			// Step one frame
		}
		
		ImGui::End();
	}

	void EditorLayer::DrawSceneHierarchy()
	{
		ImGui::Begin("Hierarchy");
		
		if (ImGui::Button("Create Empty"))
		{
			m_Entities.emplace_back("GameObject", (int)m_Entities.size() + 1);
		}
		
		ImGui::Separator();
		
		for (auto& entity : m_Entities)
		{
			DrawEntityNode(&entity);
		}
		
		ImGui::End();
	}

	void EditorLayer::DrawEntityNode(Entity* entity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (entity->IsSelected)
			flags |= ImGuiTreeNodeFlags_Selected;
		if (entity->Children.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->ID, flags, "%s", entity->Name.c_str());

		if (ImGui::IsItemClicked())
		{
			ClearSelection();
			entity->IsSelected = true;
			m_SelectedEntity = entity;
		}

		if (opened)
		{
			for (auto* child : entity->Children)
			{
				DrawEntityNode(child);
			}
			ImGui::TreePop();
		}
	}

	void EditorLayer::DrawInspector()
	{
		ImGui::Begin("Inspector");
		
		if (m_SelectedEntity)
		{
			ImGui::Text("GameObject: %s", m_SelectedEntity->Name.c_str());
			ImGui::Separator();
			
			// Transform component
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				static float position[3] = { 0.0f, 0.0f, 0.0f };
				static float rotation[3] = { 0.0f, 0.0f, 0.0f };
				static float scale[3] = { 1.0f, 1.0f, 1.0f };
				
				ImGui::DragFloat3("Position", position, 0.1f);
				ImGui::DragFloat3("Rotation", rotation, 0.1f);
				ImGui::DragFloat3("Scale", scale, 0.1f);
			}
			
			// Additional components
			if (ImGui::CollapsingHeader("Mesh Renderer"))
			{
				ImGui::Text("Material: Default");
			}
			
			if (ImGui::CollapsingHeader("Script Component"))
			{
				ImGui::Text("Script: PlayerController.cs");
				static float speed = 5.0f;
				static float jumpForce = 10.0f;
				ImGui::DragFloat("Speed", &speed, 0.1f);
				ImGui::DragFloat("Jump Force", &jumpForce, 0.1f);
			}
			
			ImGui::Separator();
			if (ImGui::Button("Add Component"))
			{
				// Open component selection dialog
			}
		}
		else
		{
			ImGui::TextDisabled("No GameObject selected");
		}
		
		ImGui::End();
	}

	void EditorLayer::DrawConsole()
	{
		ImGui::Begin("Console");
		
		// Filter buttons
		ImGui::Checkbox("Trace", &m_ShowTrace); ImGui::SameLine();
		ImGui::Checkbox("Info", &m_ShowInfo); ImGui::SameLine();
		ImGui::Checkbox("Warn", &m_ShowWarn); ImGui::SameLine();
		ImGui::Checkbox("Error", &m_ShowError); ImGui::SameLine();
		ImGui::Checkbox("Fatal", &m_ShowFatal); ImGui::SameLine();
		
		if (ImGui::Button("Clear"))
		{
			// Clear console
		}
		
		ImGui::SameLine();
		ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
		
		ImGui::Separator();
		
		// Console output area
		ImGui::BeginChild("ConsoleOutput");
		ImGui::TextWrapped("Console output will appear here...");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[TRACE] Detailed debug information");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "[INFO] General informational messages");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[WARN] Warning messages");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[ERROR] Error messages");
		ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "[FATAL] Critical errors");
		
		if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
			
		ImGui::EndChild();
		
		ImGui::End();
	}

	void EditorLayer::DrawGameView()
	{
		ImGui::Begin("Game");
		
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Text("Game Viewport: %.0fx%.0f", viewportSize.x, viewportSize.y);
		
		// Render game viewport here
		ImGui::Image(0, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		
		ImGui::End();
	}

	void EditorLayer::DrawSceneView()
	{
		ImGui::Begin("Scene");
		
		// Tool buttons
		if (ImGui::Button("Q")) {} ImGui::SameLine();
		if (ImGui::Button("W")) {} ImGui::SameLine();
		if (ImGui::Button("E")) {} ImGui::SameLine();
		if (ImGui::Button("R")) {} ImGui::SameLine();
		ImGui::Text("(Hand/Move/Rotate/Scale)");
		
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Text("Scene Viewport: %.0fx%.0f", viewportSize.x, viewportSize.y);
		
		// Render scene viewport here
		ImGui::Image(0, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		
		ImGui::End();
	}

	void EditorLayer::DrawAssetBrowser()
	{
		ImGui::Begin("Project");
		
		ImGui::Text("Current Directory: %s", m_CurrentDirectory.c_str());
		ImGui::Separator();
		
		// Left side - folder tree
		ImGui::BeginChild("Folders", ImVec2(200, 0), true);
		if (ImGui::TreeNode("Assets"))
		{
			if (ImGui::TreeNode("Scripts")) { ImGui::TreePop(); }
			if (ImGui::TreeNode("Materials")) { ImGui::TreePop(); }
			if (ImGui::TreeNode("Scenes")) { ImGui::TreePop(); }
			if (ImGui::TreeNode("Textures")) { ImGui::TreePop(); }
			if (ImGui::TreeNode("Models")) { ImGui::TreePop(); }
			ImGui::TreePop();
		}
		ImGui::EndChild();
		
		ImGui::SameLine();
		
		// Right side - asset grid
		ImGui::BeginChild("Assets");
		ImGui::TextWrapped("Asset files will appear here...");
		ImGui::EndChild();
		
		ImGui::End();
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
