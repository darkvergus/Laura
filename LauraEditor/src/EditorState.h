#pragma once
#include <Laura.h>

namespace Laura
{
	enum struct ViewportMode
	{
		FIT_TO_VIEWPORT,
		STRETCH_TO_VIEWPORT,
		CENTERED
	};

	// Default values for the editor when first opened
	// (the editor state will be serialized and saved)
	struct EditorState
	{
		entt::entity selectedEntity = entt::null;
		bool doubleConfirmation = true;
		
		/// VIEWPORT ///
		bool ViewportSettingsPanelOpen = false;
		ViewportMode viewportMode = ViewportMode::FIT_TO_VIEWPORT;
		
		/// THEMES ///
		bool ThemeSettingsPanelOpen = false;
		std::string ActiveThemeFilePath = "";
	};
}