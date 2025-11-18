#include "pch.h"
#include "Players.h"
#include "TF2/IEntityList/IEntityList.h"
#include "Engine/Math/MatrixMath.h"
#include "TF2/Constants/Bone Pairs/BonePairs.h"
#include "TF2/Constants/Strings/StatusEffects.h"
#include "GUI/Color Picker/Color Picker.h"

void Draw_Players::DrawAll()
{
	if (!bMasterToggle)
		return;

	std::scoped_lock PlayerLock(IEntityList::m_PlayersMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	for (auto& Player : IEntityList::m_Players)
	{
		if (Player.IsInvalid()) continue;

		if (Player.IsLocalPlayer()) continue;

		if (Player.IsDead()) continue;

		if (bHideFriendly && Player.IsFriendly()) continue;

		if (bHideDormant && Player.IsDormant()) continue;

		Vector2 ScreenPos;
		if (!WorldToScreen(Player.m_Origin, ScreenPos)) continue;

		ScreenPos += {WindowPos.x, WindowPos.y};

		int LineNumber = 0;

		DrawNametag(Player, ScreenPos, DrawList, LineNumber);

		DrawBones(Player, WindowPos, DrawList);
	}
}

void Draw_Players::DrawBones(CTFPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList)
{
	auto BoneList = GetBonePairsForClass(Player.m_PlayerClass);
	if (BoneList == nullptr) return;

	auto& Bones = Player.m_BoneArray;

	for (auto Pair : *BoneList)
	{
		Vector2 ScreenPos_1{};
		Vector2 ScreenPos_2{};
		auto BoneWorldPos_1 = Bones[Pair.first].GetPosition();
		auto BoneWorldPos_2 = Bones[Pair.second].GetPosition();
		if (BoneWorldPos_1.Distance(BoneWorldPos_2) > (2.0f * HammerUnitPerMeter)) continue;

		if (!WorldToScreen(BoneWorldPos_1, ScreenPos_1)) continue;
		if (!WorldToScreen(BoneWorldPos_2, ScreenPos_2)) continue;

		ImVec2 LineStartPos{ WindowPos.x + ScreenPos_1.x, WindowPos.y + ScreenPos_1.y };
		ImVec2 LineEndPos{ WindowPos.x + ScreenPos_2.x, WindowPos.y + ScreenPos_2.y };
		DrawList->AddLine(LineStartPos, LineEndPos, ColorPicker::Skeleton, 2.0);
	}
}

void Draw_Players::DrawNametag(CTFPlayer& Player, const Vector2& ScreenPos, ImDrawList* DrawList, int& LineNumber)
{
	std::string DisplayString = std::format("{0:d} {1:s} [{2:.0f}m]", Player.m_CurrentHealth, Player.GetPlayerClassName(), Player.DistanceFromLocalPlayer());

	auto TextSize = ImGui::CalcTextSize(DisplayString.c_str());

	ImVec2 TextPos{ ScreenPos.x - (TextSize.x * 0.5f), ScreenPos.y + (TextSize.y * LineNumber) };

	DrawList->AddText(TextPos, (Player.IsBlu()) ? ColorPicker::BluTeam : ColorPicker::RedTeam, DisplayString.c_str());

	LineNumber++;

	DrawConditionString(Player, ScreenPos, DrawList, LineNumber);
}

std::vector<std::string> ConditionStrings{};
void Draw_Players::DrawConditionString(CTFPlayer& Player, const Vector2& ScreenPos, ImDrawList* DrawList, int& LineNumber)
{
	ConditionStrings.clear();

	if (Player.IsInCond(ETFCond::TF_COND_CRITBOOSTED))
		ConditionStrings.emplace_back(Status::Kritz);
	if (Player.IsInCond(ETFCond::TF_COND_INVULNERABLE))
		ConditionStrings.emplace_back(Status::Ubered);
	if (Player.IsInCond(ETFCond::TF_COND_SHIELD_CHARGE))
		ConditionStrings.emplace_back(Status::Charging);
	if (Player.IsInCond(ETFCond::TF_COND_BURNING))
		ConditionStrings.emplace_back(Status::Burning);
	if (Player.IsInCond(ETFCond::TF_COND_URINE))
		ConditionStrings.emplace_back(Status::Jarate);
	if (Player.IsInCond(ETFCond::TF_COND_GAS))
		ConditionStrings.emplace_back(Status::Gas);
	if (Player.IsInCond(ETFCond::TF_COND_MAD_MILK))
		ConditionStrings.emplace_back(Status::Milk);
	if (Player.IsInCond(ETFCond::TF_COND_AIMING))
		ConditionStrings.emplace_back(Status::Scoped);
	if (Player.IsInCond(ETFCond::TF_COND_STEALTHED))
		ConditionStrings.emplace_back(Status::Cloaked);

	if (ConditionStrings.empty())
		return;

	std::string FinalConditionString;
	for (auto& String : ConditionStrings)
	{
		FinalConditionString += String;
		FinalConditionString += ", ";
	}
	FinalConditionString.pop_back();
	FinalConditionString.pop_back();

	auto TextSize = ImGui::CalcTextSize(FinalConditionString.c_str());
	ImVec2 TextPos{ ScreenPos.x - (TextSize.x * 0.5f), ScreenPos.y + (TextSize.y * LineNumber) };
	DrawList->AddText(TextPos, ImColor(255, 255, 255), FinalConditionString.c_str());
	LineNumber++;
}

/*
	This function would be useful if the total number of bones can be reduced.
	Currently, on average each player requires around 24 projections for bones.
	If that number exceeds the MAX_BONES, it would be beneficial to cache them.
	Currently, MAX_BONES(49) > 24 total projections per player
*/
void Draw_Players::ProjectBonesToCache(CTFPlayer& Player)
{
	m_ProjectedBoneCache.fill({ -1.0f, -1.0f });

	for (auto i = 0; i < MAX_BONES; ++i)
	{
		Vector3 BoneWorldPos = Player.m_BoneArray[i].GetPosition();

		Vector2 ScreenPos;
		if (WorldToScreen(BoneWorldPos, ScreenPos))
			m_ProjectedBoneCache[i] = ScreenPos;
	}
}