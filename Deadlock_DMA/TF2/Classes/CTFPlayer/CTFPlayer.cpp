#include "pch.h"

#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include "Engine/Matrix.h"
#include "TF2/Constants/Strings/ConstStrings.h"
#include "TF2/Constants/eTFCond.h"
#include "TF2/Constants/eTFClass.h"
#include "CTFPlayer.h"

#include "GUI/Color Picker/Color Picker.h"
#include "TF2/IEntityList/IEntityList.h"

const std::string CTFPlayerInfo::m_GetClassName() const
{
	using namespace Constants;

	switch (m_ClassID)
	{
	case 1:
		return ScoutString;
	case 2:
		return SniperString;
	case 3:
		return SoldierString;
	case 4:
		return DemoString;
	case 5:
		return MedicString;
	case 6:
		return HeavyString;
	case 7:
		return PyroString;
	case 8:
		return SpyString;
	case 9:
		return EngineerString;
	default:
		return NullString;
	};
}

const std::string CTFPlayerInfo::m_GetTeamName() const
{
	switch (m_TeamID)
	{
	case 1:
		return "Spectator";
	case 2:
		return "Red";
	case 3:
		return "Blue";
	default:
		return "Null";
	};
}

const bool CTFPlayerInfo::m_IsAlive() const
{
	return m_DeadByte == 0;
}

const float CTFPlayerInfo::m_GetDistanceToLocalPlayer() const
{
	return m_Origin.Distance(IEntityList::m_LocalPlayerPos);
}

const float CTFPlayerInfo::m_GetDistanceToLocalPlayerMeters() const
{
	return m_Origin.Distance(IEntityList::m_LocalPlayerPos) / HammerUnitPerMeter;
}

const uint32_t CTFPlayerInfo::m_GetHeadBoneIndex() const
{
	eTFClass ClassID = static_cast<eTFClass>(m_ClassID);
	switch (ClassID)
	{
	case eTFClass::Scout:
	case eTFClass::Medic:
	case eTFClass::Soldier:
	case eTFClass::Pyro:
	case eTFClass::Spy:
	case eTFClass::Heavy:
	case eTFClass::Sniper:
		return 6;
	case eTFClass::Demo:
		return 16;
	case eTFClass::Engineer:
		return 8;
	default:
		return 0;
	}

	return 0;
}

const uint32_t CTFPlayerInfo::m_GetBodyBoneIndex() const
{
	eTFClass ClassID = static_cast<eTFClass>(m_ClassID);
	switch (ClassID)
	{
	case eTFClass::Soldier:
	case eTFClass::Demo:
	case eTFClass::Sniper:
	case eTFClass::Medic:
	case eTFClass::Scout:
	case eTFClass::Spy:
	case eTFClass::Pyro:
		return 2;
	case eTFClass::Engineer:
	case eTFClass::Heavy:
		return 3;
	default:
		return 0;
	}

	return 0;
}

const bool CTFPlayerInfo::m_InCondition(ETFCond Condition) const
{
	uint32_t Cond = Condition;

	auto BitSet = m_ConditionBits.GetBits();

	if (BitSet.test(Cond)) return true;

	return false;
}

const bool CTFPlayerInfo::m_IsValidESPTarget() const
{
	if (m_TeamID < 2 || m_TeamID > 3) return 0;

	if (m_ClassID < 1 || m_ClassID > 9) return 0;

	if (m_IsDead()) return 0;

	return 1;
}

const bool CTFPlayerInfo::m_IsDead() const
{
	if (m_DeadByte == 0x17 && m_CurrentHealth == 1) return 1;

	return 0;
}

const ImVec4 CTFPlayerInfo::m_GetESPColor() const
{
	if (IsLocalPlayer()) return ColorPicker::LocalPlayer;

	if (IsDormant()) return ColorPicker::Dormant;

	if (m_TeamID == 1)
		return ColorPicker::Spectator;

	if (m_TeamID == 2)
		return ColorPicker::RedTeam;

	if (m_TeamID == 3)
		return ColorPicker::BluTeam;

	return ColorPicker::Unknown;
}

const uint16_t CTFPlayerInfo::m_GetMaxHealth() const
{
	switch (m_ClassID)
	{
	case 1: // Scout
	case 2: // Sniper
	case 8: // Spy
	case 9: // Engineer
		return 125;

	case 5: // Medic
		return 150;

	case 4: // Demo
	case 7: // Pyro
		return 175;

	case 3: // Soldier
		return 200;

	case 6: // Heavy
		return 300;
	}

	return 0;
}
