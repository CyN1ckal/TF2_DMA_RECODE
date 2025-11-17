#include "pch.h"

#include "CBaseEntity.h"

#include "TF2/IEntityList/IEntityList.h"
#include "GUI/Color Picker/Color Picker.h"
#include "TF2/TF2.h"

using namespace IEntityList;

const bool CBaseEntityInfo::IsFriendly() const
{
	return m_TeamID == IEntityList::m_CTFPlayerInfoMap[IEntityList::m_LocalPlayerAddr].m_TeamID;
}

const ImVec4 CBaseEntityInfo::GetTeamColor() const
{
	if (m_TeamID == 2)
		return ColorPicker::RedTeam;

	if (m_TeamID == 3)
		return ColorPicker::BluTeam;

	return ColorPicker::Unknown;
}

const bool CBaseEntityInfo::IsDormant() const
{
	return m_DormantByte;
}

const float CBaseEntityInfo::DistanceFromLocalPlayerMeters() const
{
	auto& LocalPlayer = IEntityList::m_CTFPlayerInfoMap[IEntityList::m_LocalPlayerAddr];

	return m_Origin.Distance(LocalPlayer.m_Origin) / HammerUnitPerMeter;
}

const bool CBaseEntityInfo::IsLocalPlayer() const
{
	return m_bIsLocalPlayer;
}