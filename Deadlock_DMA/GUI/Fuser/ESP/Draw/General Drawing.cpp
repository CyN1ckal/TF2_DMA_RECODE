#pragma once
#include "pch.h"
#include "General Drawing.h"

void DrawGenericText(const std::string& Name, Vector3& Origin, ImColor Color)
{
	Vector2 ScreenPos{};
	if (!WorldToScreen(Origin, ScreenPos)) return;

	auto TextSize = ImGui::CalcTextSize(Name.c_str());

	ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x * 0.5f), ScreenPos.y });

	ImGui::TextColored(Color, Name.c_str());
}