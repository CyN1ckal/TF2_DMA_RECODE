#include "pch.h"

#include "CSentryGun.h"

#include "TF2/Offsets/Offsets.h"

void CSentryGun::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CGenericBuilding::PrepareRead_1(vmsh);

	if (IsInvalid()) return;

	uintptr_t NumBulletsAddress = m_EntityAddress + Offsets::CSentryGun::NumBullets;
	VMMDLL_Scatter_PrepareEx(vmsh, NumBulletsAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(m_NumBullets), nullptr);

	uintptr_t NumRocketsAddress = m_EntityAddress + Offsets::CSentryGun::NumRockets;
	VMMDLL_Scatter_PrepareEx(vmsh, NumRocketsAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(m_NumRockets), nullptr);
}