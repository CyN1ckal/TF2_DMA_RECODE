#pragma once
#include "Engine/Vector3.h"

class CBaseEntityInfo
{
public:
	Vector3 m_Origin = { 0.0f,0.0f,0.0f };
	uint32_t m_TeamID = 0;
	uint32_t EntityID = 0;
	bool m_bIsLocalPlayer = false;
	int8_t m_DormantByte = 0x0;

	const bool IsFriendly() const;
	const bool IsLocalPlayer() const;
	const ImVec4 GetTeamColor() const;
	const bool IsDormant() const;
	const float DistanceFromLocalPlayerMeters() const;
};