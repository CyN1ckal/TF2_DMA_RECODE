#pragma once
#include "TF2/Classes/CTFPlayer/CTFPlayer.h"
#include "Engine/Vector2.h"

class Draw_Players
{
public:
	static void DrawAll();

private:
	static inline std::array<Vector2, MAX_BONES> m_ProjectedBoneCache{};
	static void ProjectBonesToCache(CTFPlayer& Player);
	static void DrawBones(CTFPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);
};