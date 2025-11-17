#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"

void Fuser::OnFrame()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	ImGui::SetNextWindowPos({ 100,100 }, ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

	ImGui::SetNextWindowSize(ScreenSize, ImGuiCond_Always);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Fuser", nullptr, window_flags);

	ESP::OnFrame();

	ImGui::End();

	ImGui::PopStyleColor(1);
}

void Fuser::RenderSettings()
{
	ImGui::Begin("Fuser Settings");

	ImGui::Checkbox("Enable Fuser", &bMasterToggle);

	ImGui::PushItemWidth(60.0f);

	ImGui::InputFloat("Screen Size X", &ScreenSize.x, 0.0f, 0.0f, "%.0f");

	ImGui::InputFloat("Screen Size Y", &ScreenSize.y, 0.0f, 0.0f, "%.0f");

	ImGui::PopItemWidth();

	ImGui::End();
}