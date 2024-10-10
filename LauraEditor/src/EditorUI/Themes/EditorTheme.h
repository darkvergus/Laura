#pragma once
#include <imgui.h>
#include <yaml-cpp/yaml.h>
#include "Laura.h"

namespace Laura
{
	#define LR_THEME_FILE_EXTENSION ".lrtheme"
	
	struct Theme
	{
		// Backgrounds
		ImVec4 DefaultWindowBg = ImVec4{ 0.109f, 0.109f, 0.109f, 1.0f };           // ImGuiCol_WindowBg
		ImVec4 DefaultPopupBg = ImVec4{ 0.08f, 0.08f, 0.08f, 0.940f };            // ImGuiCol_PopupBg
		// Borders
		ImVec4 DefaultBorder = ImVec4{ 0.174f, 0.174f, 0.174f, 0.5f };             // ImGuiCol_Border
		// Headers
		ImVec4 DefaultHeader = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f };            // ImGuiCol_Header
		ImVec4 DefaultHeaderHovered = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };      // ImGuiCol_HeaderHovered
		ImVec4 DefaultHeaderActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };       // ImGuiCol_HeaderActive
		// Buttons
		ImVec4 DefaultButton = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };             // ImGuiCol_Button (transparent)
		ImVec4 DefaultButtonHovered = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };    // ImGuiCol_ButtonHovered
		ImVec4 DefaultButtonActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };       // ImGuiCol_ButtonActive
		// Checkmarks
		ImVec4 DefaultCheckMark = ImVec4{ 0.391f, 0.391f, 0.391f, 1.0f };         // ImGuiCol_CheckMark
		// Sliders
		ImVec4 DefaultSliderGrab = ImVec4{ 0.391f, 0.391f, 0.391f, 1.0f };         // ImGuiCol_SliderGrab
		ImVec4 DefaultSliderGrabActive = ImVec4{ 0.491f, 0.811f, 0.419f, 1.0f };   // ImGuiCol_SliderGrabActive
		// Frame BG
		ImVec4 DefaultFrameBg = ImVec4{ 0.139f, 0.139f, 0.139f, 1.0f };            // ImGuiCol_FrameBg
		ImVec4 DefaultFrameBgHovered = ImVec4{ 0.199f, 0.199f, 0.199f, 1.0f };     // ImGuiCol_FrameBgHovered
		ImVec4 DefaultFrameBgActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };      // ImGuiCol_FrameBgActive
		// Tabs
		ImVec4 DefaultTab = ImVec4{ 0.130f, 0.130f, 0.130f, 1.0f };							// ImGuiCol_Tab
		ImVec4 DefaultTabHovered = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f };					// ImGuiCol_TabHovered	
		ImVec4 DefaultTabSelected = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f };				// ImGuiCol_TabSelected
		ImVec4 DefaultTabSelectedOverline = ImVec4{ 0.239f, 0.239f, 0.239f, 1.0f };		// ImGuiCol_TabSelectedOverline
		ImVec4 DefaultTabDimmed = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f }; 			   // ImGuiCol_TabDimmed
		ImVec4 DefaultTabDimmedSelected = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f };		// ImGuiCol_TabDimmedSelected
		ImVec4 DefaultTabDimmedSelectedOverline = ImVec4{ 0.239f, 0.239f, 0.239f, 1.0f };	// ImGuiCol_TabDimmedSelectedOverline
		// Title
		ImVec4 DefaultTitleBg = ImVec4{ 0.068f, 0.068f, 0.068f, 1.0f };            // ImGuiCol_TitleBg
		ImVec4 DefaultTitleBgActive = ImVec4{ 0.068f, 0.068f, 0.068f, 1.0f };      // ImGuiCol_TitleBgActive
		ImVec4 DefaultTitleBgCollapsed = ImVec4{ 0.068f, 0.068f, 0.068f, 1.0f };   // ImGuiCol_TitleBgCollapsed
		// Scrollbar
		ImVec4 DefaultScrollbarGrab = ImVec4{ 0.224f, 0.224f, 0.224f, 1.0f };      // ImGuiCol_ScrollbarGrab
		// Grips
		ImVec4 DefaultResizeGrip = ImVec4{ 1.0f, 1.0f, 1.0f, 0.200f };        // ImGuiCol_ResizeGrip
		ImVec4 DefaultResizeGripHovered = ImVec4{ 1.0f, 1.0f, 1.0f, 0.290f };  // ImGuiCol_ResizeGripHovered
		ImVec4 DefaultResizeGripActive = ImVec4{ 1.0f, 1.0f, 1.0f, 0.360f };   // ImGuiCol_ResizeGripActive
		// Separator
		ImVec4 DefaultSeparator = ImVec4{ 1.0f, 1.0f, 1.0f, 0.200f };          // ImGuiCol_Separator
		ImVec4 DefaultSeparatorHovered = ImVec4{ 1.0f, 1.0f, 1.0f, 0.290f };   // ImGuiCol_SeparatorHovered
		ImVec4 DefaultSeparatorActive = ImVec4{ 1.0f, 1.0f, 1.0f, 0.360f };    // ImGuiCol_SeparatorActive
		// Text
		ImVec4 DefaultText = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };               // ImGuiCol_Text
		ImVec4 DefaultTextDisabled = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };// ImGuiCol_TextDisabled
		// Transform widget
		ImVec4 TransformButtonXbg = ImVec4{ 0.109f, 0.109f, 0.109f, 1.0f };
		ImVec4 TransformButtonYbg = ImVec4{ 0.109f, 0.109f, 0.109f, 1.0f };
		ImVec4 TransformButtonZbg = ImVec4{ 0.109f, 0.109f, 0.109f, 1.0f };
		ImVec4 TransformButtonXHovered = ImVec4{ 0.409f, 0.109f, 0.109f, 1.0f };
		ImVec4 TransformButtonYHovered = ImVec4{ 0.109f, 0.409f, 0.109f, 1.0f };
		ImVec4 TransformButtonZHovered = ImVec4{ 0.109f, 0.109f, 0.409f, 1.0f };
		ImVec4 TransformButtonXActive = ImVec4{ 0.609f, 0.109f, 0.109f, 1.0f };
		ImVec4 TransformButtonYActive = ImVec4{ 0.109f, 0.609f, 0.109f, 1.0f };
		ImVec4 TransformButtonZActive = ImVec4{ 0.109f, 0.109f, 0.609f, 1.0f };
		// Secondary button styles
		ImVec4 ButtonGray = ImVec4{ 0.169f, 0.169f, 0.169f, 1.0f };
		// Selected headers
		ImVec4 SelectedHeader = ImVec4{ 0.138f, 0.138f, 0.138f, 1.0f };
		
		ImVec4 AddComponentButton = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f };
	};

	class ThemeManager
	{
	public:
		ThemeManager();

		inline bool LoadTheme(const std::string& filepath, std::string& statusMessage)
		{
			bool success = DeserializeTheme(m_ActiveTheme, filepath, statusMessage);
			ApplyThemeColors();
			return success;
		}

		// Example usage: ThemeManager.ImGuiSet(ImGuiCol_Text, ThemeManager.GetActiveTheme()->Text);
		inline void ImGuiSet(ImGuiCol_ widget, ImVec4 color)
		{
			ImGui::GetStyle().Colors[widget] = color;
		}

		// sets the currently active colors for ImGui widgets to defalut values of the currently active theme
		void ApplyThemeColors();

		// will generate a .lrtheme file at the specified filepath with the default theme values
		inline bool GenerateDefaultThemeFile(const std::string& filepath, std::string& statusMessage)
		{
			return SerializeTheme(std::make_shared<Theme>(), filepath, statusMessage);
		}

		inline std::shared_ptr<const Theme> GetActiveTheme() { return m_ActiveTheme; }
	private:
		std::shared_ptr<Theme> m_ActiveTheme;

	private:
		// Serializes the theme and saves it to a .lauratheme file at the filepath
		bool SerializeTheme(std::shared_ptr<Theme> theme, const std::string& filepath, std::string& statusMessage);
		// Deserializes the theme from a .lauratheme file at the filepath and outputs it to themeOut
		bool DeserializeTheme(std::shared_ptr<Theme> themeOut, const std::string& filepath, std::string& statusMessage);
	};
}

template <>
struct YAML::convert<ImVec4>
{
	static YAML::Node encode(const ImVec4& rhs)
	{
		YAML::Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		return node;
	}

	static bool decode(const YAML::Node& node, ImVec4& rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};

template <>
struct YAML::convert<Laura::Theme>
{
	static YAML::Node encode(const Laura::Theme& rhs)
	{
		YAML::Node node;
		// Backgrounds
		node["DefaultWindowBg"] = rhs.DefaultWindowBg;
		node["DefaultPopupBg"] = rhs.DefaultPopupBg;
		// Borders
		node["DefaultBorder"] = rhs.DefaultBorder;
		// Headers
		node["DefaultHeader"] = rhs.DefaultHeader;
		node["DefaultHeaderHovered"] = rhs.DefaultHeaderHovered;
		node["DefaultHeaderActive"] = rhs.DefaultHeaderActive;
		// Buttons
		node["DefaultButton"] = rhs.DefaultButton;
		node["DefaultButtonHovered"] = rhs.DefaultButtonHovered;
		node["DefaultButtonActive"] = rhs.DefaultButtonActive;
		// Checkmarks
		node["DefaultCheckMark"] = rhs.DefaultCheckMark;
		// Sliders
		node["DefaultSliderGrab"] = rhs.DefaultSliderGrab;
		node["DefaultSliderGrabActive"] = rhs.DefaultSliderGrabActive;
		// Frame
		node["DefaultFrameBg"] = rhs.DefaultFrameBg;
		node["DefaultFrameBgHovered"] = rhs.DefaultFrameBgHovered;
		node["DefaultFrameBgActive"] = rhs.DefaultFrameBgActive;
		// Tabs
		node["DefaultTab"] = rhs.DefaultTab;
		node["DefaultTabHovered"] = rhs.DefaultTabHovered;
		node["DefaultTabSelected"] = rhs.DefaultTabSelected;
		node["DefaultTabSelectedOverline"] = rhs.DefaultTabSelectedOverline;
		node["DefaultTabDimmed"] = rhs.DefaultTabDimmed;
		node["DefaultTabDimmedSelected"] = rhs.DefaultTabDimmedSelected;
		node["DefaultTabDimmedSelectedOverline"] = rhs.DefaultTabDimmedSelectedOverline;
		// Title
		node["DefaultTitleBg"] = rhs.DefaultTitleBg;
		node["DefaultTitleBgActive"] = rhs.DefaultTitleBgActive;
		node["DefaultTitleBgCollapsed"] = rhs.DefaultTitleBgCollapsed;
		// Scrollbar
		node["DefaultScrollbarGrab"] = rhs.DefaultScrollbarGrab;
		// Grips
		node["DefaultResizeGrip"] = rhs.DefaultResizeGrip;
		node["DefaultResizeGripHovered"] = rhs.DefaultResizeGripHovered;
		node["DefaultResizeGripActive"] = rhs.DefaultResizeGripActive;
		// Separator
		node["DefaultSeparator"] = rhs.DefaultSeparator;
		node["DefaultSeparatorHovered"] = rhs.DefaultSeparatorHovered;
		node["DefaultSeparatorActive"] = rhs.DefaultSeparatorActive;
		// Text
		node["DefaultText"] = rhs.DefaultText;
		node["DefaultTextDisabled"] = rhs.DefaultTextDisabled;
		// Transform
		node["TransformButtonXbg"] = rhs.TransformButtonXbg;
		node["TransformButtonYbg"] = rhs.TransformButtonYbg;
		node["TransformButtonZbg"] = rhs.TransformButtonZbg;
		node["TransformButtonXHovered"] = rhs.TransformButtonXHovered;
		node["TransformButtonYHovered"] = rhs.TransformButtonYHovered;
		node["TransformButtonZHovered"] = rhs.TransformButtonZHovered;
		node["TransformButtonXActive"] = rhs.TransformButtonXActive;
		node["TransformButtonYActive"] = rhs.TransformButtonYActive;
		node["TransformButtonZActive"] = rhs.TransformButtonZActive;
		// Secondary button styles
		node["ButtonGray"] = rhs.ButtonGray;
		// Selected headers
		node["SelectedHeader"] = rhs.SelectedHeader;

		node["AddComponentButton"] = rhs.AddComponentButton;

		return node;
	}

	static bool decode(const YAML::Node& node, Laura::Theme& rhs)
	{
		if (!node.IsMap())
			return false;

		// Backgrounds
		rhs.DefaultWindowBg = node["DefaultWindowBg"].as<ImVec4>();
		rhs.DefaultPopupBg = node["DefaultPopupBg"].as<ImVec4>();
		// Borders
		rhs.DefaultBorder = node["DefaultBorder"].as<ImVec4>();
		// Headers
		rhs.DefaultHeader = node["DefaultHeader"].as<ImVec4>();
		rhs.DefaultHeaderHovered = node["DefaultHeaderHovered"].as<ImVec4>();
		rhs.DefaultHeaderActive = node["DefaultHeaderActive"].as<ImVec4>();
		// Buttons
		rhs.DefaultButton = node["DefaultButton"].as<ImVec4>();
		rhs.DefaultButtonHovered = node["DefaultButtonHovered"].as<ImVec4>();
		rhs.DefaultButtonActive = node["DefaultButtonActive"].as<ImVec4>();
		// Checkmarks
		rhs.DefaultCheckMark = node["DefaultCheckMark"].as<ImVec4>();
		// Sliders
		rhs.DefaultSliderGrab = node["DefaultSliderGrab"].as<ImVec4>();
		rhs.DefaultSliderGrabActive = node["DefaultSliderGrabActive"].as<ImVec4>();
		// Frame
		rhs.DefaultFrameBg = node["DefaultFrameBg"].as<ImVec4>();
		rhs.DefaultFrameBgHovered = node["DefaultFrameBgHovered"].as<ImVec4>();
		rhs.DefaultFrameBgActive = node["DefaultFrameBgActive"].as<ImVec4>();
		// Tabs
		rhs.DefaultTab = node["DefaultTab"].as<ImVec4>();
		rhs.DefaultTabHovered = node["DefaultTabHovered"].as<ImVec4>();
		rhs.DefaultTabSelected = node["DefaultTabSelected"].as<ImVec4>();
		rhs.DefaultTabSelectedOverline = node["DefaultTabSelectedOverline"].as<ImVec4>();
		rhs.DefaultTabDimmed = node["DefaultTabDimmed"].as<ImVec4>();
		rhs.DefaultTabDimmedSelected = node["DefaultTabDimmedSelected"].as<ImVec4>();
		rhs.DefaultTabDimmedSelectedOverline = node["DefaultTabDimmedSelectedOverline"].as<ImVec4>();
		// Title
		rhs.DefaultTitleBg = node["DefaultTitleBg"].as<ImVec4>();
		rhs.DefaultTitleBgActive = node["DefaultTitleBgActive"].as<ImVec4>();
		rhs.DefaultTitleBgCollapsed = node["DefaultTitleBgCollapsed"].as<ImVec4>();
		// Scrollbar
		rhs.DefaultScrollbarGrab = node["DefaultScrollbarGrab"].as<ImVec4>();
		// Grips
		rhs.DefaultResizeGrip = node["DefaultResizeGrip"].as<ImVec4>();
		rhs.DefaultResizeGripHovered = node["DefaultResizeGripHovered"].as<ImVec4>();
		rhs.DefaultResizeGripActive = node["DefaultResizeGripActive"].as<ImVec4>();
		// Separator
		rhs.DefaultSeparator = node["DefaultSeparator"].as<ImVec4>();
		rhs.DefaultSeparatorHovered = node["DefaultSeparatorHovered"].as<ImVec4>();
		rhs.DefaultSeparatorActive = node["DefaultSeparatorActive"].as<ImVec4>();
		// Text
		rhs.DefaultText = node["DefaultText"].as<ImVec4>();
		rhs.DefaultTextDisabled = node["DefaultTextDisabled"].as<ImVec4>();
		// Transform
		rhs.TransformButtonXbg = node["TransformButtonXbg"].as<ImVec4>();
		rhs.TransformButtonYbg = node["TransformButtonYbg"].as<ImVec4>();
		rhs.TransformButtonZbg = node["TransformButtonZbg"].as<ImVec4>();
		rhs.TransformButtonXHovered = node["TransformButtonXHovered"].as<ImVec4>();
		rhs.TransformButtonYHovered = node["TransformButtonYHovered"].as<ImVec4>();
		rhs.TransformButtonZHovered = node["TransformButtonZHovered"].as<ImVec4>();
		rhs.TransformButtonXActive = node["TransformButtonXActive"].as<ImVec4>();
		rhs.TransformButtonYActive = node["TransformButtonYActive"].as<ImVec4>();
		rhs.TransformButtonZActive = node["TransformButtonZActive"].as<ImVec4>();
		// Secondary button styles
		rhs.ButtonGray = node["ButtonGray"].as<ImVec4>();
		// Selected headers
		rhs.SelectedHeader = node["SelectedHeader"].as<ImVec4>();

		rhs.AddComponentButton = node["AddComponentButton"].as<ImVec4>();

		return true;
	}
};