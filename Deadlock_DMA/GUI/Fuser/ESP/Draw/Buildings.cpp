#include "pch.h"

#include "Buildings.h"

#include "TF2/IEntityList/IEntityList.h"

#include "Engine/Math/MatrixMath.h"
#include "TF2/Constants/Strings/EntityNames.h"

void Draw_Buildings::DrawAll()
{
	std::scoped_lock lk(IEntityList::m_BuildingMutex);

	for (auto& Building : IEntityList::m_Buildings)
		std::visit(Draw_Buildings{}, Building);
}

void DrawGenericText(const std::string& Name, Vector3& Origin)
{
	Vector2 ScreenPos{};
	if (!WorldToScreen(Origin, ScreenPos)) return;

	auto TextSize = ImGui::CalcTextSize(Name.c_str());

	ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x * 0.5f), ScreenPos.y });

	ImGui::Text(Name.c_str());
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
