#pragma once
#include "pch.h"
#include "Engine/Math/MatrixMath.h"

void DrawGenericTextAtWorldPosition(const std::string& Name, const Vector3& Origin, int& LineNumber, ImColor Color = ImColor(255, 255, 255));
void DrawGenericTextAtScreenPosition(const std::string& Name, const Vector2& ScreenPos, int& LineNumber, ImColor Color = ImColor(255, 255, 255));
void DrawGenericStatusBar(float CurrentValue, float MaxValue, ImColor Color, ImDrawList* DrawList, const Vector2& ScreenPos, int& LineNumber);