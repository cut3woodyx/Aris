/******************************************************************************/
/*!
\file		imguiPerformance.cpp
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "imgui.h"
#include "imguiMgr.h"
#include "psapi.h"
#include "FWQuery.h"
#include "LESysMgr.h"

namespace FwEngine
{
	namespace
	{
		const size_t MAXSIZEVEC = 100;
		static std::vector<float> cpuUsageVec(MAXSIZEVEC);
		static std::vector<float> cpuMemoryUsage(MAXSIZEVEC);
		static Query perfQ;

		static float lastQueried = 0.0f;
		static const float interval = 0.25f;
		static float highestMemUsed = 0.0f;

		static bool roundTo = false;
	}

	size_t PrintMemoryInfo(DWORD processID)
	{
		HANDLE hProcess;
		PROCESS_MEMORY_COUNTERS pmc;

		// Print information about the memory usage of the process.

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, processID);
		if (NULL == hProcess)
			return 0;
		size_t memMB = 0;
		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{
			ImGui::Text(std::string("VRAM: ").append(
				std::to_string(pmc.PagefileUsage / (1 << 20)))
				.append(".")
				.append(std::to_string(pmc.PeakPagefileUsage - (pmc.PagefileUsage % 1 << 20)))
				.append(" MB").c_str());
			ImGui::Separator();
			memMB = pmc.PagefileUsage / (1 << 20);
		}

		CloseHandle(hProcess);
		return memMB;
	}


	void ImguiMgr::ImGUIPerformance(float dt)
	{
		ImGui::Begin("Performance Viewer", nullptr);

		if (ImGui::IsWindowCollapsed())
		{
			ImGui::End();
			return;
		}

		size_t memMB = PrintMemoryInfo(GetCurrentProcessId());
		
		lastQueried += dt;
		// Seperate all timings by percentile
		if (lastQueried > interval)
		{
			lastQueried -= interval;
			if (cpuUsageVec.size() > MAXSIZEVEC - 1)
				cpuUsageVec.erase(std::begin(cpuUsageVec));
			cpuUsageVec.push_back((float)perfQ);

			if (cpuMemoryUsage.size() > MAXSIZEVEC - 1)
				cpuMemoryUsage.erase(std::begin(cpuMemoryUsage));
			cpuMemoryUsage.push_back((float)memMB);

			if (highestMemUsed < (float)memMB)
				highestMemUsed = (float)memMB * 2.0f;

		}

		ImGui::PlotLines("Memory Usage", cpuMemoryUsage.data(), (int)cpuMemoryUsage.size(), 0,
			std::string(std::to_string((int)cpuMemoryUsage[cpuMemoryUsage.size() - 1])).append(" MB").c_str(), 0.0f, highestMemUsed, { 0, 100.0f });

		ImGui::Separator();

		ImGui::PlotLines("CPU Usage", cpuUsageVec.data(), (int)cpuUsageVec.size(), 0, 
			std::string(std::to_string(cpuUsageVec[cpuUsageVec.size()-1])).append(" %").c_str(), 0.0f, 100.0f, { 0, 100.0f });

		LARGE_INTEGER prevTimer = _mgr->_systemFuncTime[0].second;
		double diffTimer = (double)(_mgr->_systemFuncTime[_mgr->_systemFuncTime.size() - 1].second.QuadPart
			- prevTimer.QuadPart);

		ImGui::Separator();

		ImGui::Checkbox("Round to Lowest", &roundTo);
		std::vector<float> histoData;
		std::string display;

		// Display CPU usage per system
		for (decltype(_mgr->_systemFuncTime)::iterator i = std::begin(_mgr->_systemFuncTime) + 1;
			i != std::end(_mgr->_systemFuncTime); ++i)
		{
			double diff = (double)(i->second.QuadPart - prevTimer.QuadPart);
			prevTimer = i->second;
			diff /= diffTimer;
			diff *= 100.0f;
			histoData.emplace_back((float)diff);
			display.append(i->first).append(" ");
			if (roundTo)
				ImGui::Text(i->first.append(": ").append(std::to_string((int)diff)).append(" %%").c_str());
			else
				ImGui::Text(i->first.append(": ").append(std::to_string(diff)).append(" %%").c_str());
		}

		ImGui::Separator();

		ImGui::PlotHistogram("", histoData.data(), (int)histoData.size(), 0, NULL, 0.0f, 100.0f, ImVec2(histoData.size() * 50.0f, 140.0f));
		ImGui::Separator();
		ImGui::Text(display.c_str());

		ImGui::End();
	}
}