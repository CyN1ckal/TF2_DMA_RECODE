#include "pch.h"

#include "CGenericBuilding.h"

#include "TF2/Offsets/Offsets.h"

void CGenericBuilding::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEntity::PrepareRead_1(vmsh, false);

	if (IsInvalid()) return;

	uintptr_t BuildingLevelAddress = m_EntityAddress + Offsets::CGenericBuilding::CurrentBuildingLevel;
	VMMDLL_Scatter_PrepareEx(vmsh, BuildingLevelAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(m_CurrentBuildingLevel), nullptr);

	uintptr_t UpgradeProgressAddress = m_EntityAddress + Offsets::CGenericBuilding::UpgradeProgress;
	VMMDLL_Scatter_PrepareEx(vmsh, UpgradeProgressAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_UpgradeProgress), nullptr);

	uintptr_t CurrentHealthAddr = m_EntityAddress + Offsets::CGenericBuilding::CurrentHealth;
	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);

	uintptr_t MaxHealthAddr = m_EntityAddress + Offsets::CGenericBuilding::MaxHealth;
	VMMDLL_Scatter_PrepareEx(vmsh, MaxHealthAddr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_MaxHealth), nullptr);
}

void CGenericBuilding::Finalize()
{
	CBaseEntity::Finalize();
}