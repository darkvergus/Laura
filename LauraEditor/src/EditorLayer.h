#pragma once
#include <Laura.h>

#include "EditorState.h"
#include "EditorUI/ViewportPanel/ViewportPanel.h"
#include "EditorUI/SceneHierarchyPanel/SceneHierarchyPanel.h"
#include "EditorUI/InspectorPanel/InspectorPanel.h"
#include "EditorUI/ProfilerPanel/ProfilerPanel.h"
#include "EditorUI/RenderSettingsPanel/RenderSettingsPanel.h"
#include "EditorUI/ThemePanel/ThemePanel.h"
#include "EditorUI/AssetsPanel/AssetsPanel.h"

namespace Laura
{
	class EditorLayer : public ILayer
	{
	public:
		EditorLayer(std::shared_ptr<Renderer> renderer, 
					std::shared_ptr<Asset::ResourcePool> resourcePool, 
					std::shared_ptr<Asset::Manager> assetManager, 
					std::shared_ptr<Profiler> profiler
		);

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event* event) override;
	private:
		glm::ivec2 prevViewportWindowSize, prevViewportWindowPos, viewportSize;
		ImVec2 topLeftTextureCoords, bottomRightTextureCoords;
		float aspectRatio;

	private:
		// > ENGINE RELATED < //
		std::shared_ptr<Renderer> m_Renderer;
		std::shared_ptr<Asset::ResourcePool> m_ResourcePool;
		std::shared_ptr<Asset::Manager> m_AssetManager;
		std::shared_ptr<Profiler> m_Profiler;
		std::shared_ptr<Scene> m_Scene;

		// > EDITOR RELATED < //
		std::shared_ptr<EditorState> m_EditorState;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		InspectorPanel m_InspectorPanel;
		ViewportPanel m_ViewportPanel;
		ThemePanel m_ThemePanel;
		ProfilerPanel m_ProfilerPanel;
		RenderSettingsPanel m_RenderSettingsPanel;
		AssetsPanel m_AssetsPanel;
	};
}