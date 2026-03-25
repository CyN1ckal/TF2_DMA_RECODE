#pragma once
#include "TF2/Classes/CGenericBuilding/CGenericBuilding.h"
#include <cstdint>

class CTeleporter : public CGenericBuilding
{
public:
	CTeleporter(uintptr_t EntityAddress) : CGenericBuilding(EntityAddress) {};
};