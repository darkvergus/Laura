#include "EditorUI\AssetsPanel\AssetsPanel.h"


namespace Laura {
	void AssetsPanel::OnImGuiRender() {
        EditorTheme& theme = m_EditorState->temp.editorTheme;

        ImGui::Begin(ICON_FA_CUBES " Assets");

        theme.PushColor(ImGuiCol_Button, EditorCol_Secondary2);
        if (ImGui::Button("Import..")) {
            OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
            char buff[MAX_PATH] = {};
            ofn.lpstrFilter = "All Files\0*.*\0";
            ofn.lpstrTitle = "Select Theme:";
            ofn.nMaxFile = sizeof(buff);
            ofn.lpstrFile = buff;
            if (GetOpenFileNameA(&ofn)) {
                LR_GUID guid = m_AssetManager->LoadAsset(buff);
            }
        }
        theme.PopColor(); 

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 5.0f);
        ImGui::SliderFloat("##TileSize", &m_TileScalar, TILE_SCALAR_MIN, TILE_SCALAR_MAX, ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER);
        
        if (ImGui::BeginTable("##AssetTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings)) {
            ImGui::TableNextColumn();
            ImGui::BeginChild("LeftPanel", ImVec2(0, ImGui::GetContentRegionAvail().y), false);
            {
                float manualSpacing = 15.0f;
                ImGuiStyle& style = ImGui::GetStyle();
                float column_x1 = ImGui::GetCursorScreenPos().x; // left border x
                float column_x2 = column_x1 + ImGui::GetColumnWidth(); // right border x
                ImGui::Indent(manualSpacing);
                for (const auto& [guid, metadataPair] : m_ResourcePool->Metadata) {
                    const auto& [metadata, metadataExtension] = metadataPair;
                    const std::filesystem::path& assetPath = metadataExtension->sourcePath;
                    DrawAssetTile(guid, ICON_FA_CUBE, assetPath.filename().string().c_str());
                    float last_assetTile_x2 = ImGui::GetItemRectMax().x;
                

                    if (last_assetTile_x2 + ImGui::GetItemRectSize().x < column_x2)
                        ImGui::SameLine(0, manualSpacing);
                }
                ImGui::Unindent();
            }
            ImGui::EndChild();


            ImGui::TableNextColumn();
            static LR_GUID m_SelectedAsset = LR_GUID::INVALID;
            if (m_SelectedAsset != LR_GUID::INVALID) {
                ImGui::Text("Asset selected.");
            } else {
                ImGui::Text("No asset selected.");
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }


    void AssetsPanel::DrawAssetTile(LR_GUID guid, const char* icon, const char* title) {
        ImGui::PushID((uint64_t)guid);
        EditorTheme& theme = m_EditorState->temp.editorTheme;
        ImDrawList* drawlist = ImGui::GetWindowDrawList();

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_SelectedTile = LR_GUID::INVALID;
		} 
        // Render the Selectable
        theme.PushColor(ImGuiCol_Header, EditorCol_Secondary2);
        if (ImGui::Selectable("##tile", m_SelectedTile == guid, ImGuiSelectableFlags_None, { m_TileScalar*BASE_TILE_WH_RATIO, m_TileScalar})) {
            m_SelectedTile = guid;
        }
        theme.PopColor();


        // Render ICON
        ImVec2 tileCoordsTopLeft = ImGui::GetItemRectMin();
        ImVec2 tileCoordsBottomRight = ImGui::GetItemRectMax();
        ImVec2 tileDims = ImGui::GetItemRectSize(); // more precise than { m_TileScalar*BASE_TILE_WH_RATIO, m_TileScalar}

        if (m_SelectedTile != guid) {
            drawlist->PushClipRect(tileCoordsTopLeft, tileCoordsBottomRight, true);
        }

        ImFont* font     = ImGui::GetFont();
        float   fontSize = BASE_TILE_ICON_FONT_SIZE * m_TileScalar;
        ImVec2 iconDims = font->CalcTextSizeA(
            fontSize,    // size in pixels
            FLT_MAX,     // no wrapping
            0.0f,        // extra spacing between glyphs
            icon         // the text
        );

        ImVec2 iconPos  = {
            tileCoordsTopLeft.x + (tileDims.x - iconDims.x) * 0.5f,
            tileCoordsTopLeft.y + (tileDims.x - iconDims.y) * 0.5f
        };
        drawlist->AddText(nullptr, BASE_TILE_ICON_FONT_SIZE*m_TileScalar, iconPos, ImGui::GetColorU32(theme[EditorCol_Text1]), icon);

        // Render Title with wrapping
        ImVec2 titlePos = {
            tileCoordsTopLeft.x,
            tileCoordsTopLeft.y + tileDims.y * BASE_TILE_WH_RATIO
        };

        float wrapWidth = tileDims.x;
        drawlist->AddText(
            ImGui::GetFont(),                     // font
            BASE_TILE_TITLE_FONT_SIZE,                        // font size
            titlePos,                              // position
            ImGui::GetColorU32(theme[EditorCol_Text1]),                            // color
            title,                                // text
            nullptr,                              // text_end (nullptr = strlen)
            wrapWidth                             // wrap width
        );
        
        if (m_SelectedTile != guid) {
            drawlist->PopClipRect();
        }

        ImGui::PopID();
    }

}