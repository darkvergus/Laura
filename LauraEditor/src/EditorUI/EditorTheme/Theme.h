#pragma once
#include "Laura.h"
#include <filesystem> 
#include <yaml-cpp/yaml.h>
#include <stack>

namespace Laura
{
	#define EDITOR_THEME_FILE_EXTENSION ".lrtheme"
	
	class EditorTheme {
	public: 
		EditorTheme() {
			LoadDefaultLight();
			ApplyThemeToImgui();
		}

		inline std::pair<bool, std::string> LoadFromFile(const std::filesystem::path& filepath) {
			auto [success, errMsg] = DeserializeFromYAML(filepath);
			if (success) {
				ApplyThemeToImgui();
			}
			return { success, errMsg };
		}

		inline std::pair<bool, std::string> SaveToFile(const std::filesystem::path& filepath) {
			return SerializeToYAML(filepath);
		}
			
		inline void PushColor(ImGuiCol_ widget, ImVec4 col) {
			// cache old color on the stack
			m_ColorStack.push({ widget, ImGui::GetStyle().Colors[widget] });
			// set new color	
			ImGui::GetStyle().Colors[widget] = col; 
		}

		inline void PopColor(size_t count = 1) {
			if (count == 0) {
				throw std::runtime_error("Count must be > 0");
			}

			if (count > m_ColorStack.size()) {
				throw std::runtime_error("Tried to PopColor from an empty color stack");
			}

			for (size_t i = 0; i < count; i++) {
				// pop the latest widget change and update color 
				auto [widget, oldCol] = m_ColorStack.top();
				m_ColorStack.pop();
				// reset to old color
				ImGui::GetStyle().Colors[widget] = oldCol;
			}
		}

		// DEFAULT HARDCODED THEMES:
		inline void LoadDefaultDark() {
			name = "Default Dark";
			PRIMARY_1     = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };         // HeaderHovered, ButtonHovered
			PRIMARY_2     = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };      // HeaderActive, ButtonActive, FrameBgActive
			SECONDARY_1   = ImVec4{ 0.078f, 0.078f, 0.078f, 1.0f };      // Header, TabHovered, TabSelected, AddComponentButton
			SECONDARY_2   = ImVec4{ 0.138f, 0.138f, 0.138f, 1.0f };      // SelectedHeader
			ACCENT_1      = ImVec4{ 0.491f, 0.811f, 0.419f, 1.0f };      // SliderGrabActive (greenish accent)
			ACCENT_2      = ImVec4{ 0.391f, 0.391f, 0.391f, 1.0f };      // CheckMark, SliderGrab
			TEXT_1        = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };            // Text
			TEXT_2        = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };            // TextDisabled
			BACKGROUND_1  = ImVec4{ 0.109f, 0.109f, 0.109f, 1.0f };      // WindowBg, TransformButtonBg (X/Y/Z)
			BACKGROUND_2  = ImVec4{ 0.08f, 0.08f, 0.08f, 0.940f };       // PopupBg
			ERROR_COLOR   = ImVec4{ 0.609f, 0.109f, 0.109f, 1.0f };      // TransformButtonXActive
			WARNING_COLOR = ImVec4{ 1.0f, 0.5f, 0.0f, 1.0f };            // (not originally present — custom fallback)
			SUCCESS_COLOR = ImVec4{ 0.109f, 0.609f, 0.109f, 1.0f };      // TransformButtonYActive
			X_COLOR       = ImVec4{ 0.409f, 0.109f, 0.109f, 1.0f };      // TransformButtonXHovered
			Y_COLOR       = ImVec4{ 0.109f, 0.409f, 0.109f, 1.0f };      // TransformButtonYHovered
			Z_COLOR       = ImVec4{ 0.109f, 0.109f, 0.409f, 1.0f };      // TransformButtonZHovered
			ApplyThemeToImgui();
		}
		
		inline void LoadDefaultLight() {
			name = "Default Light";
			PRIMARY_1     = ImVec4{ 0.85f, 0.85f, 0.86f, 1.0f };          // HeaderHovered, ButtonHovered
			PRIMARY_2     = ImVec4{ 0.75f, 0.75f, 0.76f, 1.0f };          // HeaderActive, ButtonActive, FrameBgActive
			SECONDARY_1   = ImVec4{ 0.93f, 0.93f, 0.93f, 1.0f };          // Header, TabHovered, TabSelected, AddComponentButton
			SECONDARY_2   = ImVec4{ 0.82f, 0.82f, 0.82f, 1.0f };          // SelectedHeader
			ACCENT_1      = ImVec4{ 0.2f, 0.6f, 0.15f, 1.0f };            // SliderGrabActive (greenish)
			ACCENT_2      = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };             // CheckMark, SliderGrab
			TEXT_1        = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };             // Text (black)
			TEXT_2        = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };             // TextDisabled
			BACKGROUND_1  = ImVec4{ 0.98f, 0.98f, 0.98f, 1.0f };          // WindowBg
			BACKGROUND_2  = ImVec4{ 0.95f, 0.95f, 0.95f, 1.0f };          // PopupBg
			ERROR_COLOR   = ImVec4{ 0.8f, 0.2f, 0.2f, 1.0f };             // TransformButtonXActive
			WARNING_COLOR = ImVec4{ 1.0f, 0.65f, 0.0f, 1.0f };            // Orange
			SUCCESS_COLOR = ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f };             // Green
			X_COLOR       = ImVec4{ 0.8f, 0.2f, 0.2f, 1.0f };             // TransformButtonXHovered
			Y_COLOR       = ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f };             // TransformButtonYHovered
			Z_COLOR       = ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f };             // TransformButtonZHovered
			ApplyThemeToImgui();
		}

		// publicly visible members 
		std::string name;
		ImVec4 PRIMARY_1;
		ImVec4 PRIMARY_2;
		ImVec4 SECONDARY_1;
		ImVec4 SECONDARY_2;
		ImVec4 ACCENT_1;
		ImVec4 ACCENT_2;
		ImVec4 TEXT_1;
		ImVec4 TEXT_2;
		ImVec4 BACKGROUND_1;
		ImVec4 BACKGROUND_2;
		ImVec4 ERROR_COLOR;
		ImVec4 WARNING_COLOR;
		ImVec4 SUCCESS_COLOR;
		ImVec4 X_COLOR;
		ImVec4 Y_COLOR;
		ImVec4 Z_COLOR;

	private:
		std::stack<std::pair<ImGuiCol_, ImVec4>> m_ColorStack;
		void ApplyThemeToImgui();

		std::pair<bool, std::string> SerializeToYAML(const std::filesystem::path& filepath);
		std::pair<bool, std::string> DeserializeFromYAML(const std::filesystem::path& filepath);
	};
};

template <>
struct YAML::convert<ImVec4> {
	static YAML::Node encode(const ImVec4& rhs) {
		YAML::Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		return node;
	}

	static bool decode(const YAML::Node& node, ImVec4& rhs) {
		if (!node.IsSequence() || node.size() != 4) {
			return false;
		}
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};

template <>
struct YAML::convert<Laura::EditorTheme> {
	static YAML::Node encode(const Laura::EditorTheme& rhs) {
		YAML::Node node;
		node["name"]            = rhs.name;
		node["PRIMARY_1"]       = rhs.PRIMARY_1;
		node["PRIMARY_2"]       = rhs.PRIMARY_2;
		node["SECONDARY_1"]     = rhs.SECONDARY_1;
		node["SECONDARY_2"]     = rhs.SECONDARY_2;
		node["ACCENT_1"]        = rhs.ACCENT_1;
		node["ACCENT_2"]        = rhs.ACCENT_2;
		node["TEXT_1"]          = rhs.TEXT_1;
		node["TEXT_2"]          = rhs.TEXT_2;
		node["BACKGROUND_1"]    = rhs.BACKGROUND_1;
		node["BACKGROUND_2"]    = rhs.BACKGROUND_2;
		node["ERROR_COLOR"]     = rhs.ERROR_COLOR;
		node["WARNING_COLOR"]   = rhs.WARNING_COLOR;
		node["SUCCESS_COLOR"]   = rhs.SUCCESS_COLOR;
		node["X_COLOR"]         = rhs.X_COLOR;
		node["Y_COLOR"]         = rhs.Y_COLOR;
		node["Z_COLOR"]         = rhs.Z_COLOR;
		return node;
	}

	static bool decode(const YAML::Node& node, Laura::EditorTheme& rhs) {
		if (!node.IsMap()) {
			return false;
		}
		if (node["name"])           rhs.name           = node["name"].as<std::string>();
		if (node["PRIMARY_1"])      rhs.PRIMARY_1      = node["PRIMARY_1"].as<ImVec4>();
		if (node["PRIMARY_2"])      rhs.PRIMARY_2      = node["PRIMARY_2"].as<ImVec4>();
		if (node["SECONDARY_1"])    rhs.SECONDARY_1    = node["SECONDARY_1"].as<ImVec4>();
		if (node["SECONDARY_2"])    rhs.SECONDARY_2    = node["SECONDARY_2"].as<ImVec4>();
		if (node["ACCENT_1"])       rhs.ACCENT_1       = node["ACCENT_1"].as<ImVec4>();
		if (node["ACCENT_2"])       rhs.ACCENT_2       = node["ACCENT_2"].as<ImVec4>();
		if (node["TEXT_1"])         rhs.TEXT_1         = node["TEXT_1"].as<ImVec4>();
		if (node["TEXT_2"])         rhs.TEXT_2         = node["TEXT_2"].as<ImVec4>();
		if (node["BACKGROUND_1"])   rhs.BACKGROUND_1   = node["BACKGROUND_1"].as<ImVec4>();
		if (node["BACKGROUND_2"])   rhs.BACKGROUND_2   = node["BACKGROUND_2"].as<ImVec4>();
		if (node["ERROR_COLOR"])    rhs.ERROR_COLOR    = node["ERROR_COLOR"].as<ImVec4>();
		if (node["WARNING_COLOR"])  rhs.WARNING_COLOR  = node["WARNING_COLOR"].as<ImVec4>();
		if (node["SUCCESS_COLOR"])  rhs.SUCCESS_COLOR  = node["SUCCESS_COLOR"].as<ImVec4>();
		if (node["X_COLOR"])        rhs.X_COLOR        = node["X_COLOR"].as<ImVec4>();
		if (node["Y_COLOR"])        rhs.Y_COLOR        = node["Y_COLOR"].as<ImVec4>();
		if (node["Z_COLOR"])        rhs.Z_COLOR        = node["Z_COLOR"].as<ImVec4>();
		return true;
	}
};