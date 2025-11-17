#pragma once
#include "ConditionBits.h"
#include "BoneArray.h"
#include "TF2/Constants/eTFCond.h"

class CTFPlayerInfo : public CBaseEntityInfo
{
public:
	BoneArray m_BoneArray = { 0 };
	Vector3 m_Rotation = { 0.0f,0.0f,0.0f };
	uintptr_t m_BoneMatrixAddr = 0x0;
	uint32_t m_CurrentHealth = 0x0;
	uint32_t m_ClassID = 0x0;
	uint8_t m_DeadByte = 0x0;
	ConditionBits m_ConditionBits = { 0 };

public:
	const std::string m_GetClassName() const;
	const std::string m_GetTeamName() const;
	const bool m_IsAlive() const;
	const bool m_IsDead() const;
	const uint16_t m_GetMaxHealth() const;
	const bool m_IsValidESPTarget() const;
	const float m_GetDistanceToLocalPlayer() const;
	const float m_GetDistanceToLocalPlayerMeters() const;
	const uint32_t m_GetHeadBoneIndex() const;
	const uint32_t m_GetBodyBoneIndex() const;
	const bool m_InCondition(ETFCond Condition) const;
	const ImVec4 m_GetESPColor() const;
};