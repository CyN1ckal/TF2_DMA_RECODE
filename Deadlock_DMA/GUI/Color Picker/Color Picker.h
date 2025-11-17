#pragma once

class ColorPicker
{
public:
	static void RenderColorPicker();

public:
	static inline ImColor RedTeam = { 0.95f, 0.2f, 0.2f, 1.0f };
	static inline ImColor BluTeam = { 0.00f, 0.4f, 1.0f, 1.0f };
	static inline ImColor Spectator = { 0.3f, 0.3f, 0.3f, 1.0f };
	static inline ImColor Dormant = { 0.5f, 0.5f, 0.5f, 1.0f };
	static inline ImColor LocalPlayer = { 0.0f, 1.0f, 0.0f, 1.0f };
	static inline ImColor Incomplete = { 0.0f, 1.0f, 0.0f, 1.0f };
	static inline ImColor SpyAlert = { 1.0f,0.0f,0.8f,1.0f };
	static inline ImColor Unknown = { 0.6f, 0.0f, 1.0f, 1.0f };
	static inline ImColor HealthBarBackground = { 0.06f, 0.06f, 0.06f, 1.0f };
	static inline ImColor HealthBarForeground = { 0.0f, 0.7f, 0.0f, 1.0f };
	static inline ImColor HealthBarOverheal = { 0.0f, 0.7f, 0.7f, 1.0f };
	static inline ImColor Skeleton = { 1.0f, 1.0f, 1.0f, 1.0f };
	static inline ImColor Highlighted = { 1.0f, 1.0f, 0.0f, 1.0f };
	static inline ImColor FOVCircle = { 1.0f, 0.0f, 0.0f, 1.0f };
	static inline ImColor HealthPack = { 0.0f, 0.7f, 0.0f, 1.0f };
	static inline ImColor AmmoPack = { 0.66f, 0.66f, 0.66f, 1.0f };
	static inline ImColor Money = { 0.0f, 0.27f, 0.0f, 1.0f };
};