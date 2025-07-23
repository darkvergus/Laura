#include "EditorUI\AssetsPanel\AssetsPanel.h"
#include <format>

namespace Laura 
{

	void AssetsPanel::OnImGuiRender() {
        EditorTheme& theme = m_EditorState->temp.editorTheme;

        ImGui::Begin(ICON_FA_CUBE " Assets");

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
                    std::string filename = metadataExtension->sourcePath.filename().string();
                    DrawAssetTile(guid, filename.c_str());
                    float last_assetTile_x2 = ImGui::GetItemRectMax().x;
                    if (last_assetTile_x2 + ImGui::GetItemRectSize().x < column_x2) {
                        ImGui::SameLine(0, manualSpacing);
                    }
                }
                ImGui::Unindent();
            }
            ImGui::EndChild();

            ImGui::TableNextColumn();
            DrawAssetMetadata();
            ImGui::EndTable();
        }
        ImGui::End();
    }


    void AssetsPanel::DrawAssetTile(LR_GUID guid, const char* title) {
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
        if (ImGui::Selectable("##tile", true, ImGuiSelectableFlags_None, { m_TileScalar*BASE_TILE_WH_RATIO, m_TileScalar})) {
            m_SelectedTile = guid;
        }
        theme.PopColor();
        
        // decide on the icon and drag and drop destination based on asset type
        const char* icon = nullptr;
        const char* dndPayloadType = nullptr;
        if (m_ResourcePool->Get<Asset::MeshMetadata>(guid) != nullptr) {
            icon = ICON_FA_CUBE;
            dndPayloadType = DNDPayloadTypes::MESH;
        }
        else if (m_ResourcePool->Get<Asset::TextureMetadata>(guid) != nullptr) {
            icon = ICON_FA_FILE_IMAGE;
            dndPayloadType = DNDPayloadTypes::TEXTURE;
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {

                DNDPayload payload;
                m_DNDPayload.guid = guid;
                strncpy(m_DNDPayload.title, title, sizeof(m_DNDPayload.title));
                m_DNDPayload.title[sizeof(m_DNDPayload.title) - 1] = '\0'; // add null terminator

                ImGui::SetDragDropPayload(dndPayloadType, &m_DNDPayload, sizeof(DNDPayload));
                theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
                ImGui::Text("%s %s", icon, title);
                theme.PopColor();
                ImGui::EndDragDropSource();
            }

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
            ImGui::GetFont(),                           // font
            BASE_TILE_TITLE_FONT_SIZE,                  // font size
            titlePos,                                   // position
            ImGui::GetColorU32(theme[EditorCol_Text1]), // color
            title,                                      // text
            nullptr,                                    // text_end (nullptr = strlen)
            wrapWidth                                   // wrap width
        );
        if (m_SelectedTile != guid) {
            drawlist->PopClipRect();
        }

        ImGui::PopID();
    }
    
    void AssetsPanel::DrawAssetMetadata() {
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x);
        EditorTheme& theme = m_EditorState->temp.editorTheme;

        auto DrawLabelValue = [&theme](const char* label, auto value) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s", label);
            theme.PopColor();
            ImGui::SameLine(150);
            ImGui::Text("%s", std::format("{}", value).c_str());
        };
        
        // No asset selected
        if (m_SelectedTile == LR_GUID::INVALID) {
            return;
        }

        // Check (asset missing in resource pool but editor UI displays it)
        const auto& it = m_ResourcePool->Metadata.find(m_SelectedTile);
        if (it == m_ResourcePool->Metadata.end()) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Error);
            ImGui::Text("[ERROR] Invalid Asset");
            theme.PopColor();
            ImGui::PopTextWrapPos();
            return;
        }

        // Asset Selected (Draw its info)

        const auto& [metadata, metadataExtension] = it->second;
        DrawLabelValue("Source Path:", metadataExtension->sourcePath.string());
        
        // File size formatter (GB > MB > KB > B)
        auto ConvertFileSize = [](uintmax_t fileSizeInBytes) -> std::string {
            constexpr double KB = 1024.0;
            constexpr double MB = 1024.0 * KB;
            constexpr double GB = 1024.0 * MB;

            if (fileSizeInBytes >= static_cast<uintmax_t>(GB)) {
                return std::format("{:.2f} GB", fileSizeInBytes / GB);
            } else if (fileSizeInBytes >= static_cast<uintmax_t>(MB)) {
                return std::format("{:.2f} MB", fileSizeInBytes / MB);
            } else if (fileSizeInBytes >= static_cast<uintmax_t>(KB)) {
                return std::format("{:.2f} KB", fileSizeInBytes / KB);
            } else {
                return std::format("{} Bytes", fileSizeInBytes);
            }
        };

        DrawLabelValue("File Size:", ConvertFileSize(metadataExtension->fileSizeInBytes));
        DrawLabelValue("Load Time:", std::format( "{:.2f} ms", metadataExtension->loadTimeMs ));
        ImGui::Separator();

        // AssetType specific data

        // Try cast to MeshMetadata
        if (auto meshMetadata = dynamic_cast<Asset::MeshMetadata*>(metadata.get())) {
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
        else if (auto texMetadata = dynamic_cast<Asset::TextureMetadata*>(metadata.get())) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
            ImGui::Text("Texture Metadata");
            theme.PopColor();
            DrawLabelValue("Width:", texMetadata->width);
            DrawLabelValue("Height:", texMetadata->height);
            DrawLabelValue("Channels:", texMetadata->channels);
        }

        ImGui::PopTextWrapPos();
    }
}