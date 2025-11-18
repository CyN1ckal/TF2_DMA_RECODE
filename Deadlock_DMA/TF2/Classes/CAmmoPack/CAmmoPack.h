#pragma once
#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include <cstdint>

class CAmmoPack : public CBaseEntity
{
public:
	CAmmoPack(uintptr_t EntityAddress) : CBaseEntity(EntityAddress){};
};