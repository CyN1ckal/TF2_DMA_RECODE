#include "pch.h"
#include "Buildings.h"
#include "TF2/IEntityList/IEntityList.h"
#include "Engine/Math/MatrixMath.h"
#include "TF2/Constants/Strings/EntityNames.h"
#include "GUI/Color Picker/Color Picker.h"
#include "General Drawing.h"

void Draw_Buildings::DrawAll()
{
	if (!bMasterToggle)
		return;

	std::scoped_lock lk(IEntityList::m_BuildingMutex);

	for (auto& Building : IEntityList::m_Buildings)
		std::visit(Draw_Buildings{}, Building);
}



void Draw_Buildings::operator()(CSentryGun& Gun)
{
	if (bHideFriendly && Gun.IsFriendly())
		return;

	if (bHideDormant && Gun.IsDormant())
		return;
	
	DrawGenericBuildingNameTag(Constants::SentryString, Gun);
}

void Draw_Buildings::operator()(CDispenser& Dispenser)
{
	if (bHideFriendly && Dispenser.IsFriendly())
		return;

	if (bHideDormant && Dispenser.IsDormant())
		return;

	DrawGenericBuildingNameTag(Constants::DispenserString, Dispenser);
}

void Draw_Buildings::operator()(CTeleporter& Teleporter)
{
	if (bHideFriendly && Teleporter.IsFriendly())
		return;

	if (bHideDormant && Teleporter.IsDormant())
		return;

	DrawGenericBuildingNameTag(Constants::TeleporterString, Teleporter);
}

void Draw_Buildings::DrawGenericBuildingNameTag(const std::string& Name, CGenericBuilding& Building)
{
	std::string BuildingString = std::format("({0:d}) {2:s} [{1:.0f}m]", Building.m_CurrentBuildingLevel, Building.DistanceFromLocalPlayer(), Name.c_str());

	auto TextSize = ImGui::CalcTextSize(BuildingString.c_str());

	Vector2 ScreenPos{};
	if (!WorldToScreen(Building.m_Origin, ScreenPos)) return;

	auto WindowPos = ImGui::GetWindowPos();
	ScreenPos += {WindowPos.x, WindowPos.y};

	auto DrawList = ImGui::GetWindowDrawList();
	DrawList->AddText({ ScreenPos.x - (TextSize.x * 0.5f), ScreenPos.y }, (Building.IsBlu()) ? ColorPicker::BluTeam : ColorPicker::RedTeam, BuildingString.c_str());
}