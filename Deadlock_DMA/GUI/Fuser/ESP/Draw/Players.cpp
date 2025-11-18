#include "pch.h"
#include "Players.h"
#include "TF2/IEntityList/IEntityList.h"
#include "Engine/Math/MatrixMath.h"
#include "TF2/Constants/Bone Pairs/BonePairs.h"
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
	auto test = Bones[0];

	for (auto Pair : *BoneList)
	{
		Vector2 ScreenPos_1{};
		Vector2 ScreenPos_2{};
		if (!WorldToScreen(Bones[Pair.first].GetPosition(), ScreenPos_1)) continue;
		if (!WorldToScreen(Bones[Pair.second].GetPosition(), ScreenPos_2)) continue;

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