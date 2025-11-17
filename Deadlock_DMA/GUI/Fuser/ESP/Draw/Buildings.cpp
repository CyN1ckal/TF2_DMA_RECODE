#include "pch.h"

#include "Buildings.h"

#include "TF2/IEntityList/IEntityList.h"

#include "Engine/Math/MatrixMath.h"
#include "TF2/Constants/Strings/EntityNames.h"

#include "General Drawing.h"

void Draw_Buildings::DrawAll()
{
	std::scoped_lock lk(IEntityList::m_BuildingMutex);

	for (auto& Building : IEntityList::m_Buildings)
		std::visit(Draw_Buildings{}, Building);
}

void Draw_Buildings::operator()(CSentryGun& Gun)
{
	DrawGenericText(Constants::SentryString, Gun.m_Origin);
}

void Draw_Buildings::operator()(CGenericBuilding& Generic)
{
	DrawGenericText(Constants::BuildingString, Generic.m_Origin);
}

void Draw_Buildings::operator()(CDispenser& Dispenser)
{
	DrawGenericText(Constants::DispenserString, Dispenser.m_Origin);
}

void Draw_Buildings::operator()(CTeleporter& Teleporter)
{
	DrawGenericText(Constants::TeleporterString, Teleporter.m_Origin);
}
