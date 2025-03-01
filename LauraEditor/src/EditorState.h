#pragma once
#include <yaml-cpp/yaml.h>
#include "Laura.h"

#define EDITOR_STATE_FILE_PATH EDITOR_RESOURCES_PATH "EditorStatePersistent.yaml"

namespace Laura
{
	enum struct ViewportMode
	{
		FIT_TO_VIEWPORT,
		STRETCH_TO_VIEWPORT,
		CENTERED
	};

	struct EditorState
	{
		// TO ADD new temporary entries, simply add them here (these are not serialized/deserialized)
		struct
		{
			entt::entity selectedEntity = entt::null;
			// panels
			bool ViewportSettingsPanelOpen = false;
			bool ThemeSettingsPanelOpen = false;
			bool ProfilerPanelOpen = true;
		} temp;

		// TO ADD new persistent entries, add them here and update the SERIALIZE and DESERIALIZE functions 
		// (if the type is custom, also create a YAML::convert template specialization)
		struct
		{
			bool doubleConfirmation = false;
			/// VIEWPORT ///
			ViewportMode viewportMode = ViewportMode::FIT_TO_VIEWPORT;
			/// THEMES ///
			std::string ThemeFilePath = "";
		} persistent;
	};

	bool serializeState(const std::shared_ptr<const EditorState>& state);
	bool deserializeState(const std::shared_ptr<EditorState>& state);
}

// tell yaml-cpp how to convert the ViewportMode enum to and from a string when serializing and deserializing
template<>
struct YAML::convert<Laura::ViewportMode>
{
	static YAML::Node encode(const Laura::ViewportMode& rhs)
	{
		switch (rhs)
		{
		case Laura::ViewportMode::FIT_TO_VIEWPORT: return Node("FIT_TO_VIEWPORT");
		case Laura::ViewportMode::STRETCH_TO_VIEWPORT: return Node("STRETCH_TO_VIEWPORT");
		case Laura::ViewportMode::CENTERED: return Node("CENTERED");
		}
		return YAML::Node("FIT_TO_VIEWPORT");
	}

	static bool decode(const YAML::Node& node, Laura::ViewportMode& rhs)
	{
		std::string value = node.as<std::string>();
		if (value == "FIT_TO_VIEWPORT") rhs = Laura::ViewportMode::FIT_TO_VIEWPORT;
		else if (value == "STRETCH_TO_VIEWPORT") rhs = Laura::ViewportMode::STRETCH_TO_VIEWPORT;
		else if (value == "CENTERED") rhs = Laura::ViewportMode::CENTERED;
		else rhs = Laura::ViewportMode::FIT_TO_VIEWPORT;
		return true;
	}
};
