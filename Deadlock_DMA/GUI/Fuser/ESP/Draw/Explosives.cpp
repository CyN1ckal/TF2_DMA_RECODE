#include "pch.h"
#include "Explosives.h"
#include "TF2/IEntityList/IEntityList.h"
#include "General Drawing.h"
#include "TF2/Constants/Strings/EntityNames.h"

void Draw_Explosives::DrawAll()
{
	std::scoped_lock lk(IEntityList::m_ExplosiveMutex);

	for (auto& Explosive : IEntityList::m_Explosives)
		std::visit(Draw_Explosives{}, Explosive);
}

void Draw_Explosives::operator()(CRocket& Rocket)
{
	DrawGenericText(Constants::RocketString, Rocket.m_Origin);
}

void Draw_Explosives::operator()(CStickybomb& Bomb)
{
	DrawGenericText(Constants::StickyString, Bomb.m_Origin);
}
