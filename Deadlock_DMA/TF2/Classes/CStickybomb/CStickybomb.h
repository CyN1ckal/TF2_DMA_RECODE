#pragma once
#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include <cstdint>

class CStickybomb : public CBaseEntity
{
public:
	CStickybomb(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {};
};