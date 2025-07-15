#pragma once

#include <Laura.h>
#include "EditorState.h"
#include "IconsFontAwesome6.h"

namespace Laura 
{
	class AssetsPanel {
	public:
		AssetsPanel(std::shared_ptr<EditorState> editorState, 
					std::shared_ptr<Asset::Manager> assetManager, 
					std::shared_ptr<Asset::ResourcePool> resourcePool)
			: m_EditorState(editorState),
			  m_AssetManager(assetManager), 
			  m_ResourcePool(resourcePool) {
		}
		~AssetsPanel() = default;
	
		void OnImGuiRender();

	private:
		void DrawAssetTile(LR_GUID guid, const char* icon, const char* title);
		
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<Asset::Manager> m_AssetManager;
		std::shared_ptr<Asset::ResourcePool> m_ResourcePool;

		// UI related
		const float BASE_TILE_WH_RATIO = 0.75f;
		const float BASE_TILE_ICON_FONT_SIZE = 0.3f;
		const float BASE_TILE_TITLE_FONT_SIZE = 13.0f;
		const float TILE_SCALAR_MIN = 75.0f;

		const float TILE_SCALAR_MAX = 110.0f;
		LR_GUID m_SelectedTile = LR_GUID::INVALID;
		float m_TileScalar = TILE_SCALAR_MIN;
	};
}