#pragma once
#include "../CBaseEntity/CBaseEntity.h"
#include "ConditionBits.h"
#include "BoneArray.h"
#include "TF2/Constants/eTFClass.h"
#include "TF2/Constants/eTFCond.h"

class CTFPlayer : public CBaseEntity
{
public:
	BoneArray m_BoneArray{ 0 };
	ConditionBits m_ConditionBits{ 0 };
	uintptr_t m_BoneArrayAddress{ 0 };
	eTFClass m_PlayerClass{ eTFClass::Scout };

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickFinalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void UpdateLocalPlayerInfo();

public:
	void SetLocalPlayer();
	bool IsLocalPlayer();
	std::string_view GetPlayerClassName();
	Vector3 GetHeadPosition();
	Vector3 GetChestPosition();
	bool IsInCond(ETFCond Cond);
	const uint32_t GetMaxHealth();
};