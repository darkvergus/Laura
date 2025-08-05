#include "EditorUI/AssetsPanel/AssetsPanel.h"
#include "EditorUI/UtilityUI.h"
#include "ImGuiContext.h"
#include <format>

namespace Laura 
{

	void AssetsPanel::OnImGuiRender() {
        EditorTheme& theme = m_EditorState->temp.editorTheme;
        ImGui::Begin(ICON_FA_CUBE " Assets");

        if (!m_ProjectManager->ProjectIsOpen()) {
            ImGui::End();
            return;
        }
        auto assetManager = m_ProjectManager->GetAssetManager();
        auto assetPool = assetManager->GetAssetPool();

        theme.PushColor(ImGuiCol_Button, EditorCol_Secondary2);
        if (ImGui::Button(ICON_FA_PLUS " Add")) {
			ImGui::OpenPopup("Add Menu");
		}

        if (ImGui::IsPopupOpen("Add Menu")) {
            ImVec2 addButtonPos = ImGui::GetItemRectMin();
            ImVec2 addButtonSize = ImGui::GetItemRectSize();
            ImGui::SetNextWindowSizeConstraints(
                ImVec2(FLT_MIN, FLT_MIN),
                ImVec2(FLT_MAX, 300.0f) // if the popup contains too many compnents, adds a scrollbar
            );
            ImGui::SetNextWindowPos(ImVec2(addButtonPos.x, addButtonPos.y + addButtonSize.y));
            ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));
        }

		if (ImGui::BeginPopup("Add Menu", ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::MenuItem("Scene")) {
                m_ProjectManager->GetSceneManager()->CreateScene();
            }
            if (ImGui::MenuItem("Asset...")) {
				OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
				char buff[MAX_PATH] = {}; 
				ofn.lpstrFilter = "All Files\0*.*\0";
				ofn.lpstrTitle = "Select Asset:";
				ofn.nMaxFile = sizeof(buff);
				ofn.lpstrFile = buff;
				if (GetOpenFileNameA(&ofn)) {
					LR_GUID guid = assetManager->ImportAsset(buff);
				}
            }
			ImGui::EndPopup();
		}
        theme.PopColor(); 

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 5.0f);
        ImGui::SliderFloat("##TileSize", &m_TileScalar, TILE_SCALAR_MIN, TILE_SCALAR_MAX, ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER);
        
        if (ImGui::BeginTable("##AssetTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings)) {
            ImGui::TableNextColumn();
            ImGui::BeginChild("LeftPanel", ImVec2(0, ImGui::GetContentRegionAvail().y), false);
            {
                float horizontalSpacing = 15.0f;
                float verticalSpacing = 3.0f;
                ImGuiStyle& style = ImGui::GetStyle();
                float column_x1 = ImGui::GetCursorScreenPos().x; // left border x
                float column_x2 = column_x1 + ImGui::GetColumnWidth(); // right border x
                ImGui::Indent(horizontalSpacing);
                // draw scene tiles
                for (const auto& [guid, scene] : *m_ProjectManager->GetSceneManager()) {
                    DrawSceneTile(guid, scene->GetName().c_str());
                    float last_assetTile_x2 = ImGui::GetItemRectMax().x;
                    if (last_assetTile_x2 + ImGui::GetItemRectSize().x < column_x2) {
                        ImGui::SameLine(0, horizontalSpacing);
                    }
                    else {
                        ImGui::Dummy({0.0f, verticalSpacing});
                    }
                }
                // draw asset tiles
                for (const auto& [guid, metadataPair] : assetPool->Metadata) {
                    const auto& [metadata, metadataExtension] = metadataPair;
                    std::string filename = metadataExtension->sourcePath.filename().string();
                    DrawAssetTile(guid, filename.c_str());
                    float last_assetTile_x2 = ImGui::GetItemRectMax().x;
                    if (last_assetTile_x2 + ImGui::GetItemRectSize().x < column_x2) {
                        ImGui::SameLine(0, horizontalSpacing);
                    }
                    else {
                        ImGui::Dummy({0.0f, verticalSpacing});
                    }
                }
                ImGui::Unindent();
            }
            ImGui::EndChild();

            ImGui::TableNextColumn();
            DrawTileInfo();
            ImGui::EndTable();
        }
        ImGui::End();
    }
    
    void AssetsPanel::DrawSceneTile(LR_GUID guid, const char* title) {
		DrawGenericTile(guid, title, ICON_FA_CUBES_STACKED, DNDPayloadTypes::SCENE);
	}

    void AssetsPanel::DrawAssetTile(LR_GUID guid, const char* title) {
		if (!m_ProjectManager->ProjectIsOpen()) {
			return;
		}

		auto assetPool = m_ProjectManager->GetAssetManager()->GetAssetPool();
		const char* icon = nullptr;
		const char* dndPayloadType = nullptr;

		if (assetPool->find<MeshMetadata>(guid) != nullptr) {
			icon = ICON_FA_CUBE;
			dndPayloadType = DNDPayloadTypes::MESH;
		}
		else if (assetPool->find<TextureMetadata>(guid) != nullptr) {
			icon = ICON_FA_IMAGE;
			dndPayloadType = DNDPayloadTypes::TEXTURE;
		}
		else {
			return; // skip unknown asset types
		}

		DrawGenericTile(guid, title, icon, dndPayloadType);
	}
    
    void AssetsPanel::DrawGenericTile(LR_GUID guid, const char* title, const char* icon, const char* dndPayloadType) {
		ImGui::PushID((uint64_t)guid);
		EditorTheme& theme = m_EditorState->temp.editorTheme;
		ImDrawList* drawlist = ImGui::GetWindowDrawList();

		// Deselect on click outside a tile
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			m_SelectedTile = LR_GUID::INVALID;
		}

		// Render the Selectable
		EditorCol_ tileBg = (m_SelectedTile == guid) ? EditorCol_Secondary2 : EditorCol_Primary3;
		theme.PushColor(ImGuiCol_Header, tileBg);
		if (ImGui::Selectable("##tile", true, ImGuiSelectableFlags_None, { m_TileScalar * BASE_TILE_WH_RATIO, m_TileScalar })) {
			m_SelectedTile = guid;
        }
        theme.PopColor();

        // Drag and drop source
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            m_DNDPayload.guid = guid;
            strncpy(m_DNDPayload.title, title, sizeof(m_DNDPayload.title));
            m_DNDPayload.title[sizeof(m_DNDPayload.title) - 1] = '\0';

            ImGui::SetDragDropPayload(dndPayloadType, &m_DNDPayload, sizeof(DNDPayload));
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s %s", icon, title);
            theme.PopColor();
            ImGui::EndDragDropSource();
        }

        ImVec2 tileCoordsTopLeft = ImGui::GetItemRectMin();
        ImVec2 tileCoordsBottomRight = ImGui::GetItemRectMax();
        ImVec2 tileDims = ImGui::GetItemRectSize();
        if (m_SelectedTile != guid) {
            drawlist->PushClipRect(tileCoordsTopLeft, tileCoordsBottomRight, true);
        }
         
        // Render Icon
        auto* fontRegistry = static_cast<ImGuiContextFontRegistry*>(ImGui::GetIO().UserData);
		if (fontRegistry && fontRegistry->HighResIcons) {
            ImFont* font = fontRegistry->HighResIcons;
			float fontSize = BASE_TILE_ICON_FONT_SIZE * m_TileScalar;
			ImVec2 iconDims = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, icon);
			ImVec2 iconPos = {
				tileCoordsTopLeft.x + (tileDims.x - iconDims.x) * 0.5f,
				tileCoordsTopLeft.y + (tileDims.x - iconDims.y) * 0.5f
			};
            drawlist->AddText(
                font, 
                fontSize, 
                iconPos, 
                ImGui::GetColorU32(theme[EditorCol_Text1]), 
                icon
            );
		}
        
        float margin = 5.0f;
        // Render Title with wrapping
        ImVec2 titlePos = {
            floor(tileCoordsTopLeft.x + margin),
            floor(tileCoordsTopLeft.y + tileDims.y * BASE_TILE_WH_RATIO)
        };
        float wrapWidth = tileDims.x - margin*2;
        drawlist->AddText(
            ImGui::GetFont(),
            BASE_TILE_TITLE_FONT_SIZE,
            titlePos,
            ImGui::GetColorU32(theme[EditorCol_Text1]),
            title,
            nullptr,
            wrapWidth
        );

        if (m_SelectedTile != guid) {
            drawlist->PopClipRect();
        }

        ImGui::PopID();
    }

    void AssetsPanel::DrawTileInfo() {
        if (!m_ProjectManager->ProjectIsOpen()) {
            return;
        }
        auto assetPool = m_ProjectManager->GetAssetManager()->GetAssetPool();
        auto sceneManager = m_ProjectManager->GetSceneManager();

        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x);
        EditorTheme& theme = m_EditorState->temp.editorTheme;

        auto DrawLabelValue = [&theme](const char* label, auto value) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s", label);
            theme.PopColor();
            ImGui::SameLine(150);
            ImGui::Text("%s", std::format("{}", value).c_str());
            };

        auto DrawLabelInput = [&theme](const char* label, std::string& value) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s", label);
            theme.PopColor();
            ImGui::SameLine(150);
            std::string inputId = std::string("##inputText") + label;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, value.c_str());
            ImGui::AlignTextToFramePadding();
            if (ImGui::InputText(inputId.c_str(), buffer, sizeof(buffer))) {
                value = std::string(buffer);
            }
            };

        // No asset selected
        if (m_SelectedTile == LR_GUID::INVALID) {
            return;
        }
        
        // shouldDetete = true; on btn click
        auto DrawRightAlignedDeleteBtn = [&](bool& shouldDelete) {
            const char* icon = ICON_FA_TRASH_CAN;
            ImVec2 textSize = ImGui::CalcTextSize(icon);
            ImVec2 buttonSize = {
                textSize.x + ImGui::GetStyle().FramePadding.x * 2.0f,
                textSize.y + ImGui::GetStyle().FramePadding.y * 2.0f
            };
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonSize.x);
            if (ImGui::Button(ICON_FA_TRASH_CAN)) {
                shouldDelete = true;
            }
        };

        if (std::shared_ptr<Scene> scene = sceneManager->find(m_SelectedTile)) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
            ImGui::Text("General");
            theme.PopColor();
            ImGui::AlignTextToFramePadding();
            ImGui::SameLine();
            static bool shouldDeleteScene = false;
            DrawRightAlignedDeleteBtn(shouldDeleteScene);
            ConfirmAndExecute(shouldDeleteScene, ICON_FA_TRASH_CAN " Delete Scene", "Are you sure you want to delete this scene?", [&]() {
                sceneManager->DeleteScene(scene->GetGuid());
                m_SelectedTile = LR_GUID::INVALID;
			}, m_EditorState);

            DrawLabelInput("Rename:", scene->GetMutableName());
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("Open on Boot:");
            theme.PopColor();
            ImGui::SameLine(150.0f);
            bool isBoot = m_ProjectManager->IsBootScene(scene->GetGuid());
            if (ImGui::Checkbox("##bootSceneCheckbox", &isBoot)){
                if (isBoot) {
                    m_ProjectManager->SetBootSceneGuid(scene->GetGuid());
                }
                else {
                    m_ProjectManager->SetBootSceneGuid(LR_GUID::INVALID);
                }
            }

            ImGui::Separator();
            ImGui::Dummy({ 0.0f, 5.0f });
            theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
            ImGui::Text("Scene Info");
            theme.PopColor();
            DrawLabelValue("Guid:", scene->GetGuid().string().c_str());

        }
        else if (const auto& it = assetPool->Metadata.find(m_SelectedTile); it != assetPool->Metadata.end()) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
            ImGui::Text("General");
            theme.PopColor();
            ImGui::AlignTextToFramePadding();
            ImGui::SameLine();
            static bool shouldDeleteAsset = false;
            DrawRightAlignedDeleteBtn(shouldDeleteAsset);
            ConfirmAndExecute(shouldDeleteAsset, ICON_FA_TRASH_CAN " Delete Asset", "Are you sure you want to delete this asset?", [&]() {
                // TODO delete asset  assetManager->DeleteAsset(guid);
                m_SelectedTile = LR_GUID::INVALID;
			}, m_EditorState);

            const auto& [metadata, metadataExtension] = it->second;
            DrawLabelValue("Source Path:", metadataExtension->sourcePath.string());

            // File size formatter (GB > MB > KB > B)
            auto ConvertFileSize = [](uintmax_t fileSizeInBytes) -> std::string {
                constexpr double KB = 1024.0;
                constexpr double MB = 1024.0 * KB;
                constexpr double GB = 1024.0 * MB;

                if (fileSizeInBytes >= static_cast<uintmax_t>(GB)) {
                    return std::format("{:.2f} GB", fileSizeInBytes / GB);
                }
                else if (fileSizeInBytes >= static_cast<uintmax_t>(MB)) {
                    return std::format("{:.2f} MB", fileSizeInBytes / MB);
                }
                else if (fileSizeInBytes >= static_cast<uintmax_t>(KB)) {
                    return std::format("{:.2f} KB", fileSizeInBytes / KB);
                }
                else {
                    return std::format("{} Bytes", fileSizeInBytes);
                }
            };

            DrawLabelValue("File Size:", ConvertFileSize(metadataExtension->fileSizeInBytes));
            DrawLabelValue("Load Time:", std::format("{:.2f} ms", metadataExtension->loadTimeMs));
            ImGui::Separator();
            ImGui::Dummy({ 0.0f, 5.0f });
            // AssetType specific data

            // Try cast to MeshMetadata
            if (auto meshMetadata = dynamic_cast<MeshMetadata*>(metadata.get())) {
                theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
                ImGui::Text("Mesh Metadata");
                theme.PopColor();
                DrawLabelValue("Triangle Count", meshMetadata->TriCount);
                DrawLabelValue("FirstTriIdx:", meshMetadata->firstTriIdx);
                ImGui::Dummy({ 0, 3.0f });
                DrawLabelValue("BVH Node Count", meshMetadata->nodeCount);
                DrawLabelValue("BVH FirstNodeIdx:", meshMetadata->firstNodeIdx);
            }

            // Try cast to TextureMetadata
            else if (auto texMetadata = dynamic_cast<TextureMetadata*>(metadata.get())) {
                theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
                ImGui::Text("Texture Metadata");
                theme.PopColor();
                DrawLabelValue("Width:", texMetadata->width);
                DrawLabelValue("Height:", texMetadata->height);
                DrawLabelValue("Channels:", texMetadata->channels);
            }
        }
        else {
            theme.PushColor(ImGuiCol_Text, EditorCol_Error);
            ImGui::Text("[ERROR] Invalid Asset guid");
            theme.PopColor();
        }

        ImGui::PopTextWrapPos();
    }
}