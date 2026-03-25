#pragma once
#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include <cstdint>

class CHealthPack : public CBaseEntity
{
public:
	CHealthPack(uintptr_t EntityAddress) : CBaseEntity(EntityAddress)
	{
	};
};