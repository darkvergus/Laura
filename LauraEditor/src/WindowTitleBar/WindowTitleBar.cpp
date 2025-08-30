#include <imgui.h>
#include <IconsFontAwesome6.h>
#include <Codicons.h>
#include "WindowTitleBar.h"
#include "Dialogs/ConfirmationDialog.h"
#include "Project/Scene/SceneManager.h"
#include "Dialogs/ProjectDialogs/ProjectDialogs.h"
#include "ImGuiContextFontRegistry.h"

namespace Laura
{
	void WindowTitleBar::OnImGuiRender() {
		auto& theme = m_EditorState->temp.editorTheme;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 8.0f));

		m_TitleBarHeight = ImGui::GetFrameHeight();
		const float titlebarWidth = ImGui::GetContentRegionAvail().x;
		float titleBarOffset = 0.0f;
		
		if (m_Window->isMaximized()) {
			titleBarOffset = 6.0f;
		}

		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(vp->Pos.x, vp->Pos.y + titleBarOffset));
		ImGui::SetNextWindowSize(ImVec2(vp->Size.x, m_TitleBarHeight));
		ImGui::SetNextWindowViewport(vp->ID);
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
		ImGui::Begin("##Titlebar", nullptr, flags);

		// Background
		{
			ImDrawList* dl = ImGui::GetWindowDrawList();
			ImVec2 p0 = ImGui::GetWindowPos();
			ImVec2 p1 = ImVec2(p0.x + ImGui::GetWindowSize().x, p0.y + ImGui::GetWindowSize().y);
			dl->AddRectFilled(p0, p1, ImGui::GetColorU32(ImVec4(0.10f, 0.10f, 0.11f, 1.0f)));
		}

		const float frameH = ImGui::GetFrameHeight();
		const float btnH = frameH - 2.0f;
		const ImVec2 winSize = ImGui::GetWindowSize();
		const float rightButtonsWidth = btnH * 3.0f + ImGui::GetStyle().ItemSpacing.x * 2.0f;
	
		// Begin single-row menu bar container
		if (ImGui::BeginMenuBar()) {
			// 1) App icon placeholder
			ImVec2 iconSize(btnH, btnH);
			ImGui::InvisibleButton("##AppIcon", iconSize);
			{
				ImVec2 min = ImGui::GetItemRectMin();
				ImVec2 max = ImGui::GetItemRectMax();
				ImGui::GetWindowDrawList()->AddRectFilled(min, max, ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.27f, 0.0f)), 4.0f);
			}
			ImGui::SameLine();

			// 2) Menus (File, View, ...)
			auto ItemLabel = [](const std::string& icon, const std::string& label) -> std::string {
				if (icon.empty()) return std::string("         ") + label;
				return std::string("   ") + icon + "  " + label;
				};
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3, 3));
			float originalBorder = ImGui::GetStyle().PopupBorderSize;
			ImGui::GetStyle().PopupBorderSize = 0.0f;
			theme.PushColor(ImGuiCol_PopupBg, EditorCol_Background3);
			ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(FLT_MAX, FLT_MAX));
			theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
			if (ImGui::BeginMenu("File")) {
				theme.PushColor(ImGuiCol_HeaderHovered, EditorCol_Accent2);
				theme.PushColor(ImGuiCol_Text, EditorCol_Text1);
				if (ImGui::MenuItem(ItemLabel(ICON_FA_FILE_CIRCLE_PLUS, "New").c_str(), "Ctrl+N")) { m_EditorState->temp.isCreateProjectDialogOpen = true; }
				if (ImGui::MenuItem(ItemLabel(ICON_FA_FOLDER_CLOSED, "Open...").c_str(), "Ctrl+O")) { m_ShouldOpenProject = true; }
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel(ICON_FA_FLOPPY_DISK, "Save").c_str(), "Ctrl+S")) { m_ProjectManager->SaveProject(); }
				ImGui::MenuItem(ItemLabel("", "Save As...").c_str());
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel(ICON_FA_ARROW_UP_FROM_BRACKET, "Export...").c_str())) { m_EditorState->temp.shouldOpenExportPanel = true; }
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel("", "Close").c_str())) { m_ShouldCloseProject = true; }
				theme.PopColor(2); // text, headerHovered
				ImGui::EndMenu();
			}
			ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(FLT_MAX, FLT_MAX));
			if (ImGui::BeginMenu("View")) {
				theme.PushColor(ImGuiCol_HeaderHovered, EditorCol_Accent2);
				theme.PushColor(ImGuiCol_Text, EditorCol_Text1);
				if (ImGui::BeginMenu(ItemLabel(ICON_FA_TABLE_LIST, "Layouts").c_str())) {
					if (ImGui::MenuItem(ItemLabel("", "Load Default Layout").c_str())) {
						m_ImGuiContext->LoadDefaultLayout();
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel(ICON_FA_STOPWATCH, "Profiler").c_str(), NULL, false, !m_EditorState->temp.isProfilerPanelOpen)) {
					m_EditorState->temp.isProfilerPanelOpen = true;
				}
				if (ImGui::MenuItem(ItemLabel("", "Themes").c_str(), NULL, false, !m_EditorState->temp.isThemePanelOpen)) {
					m_EditorState->temp.isThemePanelOpen = true;
				}
				theme.PopColor(2); // text, headerHovered
				ImGui::EndMenu();
			}
			ImGui::GetStyle().PopupBorderSize = originalBorder;
			ImGui::PopStyleVar();
			theme.PopColor(2); // PopupBg text2

			// 3) Centered Play button
			if (m_ProjectManager->ProjectIsOpen()) {
				float currentX = ImGui::GetCursorPosX();
				const char* icon = m_EditorState->temp.isInRuntimeSimulation ? ICON_FA_SQUARE : ICON_FA_PLAY;
				ImVec2 textSize = ImGui::CalcTextSize(icon);
				float centerX = (winSize.x * 0.5f) - (textSize.x * 0.5f);
				if (centerX > currentX) ImGui::SetCursorPosX(centerX);
				auto btnIconCol = m_EditorState->temp.isInRuntimeSimulation ? EditorCol_Warning : EditorCol_Text1;
				theme.PushColor(ImGuiCol_Button, EditorCol_Transparent, 0.0f);
				theme.PushColor(ImGuiCol_Text, btnIconCol);
				if (ImGui::Button(icon)) {
					m_EditorState->temp.isInRuntimeSimulation = !m_EditorState->temp.isInRuntimeSimulation;
					if (m_EditorState->temp.isInRuntimeSimulation) {
						m_ProjectManager->GetSceneManager()->EnterRuntimeSimulation();
						m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_ProjectManager->GetMutableRuntimeRenderSettings()));
					}
					else {
						m_ProjectManager->GetSceneManager()->ExitRuntimeSimulation();
						m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_EditorState->persistent.editorRenderSettings));
					}
				}
				theme.PopColor(2);
			}

			// 4) Breadcrumbs (project > scene) aligned before right buttons
			if (m_ProjectManager->ProjectIsOpen()) {
				std::string project = m_ProjectManager->GetProjectName();
				std::string scene; bool hasScene = false;
				if (auto sceneMgr = m_ProjectManager->GetSceneManager()) {
					if (auto sc = sceneMgr->GetOpenScene()) { scene = sc->name; hasScene = true; }
				}
				ImGui::PushFont(Fonts()->notoSansBold);
				float projectWidth = ImGui::CalcTextSize(project.c_str()).x;
				float iconWidth = hasScene ? ImGui::CalcTextSize(ICON_FA_CARET_RIGHT).x : 0;
				float sceneWidth = hasScene ? ImGui::CalcTextSize(scene.c_str()).x : 0;
				float totalWidth = projectWidth + iconWidth + sceneWidth + (hasScene ? 4.0f : 0);
				ImGui::SetCursorPosX(winSize.x - rightButtonsWidth - totalWidth - 6.0f);
				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::TextUnformatted(project.c_str());
				if (hasScene) {
					ImGui::SameLine(0, 2.0f);
					ImVec2 pos = ImGui::GetCursorPos();
					ImGui::SetCursorPosY(pos.y + 1.0f);
					ImGui::TextUnformatted(ICON_FA_CARET_RIGHT);
					ImGui::SetCursorPosY(pos.y);
					ImGui::SameLine(0, 2.0f);
					ImGui::TextUnformatted(scene.c_str());
				}
				theme.PopColor();
				ImGui::PopFont();
			}
			ImGui::SetCursorPosX(winSize.x - rightButtonsWidth);
			theme.PushColor(ImGuiCol_Button, EditorCol_Transparent, 0.0f);
			// Minimize
			ImGui::PushFont(Fonts()->codicon);
			if (ImGui::Button(CODICON_CHROME_MINIMIZE, ImVec2(btnH, btnH))) { 
				m_Window->minimize();
			}
			ImGui::SameLine(0,0);

			// Maximize / Restore
			const char* icon = m_Window->isMaximized() ? CODICON_CHROME_RESTORE : CODICON_CHROME_MAXIMIZE;
			if (ImGui::Button(icon, ImVec2(btnH, btnH))) {
				if (m_Window->isMaximized()) {
					m_Window->restore();
				}
				else m_Window->maximize();
			}
			ImGui::SameLine(0,0);

			// Close
			theme.PushColor(ImGuiCol_ButtonHovered, EditorCol_Error);
			if (ImGui::Button(CODICON_CHROME_CLOSE, ImVec2(btnH, btnH))) { 
				m_Window->close(); 
			}
			theme.PopColor();
			ImGui::PopFont();
			theme.PopColor();

			ImGui::EndMenuBar();
		}
		ImGui::End();
		ImGui::PopStyleVar(2);

		float titlebarHeight = m_TitleBarHeight;
		m_Window->setTitlebarHitTestCallback([titlebarHeight, titlebarWidth](int width, int height) -> bool {
				if (ImGui::IsAnyItemHovered()) { return false; }
				if ((width >= 0 && width <= titlebarWidth) && (height >= 0 && height <= titlebarHeight)){
					return true;
				}
			}
		);

		// dialogs
		CreateProjectDialog(m_ProjectManager, m_EditorState);
		ConfirmWithCancel(
			m_ShouldOpenProject,
			ICON_FA_FOLDER_CLOSED " Open",
			"Save current project before opening?",
			ICON_FA_FLOPPY_DISK " Save & Open",
			"Open",
			"Cancel",
			[&]() { m_ProjectManager->SaveProject(); },
			[&]() {},
			m_EditorState,
			[&]() { OpenProjectDialog(m_ProjectManager); }
		);
		ConfirmWithCancel(
			m_ShouldCloseProject,
			ICON_FA_DIAGRAM_PROJECT " Close Project",
			"Save project before closing?",
			ICON_FA_FLOPPY_DISK " Save & Close",
			"Close",
			"Cancel",
			[&]() { m_ProjectManager->SaveProject(); },
			[&]() {},
			m_EditorState,
			[&]() { m_ProjectManager->CloseProject(); }
		);
	}
 }
