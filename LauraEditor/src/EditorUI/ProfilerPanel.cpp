#include <IconsFontAwesome6.h>
#include <implot.h>
#include "ProfilerPanel.h"

namespace Laura {

    void ProfilerPanel::OnImGuiRender(std::shared_ptr<Profiler> profiler)
    {
        if (!m_EditorState->temp.ProfilerPanelOpen)
            return;

        if (!profiler->globalTimerSet) {
            LR_CORE_WARN("Unable to render Profiler Panel - No Global Timer Set");
            return;
        }

        auto t = profiler->timer("ProfilerPanel");

        ImPlot::PushColormap(ImPlotColormap_Deep);
        m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);
        ImGui::Begin(ICON_FA_STOPWATCH " Profiler", &m_EditorState->temp.ProfilerPanelOpen);

        const char* playLabel = (profiler->isPaused) ? ICON_FA_PLAY : ICON_FA_PAUSE;
        m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);
        if (ImGui::Button(playLabel))
            profiler->isPaused = !profiler->isPaused;
        m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);

        const ScrollingBuffer& buff = profiler->getGlobalBuffer();
        if (!buff.empty()) {
            ImGui::SameLine();
            double Global_ms = buff.data()[buff.size() - 1];
            int Global_FPS = static_cast<int>(1000.0 / Global_ms); // FPS = 1/ms
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%d FPS [%.1f ms]", Global_FPS, Global_ms);
            ImGui::SameLine();

            if (ImGui::Button("Clear " ICON_FA_REPEAT))
                profiler->clear();
            ImGui::SameLine();
        }

        static ImPlotAxisFlags shouldAutoFit = ImPlotAxisFlags_AutoFit;
        ImGui::CheckboxFlags("Autofit", &shouldAutoFit, ImPlotAxisFlags_AutoFit);

        static int colorIndex = 0;
        static std::unordered_map<std::string, plotLineStyle> plotLineStyleMap;
        const ImPlotSubplotFlags SubPlot_Flags = ImPlotSubplotFlags_ShareItems | ImPlotSubplotFlags_NoTitle;
        if (ImPlot::BeginSubplots("##SUBPLOTS", 1, 1, ImVec2(-1, -1), SubPlot_Flags, 0)) { // Using subplot to allow chart scale independently from legend
            ImPlot::SetupLegend(ImPlotLocation_West, ImPlotLegendFlags_Sort);
            ImPlot::BeginPlot("##LinePlot", ImVec2(-1, -1), ImPlotFlags_NoMouseText);
            ImPlot::SetupAxes(nullptr, nullptr, shouldAutoFit, shouldAutoFit | ImPlotAxisFlags_Opposite);
            ImPlot::SetupAxisFormat(ImAxis_Y1, "%gms");
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.05f);
            
            for (const auto& p : profiler->data()) {
                const std::string label = p.first;
                const ScrollingBuffer& buff = p.second;

                auto it = plotLineStyleMap.find(label);
                if (it == plotLineStyleMap.end())
                    it = plotLineStyleMap.emplace(label, plotLineStyle(ImPlot::GetColormapColor(colorIndex), colorIndex++, 1.0f)).first;

                auto& lineStyle = it->second;
                ImPlot::SetNextLineStyle(lineStyle.color, lineStyle.thickness);
                ImPlot::PlotLine<double>(label.c_str(), buff.data(), buff.size());
                ImPlot::PlotShaded<double>(label.c_str(), buff.data(), buff.size());

                if (ImPlot::IsLegendEntryHovered(label.c_str())) {
                    ImGui::SetTooltip("Open %s Options", label.c_str());

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                        ImGui::OpenPopup(label.c_str());
                }

                if (ImGui::BeginPopup(label.c_str())) {
                    ImGui::Text("%s Options", label.c_str());
                    if (ImGui::MenuItem(ICON_FA_FILE_ARROW_DOWN " Export...")) {
                    }

                    ImGui::Separator();
                    ImGui::Spacing();
                    ImGui::ColorPicker4("##clrpckr", (float*)&lineStyle.color, ImGuiColorEditFlags_NoInputs);
                    ImGui::SliderFloat("##thcknss", &lineStyle.thickness, 1.0f, 10.0f, "Thickness %.1f");
                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FA_ROTATE)) {
                        lineStyle.color = ImPlot::GetColormapColor(lineStyle.colormapID);
                        lineStyle.thickness = 1.0f;
                    }
                    ImGui::EndPopup();
                }
            }
            ImPlot::PopStyleVar();
            ImPlot::EndPlot();
            ImPlot::EndSubplots();
        }
        ImGui::End();
        ImPlot::PopColormap();
        m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);
    }
}
