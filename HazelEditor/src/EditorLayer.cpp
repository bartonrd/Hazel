#include "EditorLayer.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/MeshGenerator.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace HazelEditor {

	glm::mat4 Transform::GetTransformMatrix() const
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position);
		transform = glm::rotate(transform, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, Scale);
		return transform;
	}

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_INFO("EditorLayer::OnAttach - Unity-like editor interface initialized");

		// CRITICAL: Set the ImGui context from Hazel.dll
		// This ensures we use the same ImGui context that was initialized in Hazel's ImGuiLayer
		ImGui::SetCurrentContext(Hazel::ImGuiLayer::GetContext());

		// Initialize scene rendering
		m_EditorCamera = std::make_unique<Hazel::EditorCamera>();
		m_SceneFramebuffer = std::make_unique<Hazel::Framebuffer>(1280, 720);
		
		// Create shader for scene rendering
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_FragPos;
			out vec3 v_Normal;

			void main()
			{
				v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
				v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;

			struct Material {
				vec4 color;
				float shininess;
				float metallic;
				float roughness;
			};

			struct DirectionalLight {
				vec3 direction;
				vec3 color;
				float intensity;
			};

			in vec3 v_FragPos;
			in vec3 v_Normal;

			uniform Material u_Material;
			uniform DirectionalLight u_DirectionalLights[4];
			uniform int u_DirectionalLightCount;

			void main()
			{
				vec3 result = vec3(0.0);
				vec3 normal = normalize(v_Normal);
				vec3 viewDir = normalize(-v_FragPos);

				// Ambient
				vec3 ambient = 0.2 * u_Material.color.rgb;
				result += ambient;

				// Directional lights
				for(int i = 0; i < u_DirectionalLightCount; i++)
				{
					vec3 lightDir = normalize(-u_DirectionalLights[i].direction);
					
					// Diffuse
					float diff = max(dot(normal, lightDir), 0.0);
					vec3 diffuse = diff * u_DirectionalLights[i].color * u_DirectionalLights[i].intensity;
					
					// Specular
					vec3 reflectDir = reflect(-lightDir, normal);
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
					vec3 specular = spec * u_DirectionalLights[i].color * u_DirectionalLights[i].intensity;
					
					result += (diffuse + specular) * u_Material.color.rgb;
				}

				color = vec4(result, u_Material.color.a);
			}
		)";
		
		m_SceneShader = std::make_shared<Hazel::Shader>(vertexSrc, fragmentSrc);
		m_DefaultMaterial = std::make_shared<Hazel::Material>(m_SceneShader);
		
		// Create scene light
		m_SceneLight = std::make_shared<Hazel::DirectionalLight>();
		m_SceneLight->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
		m_SceneLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		m_SceneLight->SetIntensity(1.0f);
		
		// Initialize mesh buffers
		InitializeMeshBuffers();

		// Create sample scene hierarchy
		m_Entities.emplace_back("Main Camera", m_NextEntityID++);
		m_Entities.emplace_back("Directional Light", m_NextEntityID++);
		
		// Add a default cube to demonstrate
		CreateEntity("Cube", MeshType::Cube);
		m_Entities.back().EntityTransform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Entities.back().Color = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
		
		// Focus camera on the default cube so it's visible
		FocusOnEntity(&m_Entities.back());
	}

	void EditorLayer::OnDetach()
	{
		HZ_INFO("EditorLayer::OnDetach");
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		// Update editor camera
		if (m_ViewportFocused)
		{
			m_EditorCamera->OnUpdate(deltaTime);
		}
		
		// Update editor logic
		if (m_IsPlaying && !m_IsPaused)
		{
			// Update game logic when playing
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		// Draw the main menu bar at the top (outside dockspace)
		DrawMenuBar();
		
		// Draw the static toolbar below the menu bar (outside dockspace)
		DrawToolbar();

		// Create a fullscreen dockspace that starts below the toolbar
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistent = true;
		bool opt_fullscreen = opt_fullscreen_persistent;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			
			// Position the dockspace below the menu bar and toolbar
			// WorkPos already accounts for the menu bar, so we just need to add toolbar height
			float toolbarHeight = 40.0f; // Must match the height in DrawToolbar()
			ImVec2 dockspacePos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + toolbarHeight);
			ImVec2 dockspaceSize = ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - toolbarHeight);
			
			ImGui::SetNextWindowPos(dockspacePos);
			ImGui::SetNextWindowSize(dockspaceSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		
		ImGui::End();

		// Draw all the editor panels - they will dock into the dockspace
		DrawSceneHierarchy();
		DrawInspector();
		DrawConsole();
		DrawGameView();
		DrawSceneView();
		DrawAssetBrowser();
	}

	void EditorLayer::DrawMenuBar()
	{
		// Create main menu bar at the top of the screen
		if (ImGui::BeginMainMenuBar())
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
				if (ImGui::MenuItem("Create Empty", "Ctrl+Shift+N")) 
				{
					CreateEntity("GameObject", MeshType::None);
				}
				if (ImGui::BeginMenu("3D Object"))
				{
					if (ImGui::MenuItem("Cube")) 
					{
						CreateEntity("Cube", MeshType::Cube);
					}
					if (ImGui::MenuItem("Sphere")) 
					{
						CreateEntity("Sphere", MeshType::Sphere);
					}
					if (ImGui::MenuItem("Capsule")) 
					{
						CreateEntity("Capsule", MeshType::Capsule);
					}
					// TODO: Implement Cylinder and Plane mesh generation
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
			
			ImGui::EndMainMenuBar();
		}
	}

	void EditorLayer::DrawToolbar()
	{
		// Create a static toolbar window below the main menu bar
		// This toolbar is not dockable and cannot be moved
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		// Calculate position: below the menu bar
		ImVec2 toolbarPos = viewport->WorkPos;
		
		// Calculate size: full width, fixed height
		float toolbarHeight = 40.0f;
		ImVec2 toolbarSize = ImVec2(viewport->WorkSize.x, toolbarHeight);
		
		// Set window properties to make it static and non-dockable
		ImGui::SetNextWindowPos(toolbarPos);
		ImGui::SetNextWindowSize(toolbarSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
		                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
		                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
		
		ImGui::Begin("##Toolbar", nullptr, window_flags);
		
		// Center the buttons
		float buttonWidth = 50.0f;
		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float totalWidth = (buttonWidth * 3) + (spacing * 2);
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);
		ImGui::SetCursorPosY((toolbarHeight - ImGui::GetFrameHeight()) * 0.5f);
		
		if (ImGui::Button(m_IsPlaying ? "Stop" : "Play", ImVec2(buttonWidth, 0)))
		{
			m_IsPlaying = !m_IsPlaying;
			if (!m_IsPlaying)
				m_IsPaused = false;
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button(m_IsPaused ? "Resume" : "Pause", ImVec2(buttonWidth, 0)))
		{
			if (m_IsPlaying)
				m_IsPaused = !m_IsPaused;
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Step", ImVec2(buttonWidth, 0)))
		{
			// Step one frame
		}
		
		ImGui::End();
		ImGui::PopStyleVar(2);
	}

	void EditorLayer::DrawSceneHierarchy()
	{
		ImGui::Begin("Hierarchy");
		
		if (ImGui::Button("Create Empty"))
		{
			CreateEntity("GameObject", MeshType::None);
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

		// Focus camera on entity when double-clicked
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			FocusOnEntity(entity);
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
				ImGui::DragFloat3("Position", glm::value_ptr(m_SelectedEntity->EntityTransform.Position), 0.1f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(m_SelectedEntity->EntityTransform.Rotation), 1.0f);
				ImGui::DragFloat3("Scale", glm::value_ptr(m_SelectedEntity->EntityTransform.Scale), 0.1f);
			}
			
			// Mesh Renderer component (if entity has a mesh)
			if (m_SelectedEntity->Mesh != MeshType::None)
			{
				if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
				{
					const char* meshNames[] = { "None", "Cube", "Sphere", "Capsule" };
					int meshType = (int)m_SelectedEntity->Mesh;
					if (ImGui::Combo("Mesh", &meshType, meshNames, 4))
					{
						m_SelectedEntity->Mesh = (MeshType)meshType;
					}
					
					ImGui::ColorEdit4("Color", glm::value_ptr(m_SelectedEntity->Color));
				}
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
		
		// Render game viewport here (placeholder - will show black until framebuffer is set up)
		// ImGui::Image((void*)(intptr_t)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Dummy(viewportSize); // Placeholder for now
		
		ImGui::End();
	}

	void EditorLayer::DrawSceneView()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");
		
		// Get viewport size
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
		// Resize framebuffer if needed
		if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0 &&
			(m_SceneFramebuffer->GetWidth() != viewportPanelSize.x || 
			 m_SceneFramebuffer->GetHeight() != viewportPanelSize.y))
		{
			m_SceneFramebuffer->Resize((unsigned int)viewportPanelSize.x, (unsigned int)viewportPanelSize.y);
			m_EditorCamera->SetViewportSize(viewportPanelSize.x, viewportPanelSize.y);
		}
		
		// Render scene to framebuffer
		RenderScene();
		
		// Display framebuffer texture
		uint64_t textureID = m_SceneFramebuffer->GetColorAttachment();
		ImGui::Image((void*)(intptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		
		// Check if viewport is focused/hovered
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsItemHovered();
		
		// Handle mouse input for camera rotation
		if (m_ViewportHovered && ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			
			ImVec2 mousePos = ImGui::GetMousePos();
			if (!m_CameraRotating)
			{
				m_LastMousePos = glm::vec2(mousePos.x, mousePos.y);
				m_CameraRotating = true;
			}
			
			glm::vec2 currentPos(mousePos.x, mousePos.y);
			glm::vec2 delta = currentPos - m_LastMousePos;
			m_LastMousePos = currentPos;
			
			m_EditorCamera->ProcessMouseMovement(delta.x, -delta.y);
		}
		else
		{
			m_CameraRotating = false;
		}
		
		// Handle keyboard input for camera movement (WASD + QE)
		if (m_ViewportFocused)
		{
			m_EditorCamera->SetMoveForward(ImGui::IsKeyDown(ImGuiKey_W));
			m_EditorCamera->SetMoveBackward(ImGui::IsKeyDown(ImGuiKey_S));
			m_EditorCamera->SetMoveLeft(ImGui::IsKeyDown(ImGuiKey_A));
			m_EditorCamera->SetMoveRight(ImGui::IsKeyDown(ImGuiKey_D));
			m_EditorCamera->SetMoveUp(ImGui::IsKeyDown(ImGuiKey_E));
			m_EditorCamera->SetMoveDown(ImGui::IsKeyDown(ImGuiKey_Q));
		}
		else
		{
			// Stop all movement when not focused
			m_EditorCamera->SetMoveForward(false);
			m_EditorCamera->SetMoveBackward(false);
			m_EditorCamera->SetMoveLeft(false);
			m_EditorCamera->SetMoveRight(false);
			m_EditorCamera->SetMoveUp(false);
			m_EditorCamera->SetMoveDown(false);
		}
		
		ImGui::End();
		ImGui::PopStyleVar();
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

	void EditorLayer::FocusOnEntity(Entity* entity)
	{
		if (!entity || !m_EditorCamera)
		{
			HZ_WARN("FocusOnEntity called with null entity or camera");
			return;
		}

		// Get the entity's position from its transform
		glm::vec3 entityPos = entity->EntityTransform.Position;
		
		// Calculate the size of the object based on its scale
		// Use the maximum scale component to determine distance
		float maxScale = glm::max(glm::max(entity->EntityTransform.Scale.x, 
		                                    entity->EntityTransform.Scale.y), 
		                          entity->EntityTransform.Scale.z);
		
		// Calculate camera distance based on object size
		// Distance should be enough to frame the object nicely
		float distance = maxScale * 3.0f;
		if (distance < 5.0f) distance = 5.0f; // Minimum distance
		
		// Position camera behind and slightly above the object
		glm::vec3 offset(0.0f, distance * 0.3f, distance);
		glm::vec3 newCameraPos = entityPos + offset;
		
		// Set the camera position
		m_EditorCamera->SetPosition(newCameraPos);
		
		// Calculate direction from camera to entity
		glm::vec3 direction = glm::normalize(entityPos - newCameraPos);
		
		// Calculate yaw and pitch from direction vector
		// yaw: rotation around Y axis (horizontal)
		// pitch: rotation around X axis (vertical)
		float yaw = glm::degrees(atan2(direction.z, direction.x));
		float pitch = glm::degrees(asin(direction.y));
		
		// Set camera orientation to look at the entity
		m_EditorCamera->SetYaw(yaw);
		m_EditorCamera->SetPitch(pitch);
		
		HZ_INFO("Camera focused on entity: " + entity->Name + 
		        " at pos(" + std::to_string(entityPos.x) + "," + std::to_string(entityPos.y) + "," + std::to_string(entityPos.z) + ")" +
		        " camera at(" + std::to_string(newCameraPos.x) + "," + std::to_string(newCameraPos.y) + "," + std::to_string(newCameraPos.z) + ")" +
		        " yaw=" + std::to_string(yaw) + " pitch=" + std::to_string(pitch));
	}

	void EditorLayer::CreateEntity(const std::string& name, MeshType meshType)
	{
		Entity newEntity(name, m_NextEntityID++);
		newEntity.Mesh = meshType;
		
		// Set default colors based on mesh type
		switch (meshType)
		{
		case MeshType::Cube:
			newEntity.Color = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f); // Red
			break;
		case MeshType::Sphere:
			newEntity.Color = glm::vec4(0.3f, 0.8f, 0.3f, 1.0f); // Green
			break;
		case MeshType::Capsule:
			newEntity.Color = glm::vec4(0.3f, 0.3f, 0.8f, 1.0f); // Blue
			break;
		default:
			newEntity.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // White
			break;
		}
		
		m_Entities.push_back(newEntity);
	}

	void EditorLayer::RenderScene()
	{
		static bool logOnce = true;
		if (logOnce)
		{
			HZ_INFO("RenderScene() called for the first time");
			logOnce = false;
		}
		
		// Bind framebuffer and clear
		m_SceneFramebuffer->Bind();
		Hazel::Renderer::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		Hazel::Renderer::Clear();
		
		// Clear lights and add scene light
		Hazel::Renderer::ClearLights();
		Hazel::Renderer::AddLight(m_SceneLight);
		
		// Begin scene with editor camera
		Hazel::Renderer::BeginScene(*m_EditorCamera);
		
		// Render all entities with meshes
		int entitiesRendered = 0;
		int totalEntities = static_cast<int>(m_Entities.size());
		for (auto& entity : m_Entities)
		{
			if (entity.Mesh != MeshType::None)
			{
				auto meshVA = GetMeshVertexArray(entity.Mesh);
				if (meshVA)
				{
					// Update material color
					m_DefaultMaterial->SetColor(entity.Color);
					
					// Get transform matrix
					glm::mat4 transform = entity.EntityTransform.GetTransformMatrix();
					
					// Submit for rendering
					Hazel::Renderer::Submit(meshVA, m_DefaultMaterial, transform);
					entitiesRendered++;
				}
				else
				{
					HZ_WARN("Mesh vertex array is null for entity: " + entity.Name);
				}
			}
		}
		
		static bool logRenderOnce = true;
		if (logRenderOnce)
		{
			HZ_INFO("First render pass: " + std::to_string(entitiesRendered) + " entities rendered out of " + std::to_string(totalEntities) + " total");
			logRenderOnce = false;
		}
		
		if (entitiesRendered == 0 && totalEntities > 0)
		{
			HZ_WARN("No entities were rendered in scene (0/" + std::to_string(totalEntities) + ")");
		}
		
		Hazel::Renderer::EndScene();
		
		// Unbind framebuffer
		m_SceneFramebuffer->Unbind();
	}

	void EditorLayer::InitializeMeshBuffers()
	{
		using namespace Hazel;
		
		HZ_INFO("Initializing mesh buffers...");
		
		// Create cube mesh
		{
			MeshData cubeData = MeshGenerator::CreateCube(1.0f);
			HZ_INFO("Cube mesh data: " + std::to_string(cubeData.Vertices.size()) + " vertices, " + std::to_string(cubeData.Indices.size()) + " indices");
			
			m_CubeMesh = std::make_shared<VertexArray>();
			m_CubeVertexBuffer = std::make_shared<VertexBuffer>(
				cubeData.Vertices.data(), 
				static_cast<unsigned int>(cubeData.Vertices.size() * sizeof(float)));
			m_CubeVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" }
			});
			m_CubeMesh->AddVertexBuffer(m_CubeVertexBuffer.get());
			
			m_CubeIndexBuffer = std::make_shared<IndexBuffer>(
				cubeData.Indices.data(), 
				static_cast<unsigned int>(cubeData.Indices.size()));
			m_CubeMesh->SetIndexBuffer(m_CubeIndexBuffer.get());
		}
		
		// Create sphere mesh
		{
			MeshData sphereData = MeshGenerator::CreateSphere(0.5f, 32);
			
			m_SphereMesh = std::make_shared<VertexArray>();
			m_SphereVertexBuffer = std::make_shared<VertexBuffer>(
				sphereData.Vertices.data(), 
				static_cast<unsigned int>(sphereData.Vertices.size() * sizeof(float)));
			m_SphereVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" }
			});
			m_SphereMesh->AddVertexBuffer(m_SphereVertexBuffer.get());
			
			m_SphereIndexBuffer = std::make_shared<IndexBuffer>(
				sphereData.Indices.data(), 
				static_cast<unsigned int>(sphereData.Indices.size()));
			m_SphereMesh->SetIndexBuffer(m_SphereIndexBuffer.get());
		}
		
		// Create capsule mesh
		{
			MeshData capsuleData = MeshGenerator::CreateCapsule(1.0f, 0.5f, 32);
			
			m_CapsuleMesh = std::make_shared<VertexArray>();
			m_CapsuleVertexBuffer = std::make_shared<VertexBuffer>(
				capsuleData.Vertices.data(), 
				static_cast<unsigned int>(capsuleData.Vertices.size() * sizeof(float)));
			m_CapsuleVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" }
			});
			m_CapsuleMesh->AddVertexBuffer(m_CapsuleVertexBuffer.get());
			
			m_CapsuleIndexBuffer = std::make_shared<IndexBuffer>(
				capsuleData.Indices.data(), 
				static_cast<unsigned int>(capsuleData.Indices.size()));
			m_CapsuleMesh->SetIndexBuffer(m_CapsuleIndexBuffer.get());
		}
	}

	std::shared_ptr<Hazel::VertexArray> EditorLayer::GetMeshVertexArray(MeshType type)
	{
		switch (type)
		{
		case MeshType::Cube:
			return m_CubeMesh;
		case MeshType::Sphere:
			return m_SphereMesh;
		case MeshType::Capsule:
			return m_CapsuleMesh;
		default:
			return nullptr;
		}
	}

}
