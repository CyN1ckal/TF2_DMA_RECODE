#pragma once

class ColorPicker
{
public:
	static void RenderColorPicker();

public:
	static inline ImColor RedTeam = { 0.95f, 0.2f, 0.2f, 1.0f };
	static inline ImColor BluTeam = { 0.00f, 0.4f, 1.0f, 1.0f };
	static inline ImColor Dormant = { 0.5f, 0.5f, 0.5f, 1.0f };
	static inline ImColor Unknown = { 0.6f, 0.0f, 1.0f, 1.0f };
	static inline ImColor Skeleton = { 1.0f, 1.0f, 1.0f, 1.0f };
	static inline ImColor HighlightedSkeleton = { 255,255,0 };
	static inline ImColor MaxFOV = { 255,255,255 };
	static inline ImColor DeadzoneFOV = { 255,0,0 };
	static inline ImColor HealthPack = { 0.0f, 0.7f, 0.0f, 1.0f };
	static inline ImColor AmmoPack = { 0.66f, 0.66f, 0.66f, 1.0f };
	static inline ImColor Money = { 0.0f, 0.27f, 0.0f, 1.0f };
	static inline ImColor Rocket = { 255,0,0 };
	static inline ImColor Sticky = { 255,0,0 };
	static inline ImColor ValueBarBackground = { 50,50,50 };
};