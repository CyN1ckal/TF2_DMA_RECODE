#pragma once
#include "TF2/Classes/CGenericBuilding/CGenericBuilding.h"

class CSentryGun : public CGenericBuilding
{
public:
	uint32_t m_NumBullets{ 0x0 };
	uint32_t m_NumRockets{ 0x0 };

public:
	CSentryGun(uintptr_t EntityAddress) : CGenericBuilding(EntityAddress) {};
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);

public:
	const uint32_t GetMaxAmmo();
};