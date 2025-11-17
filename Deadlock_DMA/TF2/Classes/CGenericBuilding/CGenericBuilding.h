#pragma once
#include "TF2/Classes/CBaseEntity/CBaseEntity.h"

class CGenericBuilding : public CBaseEntity
{
public:
	uint32_t m_UpgradeProgress{ 0x0 };
	uint32_t m_CurrentBuildingLevel{ 0x0 };
	uint32_t m_MaxHealth{ 0 };

public:
	CGenericBuilding(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {};
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
};