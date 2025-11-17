#include "pch.h"

#include "ESP.h"

#include "GUI/Color Picker/Color Picker.h"

#include "Draw/Players.h"

void ESP::OnFrame()
{
	if (!bMasterToggle)
		return;

	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	Draw_Players::DrawAll();

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");

	ImGui::Checkbox("Enable ESP", &bMasterToggle);

	ImGui::End();
}