#include "pch.h"
#include "Players.h"
#include "TF2/IEntityList/IEntityList.h"
#include "Engine/Math/MatrixMath.h"
#include "TF2/Constants/Bone Pairs/BonePairs.h"

void Draw_Players::DrawAll()
{
	std::scoped_lock PlayerLock(IEntityList::m_PlayersMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	for (auto& Player : IEntityList::m_Players)
	{
		Vector2 ScreenPos;
		if (!WorldToScreen(Player.m_Origin, ScreenPos)) continue;

		DrawList->AddCircleFilled({ WindowPos.x + ScreenPos.x, WindowPos.y + ScreenPos.y }, 5.0f, IM_COL32(255, 0, 0, 255));

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
		DrawList->AddLine(LineStartPos, LineEndPos, ImColor(255, 255, 255), 2.0);
	}
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