#pragma once

#include <imgui.h>

namespace Laura
{
	struct ImGuiContextFontRegistry {
		ImFont* highResIcons = nullptr;
		ImFont* notoSansRegular = nullptr;
		ImFont* notoSansBold = nullptr;
	};

	inline ImGuiContextFontRegistry* Fonts() {
		return static_cast<ImGuiContextFontRegistry*>(ImGui::GetIO().UserData);
	}
}
