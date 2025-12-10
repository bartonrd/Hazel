#pragma once

#include <Hazel.h>
#include <string>
#include <vector>

namespace HazelEditor {

	// Simple entity for scene hierarchy demonstration
	struct Entity
	{
		std::string Name;
		int ID;
		bool IsSelected = false;
		std::vector<Entity*> Children;

		Entity(const std::string& name, int id) : Name(name), ID(id) {}
	};

	class EditorLayer : public Hazel::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnImGuiRender() override;

	private:
		void DrawMenuBar();
		void DrawSceneHierarchy();
		void DrawInspector();
		void DrawConsole();
		void DrawGameView();
		void DrawSceneView();
		void DrawAssetBrowser();
		void DrawToolbar();

		// Helper functions
		void DrawEntityNode(Entity* entity);
		void ClearSelection();

	private:
		// Scene data
		std::vector<Entity> m_Entities;
		Entity* m_SelectedEntity = nullptr;

		// Editor state
		bool m_IsPlaying = false;
		bool m_IsPaused = false;

		// Console state
		bool m_ShowTrace = true;
		bool m_ShowInfo = true;
		bool m_ShowWarn = true;
		bool m_ShowError = true;
		bool m_ShowFatal = true;
		bool m_AutoScroll = true;

		// Asset browser state
		std::string m_CurrentDirectory = "Assets";
	};

}
