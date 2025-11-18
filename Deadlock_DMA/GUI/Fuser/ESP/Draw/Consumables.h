#pragma once
#include "TF2/Classes/CAmmoPack/CAmmoPack.h"
#include "TF2/Classes/CHealthPack/CHealthPack.h"

class Draw_Consumables
{
public:
	static void DrawAll();
	static void operator()(CAmmoPack& AmmoPack);
	static void operator()(CHealthPack& HealthPack);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bDrawHealthPacks{ true };
	static inline bool bDrawAmmoPacks{ true };

private:

};