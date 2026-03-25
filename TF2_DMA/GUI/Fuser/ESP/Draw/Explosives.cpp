#include "pch.h"
#include "Explosives.h"
#include "TF2/IEntityList/IEntityList.h"
#include "General Drawing.h"
#include "TF2/Constants/Strings/EntityNames.h"
#include "GUI/Color Picker/Color Picker.h"

void Draw_Explosives::DrawAll()
{
	std::scoped_lock lk(IEntityList::m_ExplosiveMutex);

	for (auto& Explosive : IEntityList::m_Explosives)
		std::visit(Draw_Explosives{}, Explosive);
}

void Draw_Explosives::operator()(CRocket& Rocket)
{
	if (bHideFriendly && Rocket.IsFriendly())
		return;

	int LineNumber = 0;

	DrawGenericTextAtWorldPosition(Constants::RocketString, Rocket.m_Origin, LineNumber, ColorPicker::Rocket);
}

void Draw_Explosives::operator()(CStickybomb& Bomb)
{
	if (bHideFriendly && Bomb.IsFriendly())
		return;

	int LineNumber = 0;

	DrawGenericTextAtWorldPosition(Constants::StickyString, Bomb.m_Origin, LineNumber, ColorPicker::Sticky);
}
