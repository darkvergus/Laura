#pragma once

#include <Laura.h>
#include "EditorState.h"

/// --- PANELS --- ///
#include "EditorUI/ViewportPanel.h"
#include "EditorUI/SceneHierarchyPanel.h"
#include "EditorUI/InspectorPanel.h"

namespace Laura
{
	class EditorLayer : public ILayer
	{
	public:
		EditorLayer(std::shared_ptr<Renderer> renderer, std::shared_ptr<AssetManager> assetManager);
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event* event) override;
	private:
		std::shared_ptr<Renderer> m_Renderer;
		std::shared_ptr<AssetManager> m_AssetManager;
	private:
		glm::ivec2 prevViewportWindowSize, prevViewportWindowPos, viewportSize;
		ImVec2 topLeftTextureCoords, bottomRightTextureCoords;
		float aspectRatio;

	private:
		// The Editor Layer owns the Editor State (and deletes it)
		EditorState* m_EditorState;
		// The Editor Layer manages the Scenes
		// It has to manage the LOADING and UNLOADING of scenes
		// UI panels only work with the active scene
		std::shared_ptr<Scene> m_Scene;

		// UI panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		InspectorPanel m_InspectorPanel;
		ViewportPanel m_ViewportPanel;
	};

}