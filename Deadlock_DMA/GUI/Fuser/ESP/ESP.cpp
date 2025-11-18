#include "pch.h"

#include "ESP.h"

#include "GUI/Color Picker/Color Picker.h"

#include "Draw/Players.h"
#include "Draw/Buildings.h"
#include "Draw/Explosives.h"

void ESP::OnFrame()
{
	if (!bMasterToggle)
		return;

	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	Draw_Players::DrawAll();

	Draw_Buildings::DrawAll();

	Draw_Explosives::DrawAll();

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	if (!bSettings)
		return;

	ImGui::Begin("ESP Settings", &bSettings);

	ImGui::Checkbox("Enable ESP", &bMasterToggle);

	ImGui::Checkbox("Enable Player ESP", &Draw_Players::bMasterToggle);
	ImGui::Indent();
	ImGui::Checkbox("Hide Friendly Players", &Draw_Players::bHideFriendly);
	ImGui::Checkbox("Hide Dormant Players", &Draw_Players::bHideDormant);
	ImGui::Unindent();

	ImGui::Checkbox("Enable Building ESP", &Draw_Buildings::bMasterToggle);
	ImGui::Indent();
	ImGui::Checkbox("Hide Friendly Buildings", &Draw_Buildings::bHideFriendly);
	ImGui::Checkbox("Hide Dormant Buildings", &Draw_Buildings::bHideDormant);
	ImGui::Unindent();

	ImGui::Checkbox("Enable Explosive ESP", &Draw_Explosives::bMasterToggle);
	ImGui::Indent();
	ImGui::Checkbox("Hide Friendly Explosives", &Draw_Explosives::bHideFriendly);
	ImGui::Unindent();

	ImGui::End();
}