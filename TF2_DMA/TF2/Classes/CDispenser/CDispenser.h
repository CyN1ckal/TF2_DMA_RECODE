#pragma once
#include "TF2/Classes/CGenericBuilding/CGenericBuilding.h"
#include <cstdint>

class CDispenser : public CGenericBuilding
{
public:
	CDispenser(uintptr_t m_EntityAddress) : CGenericBuilding(m_EntityAddress) {};
};