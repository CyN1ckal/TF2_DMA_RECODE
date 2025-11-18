#include "pch.h"
#include "Buildings.h"
#include "TF2/IEntityList/IEntityList.h"
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

	Vector2 ScreenPos{};
	if (!WorldToScreen(Gun.m_Origin, ScreenPos)) return;
	auto WindowPos = ImGui::GetWindowPos();
	ScreenPos += {WindowPos.x, WindowPos.y};

	int LineNumber = 0;

	DrawGenericBuildingNameTag(Constants::SentryString, Gun, ScreenPos, LineNumber);

	if (bSentryHealthBar)
		DrawBuildingHealthBar(Gun, ScreenPos, LineNumber);

	if (bSentryAmmoBar)
		DrawSentryAmmoBar(Gun, ScreenPos, LineNumber);
}

void Draw_Buildings::operator()(CDispenser& Dispenser)
{
	if (bHideFriendly && Dispenser.IsFriendly())
		return;

	if (bHideDormant && Dispenser.IsDormant())
		return;

	Vector2 ScreenPos{};
	if (!WorldToScreen(Dispenser.m_Origin, ScreenPos)) return;
	auto WindowPos = ImGui::GetWindowPos();
	ScreenPos += {WindowPos.x, WindowPos.y};

	int LineNumber = 0;

	DrawGenericBuildingNameTag(Constants::DispenserString, Dispenser, ScreenPos, LineNumber);

	if (bDispenserHealthBar)
		DrawBuildingHealthBar(Dispenser, ScreenPos, LineNumber);
}

void Draw_Buildings::operator()(CTeleporter& Teleporter)
{
	if (bHideFriendly && Teleporter.IsFriendly())
		return;

	if (bHideDormant && Teleporter.IsDormant())
		return;

	Vector2 ScreenPos{};
	if (!WorldToScreen(Teleporter.m_Origin, ScreenPos)) return;
	auto WindowPos = ImGui::GetWindowPos();
	ScreenPos += {WindowPos.x, WindowPos.y};

	int LineNumber = 0;

	DrawGenericBuildingNameTag(Constants::TeleporterString, Teleporter, ScreenPos, LineNumber);

	if (bTeleporterHealthBar)
		DrawBuildingHealthBar(Teleporter, ScreenPos, LineNumber);
}

void Draw_Buildings::DrawGenericBuildingNameTag(const std::string& Name, CGenericBuilding& Building, const Vector2& ScreenPos, int& LineNumber)
{
	std::string NameTagString = std::format("({0:d}) {2:s} [{1:.0f}m]", Building.m_CurrentBuildingLevel, Building.DistanceFromLocalPlayer(), Name.c_str());

	DrawGenericTextAtScreenPosition(NameTagString, ScreenPos, LineNumber, (Building.IsBlu()) ? ColorPicker::BluTeam : ColorPicker::RedTeam);
}

void Draw_Buildings::DrawBuildingHealthBar(CGenericBuilding& Building, const Vector2& ScreenPosition, int& LineNumber)
{
	DrawGenericStatusBar(static_cast<float>(Building.m_CurrentHealth), static_cast<float>(Building.m_MaxHealth), (Building.IsBlu()) ? ColorPicker::BluTeam : ColorPicker::RedTeam, ImGui::GetWindowDrawList(), ScreenPosition, LineNumber);
	LineNumber--;
	DrawGenericTextAtScreenPosition(std::format("{}", Building.m_CurrentHealth), ScreenPosition, LineNumber);
}

void Draw_Buildings::DrawSentryAmmoBar(CSentryGun& Gun, const Vector2& ScreenPosition, int& LineNumber)
{
	DrawGenericStatusBar(Gun.m_NumBullets, Gun.GetMaxAmmo(), (Gun.IsBlu()) ? ColorPicker::BluTeam : ColorPicker::RedTeam, ImGui::GetWindowDrawList(), ScreenPosition, LineNumber);
	LineNumber--;
	DrawGenericTextAtScreenPosition(std::format("{}", Gun.m_NumBullets), ScreenPosition, LineNumber);
}