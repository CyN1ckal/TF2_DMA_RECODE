#pragma once
#include "TF2/Classes/CGenericBuilding/CGenericBuilding.h"
#include "TF2/Classes/CSentryGun/CSentryGun.h"
#include "TF2/Classes/CDispenser/CDispenser.h"
#include "TF2/Classes/CTeleporter/CTeleporter.h"

class Draw_Buildings
{
public:
	static void DrawAll();

	static void operator()(CSentryGun& Gun);
	static void operator()(CGenericBuilding& Generic);
	static void operator()(CDispenser& Generic);
	static void operator()(CTeleporter& Generic);

private:
};