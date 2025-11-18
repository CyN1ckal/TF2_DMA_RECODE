#pragma once
#include "pch.h"
#include "General Drawing.h"
#include "GUI/Color Picker/Color Picker.h"

void DrawGenericTextAtWorldPosition(const std::string& Name, const Vector3& Origin, int& LineNumber, ImColor Color)
{
	Vector2 ScreenPos{};
	if (!WorldToScreen(Origin, ScreenPos)) return;

	auto WindowPos = ImGui::GetWindowPos();
	ScreenPos += Vector2(WindowPos.x, WindowPos.y);

	DrawGenericTextAtScreenPosition(Name, ScreenPos, LineNumber, Color);
}

void DrawGenericTextAtScreenPosition(const std::string& Name, const Vector2& ScreenPos, int& LineNumber, ImColor Color)
{
	auto TextSize = ImGui::CalcTextSize(Name.c_str());

	ImVec2 TextPosition = { ScreenPos.x - (TextSize.x * 0.5f), ScreenPos.y + (LineNumber * TextSize.y) };

	ImGui::GetWindowDrawList()->AddText(TextPosition, Color, Name.c_str());

	LineNumber++;
}

void DrawGenericStatusBar(float CurrentValue, float MaxValue, ImColor Color, ImDrawList* DrawList, const Vector2& ScreenPos, int& LineNumber)
{
	constexpr float BarWidth = 80.0f;
	constexpr float Padding = 2.0f;
	constexpr float EffectiveWidth = BarWidth - (Padding * 2.0f);

	float ValueRatio = CurrentValue / MaxValue;
	ValueRatio = std::clamp(ValueRatio, 0.0f, 1.0f);

	auto TextHeight = ImGui::GetTextLineHeight();

	ImVec2 TopLeft = ImVec2{ ScreenPos.x - (BarWidth * 0.5f), ScreenPos.y + (LineNumber * TextHeight) };
	ImVec2 BottomRight = ImVec2{ TopLeft.x + BarWidth, TopLeft.y + TextHeight };
	DrawList->AddRectFilled(TopLeft, BottomRight, ColorPicker::ValueBarBackground);

	TopLeft.x += Padding;
	TopLeft.y += Padding;
	BottomRight.x = TopLeft.x + (EffectiveWidth * ValueRatio);
	BottomRight.y -= Padding;
	DrawList->AddRectFilled(TopLeft, BottomRight, Color);

	LineNumber++;
}