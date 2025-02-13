#include <IconsFontAwesome6.h>
#include <implot.h>
#include "ProfilerPanel.h"

namespace Laura {

    void ProfilerPanel::OnImGuiRender(std::shared_ptr<Profiler> profiler)
    {
        // Skip rendering if the profiler panel is closed.
        if (!m_EditorState->temp.ProfilerPanelOpen)
            return;

        auto t = profiler->timer("ProfilerPanel");

        static int span = 100; // Span 100% by default
        static ImPlotAxisFlags AutoFitFlag_Checkbox_bool = ImPlotAxisFlags_AutoFit; // AutoFit by default
        static float SubPlot_WidthRatios[] = { 5, 1 };

        ImPlot::PushColormap(ImPlotColormap_Deep);

        // Begin the main profiler window.
        ImGui::Begin(ICON_FA_STOPWATCH " Profiler", &m_EditorState->temp.ProfilerPanelOpen);

        // Display FPS, ms, Clear Button & Autofit
        if (profiler->globalTimerSet) {
            const ScrollingBuffer& buff = profiler->getGlobalBuffer();
            if (!buff.empty()) {
                double Global_ms = buff.data()[buff.size() - 1];
                int Global_FPS = static_cast<int>(1000.0 / Global_ms); // FPS = 1/ms
                ImGui::AlignTextToFramePadding();
                ImGui::Text("%d FPS", Global_FPS);
                ImGui::SameLine();
                ImGui::Text("[%.1f ms]", Global_ms);
                ImGui::SameLine();

                // Button to clear the profiler data.
                m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);
                if (ImGui::Button("Clear All " ICON_FA_REPEAT))
                    profiler->clear();
                ImGui::SameLine();

                // Checkbox to toggle Autofit for Y-axis.
                ImGui::CheckboxFlags("Autofit", &AutoFitFlag_Checkbox_bool, ImPlotAxisFlags_AutoFit);
                m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);
            }
        }

        // Calculate available space for plots.
        ImVec2 avail = ImGui::GetContentRegionAvail();
        float SpanSlider_Height = ImGui::GetFrameHeightWithSpacing();
        ImVec2 SubPlot_Size = { avail.x, avail.y - SpanSlider_Height };

        // Begin subplots: one row, two columns.
        const ImPlotSubplotFlags SubPlot_Flags = ImPlotSubplotFlags_ShareItems | ImPlotSubplotFlags_NoTitle;
        ImPlot::BeginSubplots("##SUBPLOTS", 1, 2, SubPlot_Size, SubPlot_Flags, 0, SubPlot_WidthRatios);

        // Setup a shared legend on the left side.
        ImPlot::SetupLegend(ImPlotLocation_West, ImPlotLegendFlags_Sort);

        // --- Left Subplot: Line Chart ---
        if (ImPlot::BeginPlot("##LINE_CHART")) {
            if (profiler->globalTimerSet) {
                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit, AutoFitFlag_Checkbox_bool | ImPlotAxisFlags_Opposite);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, profiler->getGlobalBuffer().size(span), ImGuiCond_Always);
                //ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_Opposite);
                ImPlot::SetupAxisFormat(ImAxis_Y1, "%g ms");
               
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.1f);

                for (const auto& p : profiler->data()) {
                    const std::string label = p.first;
                    const ScrollingBuffer buff = p.second;
                    ImPlot::PlotShaded<double>(label.c_str(), buff.data(span), buff.size(span));
                    ImPlot::PlotLine<double>(label.c_str(), buff.data(span), buff.size(span));
                }
                ImPlot::PopStyleVar();
            }
            ImPlot::EndPlot();
            ImGui::SameLine();
        }

        // --- Right Subplot: Pie Chart ---
        if (ImPlot::BeginPlot("##PIE_CHART", ImVec2(-1, -1), ImPlotFlags_Equal)) {
            std::vector<std::string> labels;
            std::vector<double> data;
            for (const auto& p : profiler->data()) {
                // Skip the global timer entry.
                if (p.first == profiler->getGlobalLabel())
                    continue;
                labels.push_back(p.first);
                //data.push_back(p.second.data()[p.second.size() - 1]);
                data.push_back(p.second.average(std::ceil(span/10.0))); // average of the last tenth of history
            }

            // Convert labels to const char* array.
            std::vector<const char*> clabels;
            for (const std::string& str : labels) {
                clabels.push_back(str.c_str());
            }

            const ImPlotAxisFlags PieChart_AxisFlags = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit;
            ImPlot::SetupAxes(nullptr, nullptr, PieChart_AxisFlags, PieChart_AxisFlags);
            ImPlot::SetupAxesLimits(0, 1, 0, 1);
            // Plot the pie chart.
            ImPlot::PlotPieChart<double>(clabels.data(), data.data(), labels.size(), 0.5, 0.5, 0.40, "%.1f", 90, ImPlotPieChartFlags_Exploding);
            ImPlot::EndPlot();
        }

        ImPlot::EndSubplots();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##History", &span, 1, 100, "History [%d%%]");

        ImGui::End(); // Profiler Window
        ImPlot::PopColormap();
    }

} // namespace Laura
