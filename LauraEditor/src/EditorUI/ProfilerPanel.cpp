#include "ProfilerPanel.h"

#include <implot.h>

namespace Laura
{


	void ProfilerPanel::OnImGuiRender(std::shared_ptr<const Profiler> profiler)
	{
		ImGui::Begin("Profiler");
        static ImS8  data[30] = { 83, 67, 23, 89, 83, 78, 91, 82, 85, 90,  // midterm
                         80, 62, 56, 99, 55, 78, 88, 78, 90, 100, // final
                         80, 69, 52, 92, 72, 78, 75, 76, 89, 95 }; // course

        static const char* ilabels[] = { "Midterm Exam","Final Exam","Course Grade" };
        //static const char* glabels[] = { "S1","S2","S3","S4","S5","S6","S7","S8","S9","S10" };
        static const double positions[] = { 0,1,2,3,4,5,6,7,8,9 };

        static int items = 3;
        static int groups = 10;
        static float size = 0.67f;

        static ImPlotBarGroupsFlags flags = 0;

        ImGui::CheckboxFlags("Stacked", (unsigned int*)&flags, ImPlotBarGroupsFlags_Stacked);
        ImGui::SameLine();

        ImGui::SliderInt("Items", &items, 1, 3);

        if (ImPlot::BeginPlot("Frame Breakdown")) {
            ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
            ImPlot::SetupAxes("Time", "Frame length", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisTicks(ImAxis_X1, positions, groups);
            ImPlot::PlotBarGroups(ilabels, data, items, groups, 1, 0, flags);
            ImPlot::EndPlot();
        }
		ImGui::End();
	}
}