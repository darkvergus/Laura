#include <IconsFontAwesome6.h>
#include "ProfilerPanel.h"
#include <implot.h>

namespace Laura
{

	void ProfilerPanel::OnImGuiRender(std::shared_ptr<Profiler> profiler)
	{
		if (!m_EditorState->temp.ProfilerPanelOpen)
			return;

		auto t = profiler->timer("ProfilerPanel");

		static int span = 100; // full range by default
		static ImPlotAxisFlags YaxisFlags = ImPlotAxisFlags_AutoFit;
		ImGui::Begin(ICON_FA_STOPWATCH " Profiler", &m_EditorState->temp.ProfilerPanelOpen);
		if (profiler->globalTimerSet) {
			const ScrollingBuffer& buff = profiler->getGlobalBuffer();
			if (buff.size() != 0)
			{
				double global_ms = buff.data()[buff.size() - 1];
				int global_FPS = 1000.0 / global_ms;
				ImGui::AlignTextToFramePadding();
				ImGui::Text("%d FPS", global_FPS); ImGui::SameLine();
				ImGui::Text("[%.1f ms]", global_ms); ImGui::SameLine();

				m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);
				if (ImGui::Button("Clear All " ICON_FA_REPEAT))
					profiler->clear();
				ImGui::SameLine();
				ImGui::CheckboxFlags("Autofit", &YaxisFlags, ImPlotAxisFlags_AutoFit);
				m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);
			}
		}

		float sliderHeight = ImGui::GetFrameHeightWithSpacing();
		ImVec2 avail = ImGui::GetContentRegionAvail();
		float plotHeight = avail.y - sliderHeight;

		if (ImPlot::BeginPlot("##Rolling", ImVec2(avail.x - plotHeight, plotHeight)));
		{
			if (profiler->globalTimerSet)
			{
				ImPlot::SetupAxes("##Frame", "ms",
					ImPlotAxisFlags_AutoFit,
					YaxisFlags);

				ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
				ImPlot::SetupAxisLimits(ImAxis_X1, 0, profiler->getGlobalBuffer().size(span), ImGuiCond_Always);
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.2f);
				bool setLimits = false;
				for (const auto& p : profiler->data())
				{
					const std::string label = p.first;
					const ScrollingBuffer buff = p.second;
					ImPlot::PlotShaded<double>(label.c_str(), buff.data(span), buff.size(span));
					ImPlot::PlotLine<double>(label.c_str(), buff.data(span), buff.size(span));
				}
				ImPlot::PopStyleVar();
			}
			ImPlot::EndPlot(); ImGui::SameLine();
		}

		if (ImPlot::BeginPlot("##Pie", ImVec2(plotHeight, plotHeight)))
		{
			std::vector<std::string> labels;
			std::vector<double> data;
			for (const auto& p : profiler->data())
			{
				if (p.first == profiler->getGlobalLabel()) continue;

				labels.push_back(p.first);
				data.push_back(p.second.data()[p.second.size() - 1]);
			}
			
			std::vector<const char*> clabels;
			for (const std::string& str : labels){
				clabels.push_back(str.c_str());
			}

			ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
			ImPlot::SetupAxesLimits(0, 1, 0, 1);
			ImPlot::PlotPieChart<double>(clabels.data(), data.data(), labels.size(), 0.5, 0.5, 0.4, "%.2f", 90);
			ImPlot::EndPlot();
		}

		ImGui::SetNextItemWidth(-FLT_MIN); // span full width
		ImGui::SliderInt("##History", &span, 1, 100, "Span [%d%%]");
		ImGui::End();
	}
}