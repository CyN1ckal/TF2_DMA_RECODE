#pragma once
#include "TF2/Classes/CRocket/CRocket.h"
#include "TF2/Classes/CStickybomb/CStickybomb.h"

class Draw_Explosives
{
public:
	static void DrawAll();
	static void operator()(CRocket& Rocket);
	static void operator()(CStickybomb& Bomb);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bHideFriendly{ true };
};