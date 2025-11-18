#include "pch.h"
#include "Consumables.h"
#include "TF2/IEntityList/IEntityList.h"
#include "Engine/Math/MatrixMath.h"
#include "General Drawing.h"
#include "GUI/Color Picker/Color Picker.h"
#include "TF2/Constants/Strings/EntityNames.h"

void Draw_Consumables::DrawAll()
{
	if (bMasterToggle == false) return;

	std::scoped_lock lk(IEntityList::m_ConsumableMutex);

	for (auto& Consumable : IEntityList::m_Consumables)
		std::visit(Draw_Consumables{}, Consumable);
}

void Draw_Consumables::operator()(CAmmoPack& AmmoPack)
{
	if (bDrawAmmoPacks == false) return;

	if (AmmoPack.IsInvalid()) return;

	std::string DisplayString = std::format("{0:s} [{1:.0f}m]", Constants::AmmoPackString, AmmoPack.DistanceFromLocalPlayer());

	DrawGenericText(DisplayString, AmmoPack.m_Origin, ColorPicker::AmmoPack);
}

void Draw_Consumables::operator()(CHealthPack& HealthPack)
{
	if (bDrawHealthPacks == false) return;

	if (HealthPack.IsInvalid()) return;

	std::string DisplayString = std::format("{0:s} [{1:.0f}m]", Constants::HealthPackString, HealthPack.DistanceFromLocalPlayer());

	DrawGenericText(DisplayString, HealthPack.m_Origin, ColorPicker::HealthPack);
}
