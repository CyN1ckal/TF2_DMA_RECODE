#pragma once
#include "makcu/makcu.h"
#include "Engine/Math/MatrixMath.h"

class Aimbot
{
public:
	static void RenderSettings();
	static void RenderFOVCircles();
	static void OnDMAFrame(DMA_Connection* Conn);

private:
	static inline bool bMasterToggle{ false };
	static inline float fMaxFOV{ 100.0f };
	static inline float fDeadzoneFOV{ 2.0f };
	static inline float fDampen{ 0.95f };

private:
	static inline makcu::Device m_Device{};

private:
	static Vector2 GetMouseDelta();
};