#pragma once

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <cassert>
#include "Laura.h"
#include "EditorTheme.h"

#define EDITOR_STATE_FILE_PATH EDITOR_RESOURCES_PATH "EditorStatePersistent.yaml"

namespace Laura
{

	enum struct ViewportMode {
		FitToViewport,
		StretchToViewport,
		CenterToViewport,
		COUNT
	};
	inline constexpr const char* ViewportModeStr[] = { 
		"Fit", 
		"Stretch", 
		"Center" 
	};
	static_assert(std::size(ViewportModeStr) == static_cast<size_t>(ViewportMode::COUNT),
				  "ViewportModeStr must match ViewportMode enum");

	struct EditorState {
		struct {
			entt::entity selectedEntity = entt::null;
			bool isViewportSettingsPanelOpen = false;
			bool isThemePanelOpen = false;
			bool isProfilerPanelOpen = true;
			EditorTheme editorTheme;
		} temp;

		// TO ADD new persistent entries, add them here and update the SERIALIZE and DESERIALIZE functions 
		// (if the type is custom, also create a YAML::convert template specialization)
		struct {
			ViewportMode viewportMode = ViewportMode::FitToViewport;
			std::filesystem::path editorThemeFilepath = "";
		} persistent;
	};

	bool serializeState(const std::shared_ptr<const EditorState>& state);
	// also deserializes derived state (EditorTheme from editorThemeFilepath)
	bool deserializeState(const std::shared_ptr<EditorState>& state); 
}

template<>
struct YAML::convert<Laura::ViewportMode> {
	static YAML::Node encode(const Laura::ViewportMode& rhs) {
		switch (rhs) {
			case Laura::ViewportMode::StretchToViewport:	
				return Node("StretchToViewport");
			case Laura::ViewportMode::CenterToViewport:		
				return Node("CenterToViewport");
			default:
				return Node("FitToViewport");
		}
	}

	static bool decode(const YAML::Node& node, Laura::ViewportMode& rhs) {
		std::string value = node.as<std::string>();
		if (value == "StretchToViewport")		{ rhs = Laura::ViewportMode::StretchToViewport; }
		else if (value == "CenterToViewport")	{ rhs = Laura::ViewportMode::CenterToViewport;	}
		else									{ rhs = Laura::ViewportMode::FitToViewport;		}
		return true;
	}
};
