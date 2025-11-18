#pragma once
#include "TF2/Classes/CGenericBuilding/CGenericBuilding.h"
#include "TF2/Classes/CSentryGun/CSentryGun.h"
#include "TF2/Classes/CDispenser/CDispenser.h"
#include "TF2/Classes/CTeleporter/CTeleporter.h"
#include "Engine/Math/MatrixMath.h"

class Draw_Buildings
{
public:
	static void DrawAll();
	static void operator()(CSentryGun& Gun);
	static void operator()(CDispenser& Generic);
	static void operator()(CTeleporter& Generic);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bHideFriendly{ true };
	static inline bool bHideDormant{ true };
	static inline bool bSentryHealthBar{ true };
	static inline bool bSentryAmmoBar{ true };
	static inline bool bDispenserHealthBar{ true };
	static inline bool bTeleporterHealthBar{ true };

private:
	static void DrawGenericBuildingNameTag(const std::string& Name, CGenericBuilding& Building, const Vector2& ScreenPosition, int& LineNumber);
	static void DrawBuildingHealthBar(CGenericBuilding& Building, const Vector2& ScreenPosition, int& LineNumber);
	static void DrawSentryAmmoBar(CSentryGun& Gun, const Vector2& ScreenPosition, int& LineNumber);
};