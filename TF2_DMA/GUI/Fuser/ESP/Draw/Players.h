#pragma once
#include "TF2/Classes/CTFPlayer/CTFPlayer.h"
#include "Engine/Vector2.h"

class Draw_Players
{
public:
	static void DrawAll();

public:
	static inline bool bMasterToggle{ true };
	static inline bool bHideFriendly{ true };
	static inline bool bHideDormant{ true };
	static inline bool bHighlightMedic{ true };

private:
	static inline std::array<Vector2, MAX_BONES> m_ProjectedBoneCache{};
	static void ProjectBonesToCache(CTFPlayer& Player);
	static void DrawBones(CTFPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);
	static void DrawNametag(CTFPlayer& Player, const Vector2& ScreenPos, ImDrawList* DrawList, int& LineNumber);
	static void DrawConditionString(CTFPlayer& Player, const Vector2& ScreenPos, ImDrawList* DrawList, int& LineNumber);
	static void DrawStatusBar(CTFPlayer& Player, const Vector2& ScreenPos, ImDrawList* DrawList, int& LineNumber);
};