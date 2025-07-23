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
		EditorLayer(std::weak_ptr<IEventDispatcher> eventDispatcher,
					std::shared_ptr<Asset::ResourcePool> resourcePool, 
					std::shared_ptr<Asset::Manager> assetManager, 
					std::shared_ptr<Profiler> profiler
		);

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;

	private:
		void DrawMainMenu();

		glm::ivec2 prevViewportWindowSize, prevViewportWindowPos, viewportSize;
		ImVec2 topLeftTextureCoords, bottomRightTextureCoords;
		float aspectRatio;

	private:
		// Engine
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<Asset::ResourcePool> m_ResourcePool;
		std::shared_ptr<Asset::Manager> m_AssetManager;
		std::shared_ptr<Profiler> m_Profiler;

		// Values received from events
		std::weak_ptr<Scene> m_Scene;
		std::weak_ptr<IImage2D> m_LatestFrameRender;

		// Editor
		std::shared_ptr<EditorState> m_EditorState; // shared across panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		InspectorPanel m_InspectorPanel;
		ViewportPanel m_ViewportPanel;
		ThemePanel m_ThemePanel;
		ProfilerPanel m_ProfilerPanel;
		RenderSettingsPanel m_RenderSettingsPanel;
		AssetsPanel m_AssetsPanel;
	};
}