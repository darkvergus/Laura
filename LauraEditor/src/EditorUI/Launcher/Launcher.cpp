#include <IconsFontAwesome6.h>
#include "Launcher.h"


namespace Laura
{

	std::optional<std::filesystem::path> Launcher::ShowFolderPickerDialog() {
		IFileDialog* pDialog = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
									  IID_IFileDialog, reinterpret_cast<void**>(&pDialog));
		if (FAILED(hr) || !pDialog) {
			return std::nullopt;
		}
		// Tell the dialog to pick folders
		DWORD options;
		pDialog->GetOptions(&options);
		pDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
		pDialog->SetTitle(L"Select Project Folder");
		// Show the dialog
		hr = pDialog->Show(nullptr);
		if (FAILED(hr)) {
			pDialog->Release();
			return std::nullopt;
		}
		// Get the result
		IShellItem* pItem = nullptr;
		hr = pDialog->GetResult(&pItem);
		if (FAILED(hr) || !pItem) {
			pDialog->Release();
			return std::nullopt;
		}
		PWSTR pszPath = nullptr;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
		std::optional<std::filesystem::path> result;

		if (SUCCEEDED(hr)) {
			result = std::filesystem::path(pszPath);
			CoTaskMemFree(pszPath);
		}
		pItem->Release();
		pDialog->Release();
		return result;
	}

	void Launcher::OnImGuiRender() {
		auto& theme = m_EditorState->temp.editorTheme;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		theme.PushColor(ImGuiCol_WindowBg, EditorCol_Background1);
		theme.PushColor(ImGuiCol_Button, EditorCol_Secondary1);
		ImGui::Begin("##Launcher", nullptr, flags);

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		float buttonWidth = windowSize.x / 8.0f;
		float buttonHeight = 40.0f;
		float spacingY = 16.0f;
		// center vertical
		float totalHeight = buttonHeight * 2 + spacingY;
		ImGui::SetCursorPosY((windowSize.y - totalHeight) / 2.0f);
		// center horizontal
		float centerX = (windowSize.x - buttonWidth) / 2.0f;
		ImGui::SetCursorPosX(centerX);

		if (ImGui::Button("New Project   " ICON_FA_CUBE, ImVec2(buttonWidth, buttonHeight))) {
			m_CreateProjectWindowOpen = true;
		}
		if (m_CreateProjectWindowOpen) {
			DrawCreateProjectWindow(); // resets the flag on close 
		}

		ImGui::Spacing();
		ImGui::SetCursorPosX(centerX);
		if (ImGui::Button("Open Project   " ICON_FA_SHARE, ImVec2(buttonWidth, buttonHeight))) {
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = {};
			ofn.lpstrFilter = PROJECT_FILE_EXTENSION "Files\0 * " PROJECT_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Select Project File:";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			if (GetOpenFileNameA(&ofn)) {
				std::filesystem::path projectfilePath = buff;
				if (m_ProjectManager->OpenProject(projectfilePath)) {
					m_CreateProjectWindowOpen = false;
				}
			}
		}

		ImGui::End();
		theme.PopColor(2);
		ImGui::PopStyleVar(2);
	}

	void Launcher::DrawCreateProjectWindow(){
		auto& theme = m_EditorState->temp.editorTheme;
		theme.PushColor(ImGuiCol_WindowBg, EditorCol_Secondary2);
		theme.PushColor(ImGuiCol_TitleBg, EditorCol_Secondary1);
		theme.PushColor(ImGuiCol_TitleBgActive, EditorCol_Secondary1);
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 150), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::Begin("Create Project " ICON_FA_CUBE, &m_CreateProjectWindowOpen);

		float margin = 3.0f;
		static char projectName[250] = "";
		static std::filesystem::path folderpath = "";
		static bool folderpathSelected = false;

		// Project Name
		ImGui::Text("Project name");
		ImGui::SameLine(150.0f);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - margin);
		ImGui::InputTextWithHint("##projectName-input", "Project name", projectName, IM_ARRAYSIZE(projectName));


		// Folder Path Button (acts as label + picker)
		ImGui::Text("Location");
		ImGui::SameLine(150.0f);
		std::string buttonLabel = folderpath.empty()
			? "Location"
			: folderpath.string();
		std::string buttonId = "##folder-button";
		EditorCol_ textCol = folderpathSelected ? EditorCol_Text1 : EditorCol_Text2;
		theme.PushColor(ImGuiCol_Text, textCol);
		theme.PushColor(ImGuiCol_Button, EditorCol_Background1);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2{ 0.0f, 1.0f });
		if (ImGui::Button((buttonLabel + buttonId).c_str(), {ImGui::GetContentRegionAvail().x - margin, 0/*keep original*/})) {
			if (auto newPath = ShowFolderPickerDialog()) {
				folderpath = *newPath;
				folderpathSelected = true;
			}
		}
		ImGui::PopStyleVar();
		theme.PopColor(2);

		float createProjectBtnHeight = 30.0f;
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImGui::Dummy({ 0.0f, (windowSize.y - createProjectBtnHeight) / 2.0f });

		// center horizontal
		std::string createProjectLabel = "Create Project " ICON_FA_SHARE;
		float centerX = (windowSize.x - ImGui::CalcTextSize(createProjectLabel.c_str()).x) / 2.0f;
		ImGui::SetCursorPosX(centerX);
		if (ImGui::Button(createProjectLabel.c_str(), {0, createProjectBtnHeight})) {
			auto projectPath = folderpath / projectName;
			if (m_ProjectManager->NewProject(projectPath)) {
				m_CreateProjectWindowOpen = false;
			}
		}

		ImGui::End();
		theme.PopColor(3);
	}
}
