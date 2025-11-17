#pragma once

#include "TF2/Classes/CBaseEntity/CBaseEntity.h"

class CBuilding : public CBaseEntityInfo
{
public:
	uint32_t m_UpgradeProgress = 0x0;
	uint32_t m_CurrentBuildingLevel = 0x0;
	uint32_t m_CurrentHealth = 0;
	uint32_t m_MaxHealth = 0;
};