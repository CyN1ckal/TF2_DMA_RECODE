#include "pch.h"
#include "Players.h"
#include "TF2/IEntityList/IEntityList.h"
#include "Engine/Vector2.h"
#include "Engine/Math/MatrixMath.h"

void Draw_Players::DrawAll()
{
	std::scoped_lock PlayerLock(IEntityList::m_CTFPlayerInfoMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	for (auto&& [EntAddr, EntInfo] : IEntityList::m_CTFPlayerInfoMap)
	{
		if (EntInfo.m_bIsLocalPlayer)
			continue;

		Vector2 ScreenPos;
		if (!WorldToScreen(EntInfo.m_Origin, ScreenPos)) continue;

		DrawList->AddCircleFilled({ WindowPos.x + ScreenPos.x, WindowPos.y + ScreenPos.y }, 5.0f, IM_COL32(255, 0, 0, 255));
	}
}