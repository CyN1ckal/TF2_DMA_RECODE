#pragma once
#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include <cstdint>

class CRocket : public CBaseEntity
{
public:
	CRocket(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {};
};