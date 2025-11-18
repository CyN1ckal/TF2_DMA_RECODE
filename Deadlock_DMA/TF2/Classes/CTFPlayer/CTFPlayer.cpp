#include "pch.h"

#include "CTFPlayer.h"

#include "TF2/Offsets/Offsets.h"

#include "TF2/Constants/Strings/EntityNames.h"

#include "TF2/IEntityList/IEntityList.h"

void CTFPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEntity::PrepareRead_1(vmsh);

	uintptr_t ConditionBitsAddress = m_EntityAddress + Offsets::CTFPlayer::ConditionBits;
	VMMDLL_Scatter_PrepareEx(vmsh, ConditionBitsAddress, sizeof(ConditionBits), reinterpret_cast<BYTE*>(&m_ConditionBits), nullptr);

	uintptr_t BoneArrayPtr = m_EntityAddress + Offsets::CTFPlayer::BoneArray;
	VMMDLL_Scatter_PrepareEx(vmsh, BoneArrayPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneArrayAddress), nullptr);

	uintptr_t ClassID = m_EntityAddress + Offsets::CTFPlayer::ClassID;
	VMMDLL_Scatter_PrepareEx(vmsh, ClassID, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_PlayerClass), nullptr);
}

void CTFPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	CBaseEntity::Finalize();

	if (!m_BoneArrayAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneArrayAddress, sizeof(BoneArray), reinterpret_cast<BYTE*>(&m_BoneArray), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CTFPlayer::Finalize()
{
	if (m_BytesRead != sizeof(BoneArray))
		SetInvalid();

	if (IsInvalid()) return;

	UpdateLocalPlayerInfo();
}

void CTFPlayer::QuickFinalize()
{
	CBaseEntity::QuickFinalize();

	if (IsInvalid()) return;

	UpdateLocalPlayerInfo();
}

void CTFPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	CBaseEntity::QuickRead(vmsh);

	uintptr_t CurrentHealthAddress = m_EntityAddress + Offsets::CBaseEntity::CurrentHealth;
	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneArrayAddress, sizeof(BoneArray), reinterpret_cast<BYTE*>(&m_BoneArray), nullptr);
}

void CTFPlayer::UpdateLocalPlayerInfo()
{
	std::scoped_lock lk(IEntityList::m_LocalPlayerMutex);
	if (m_EntityAddress == IEntityList::m_LocalPlayerAddr)
	{
		SetLocalPlayer();
		IEntityList::m_LocalPlayerPos = m_Origin;
	}
}

void CTFPlayer::SetLocalPlayer()
{
	m_Flags |= 0x2;
	IEntityList::m_LocalPlayerTeamID = m_TeamID;
}

bool CTFPlayer::IsLocalPlayer()
{
	return (m_Flags & 0x2);
}

std::string_view CTFPlayer::GetPlayerClassName()
{
	switch (m_PlayerClass)
	{
	case eTFClass::Scout:
		return Constants::ScoutString;
	case eTFClass::Soldier:
		return Constants::SoldierString;
	case eTFClass::Pyro:
		return Constants::PyroString;
	case eTFClass::Demo:
		return Constants::DemoString;
	case eTFClass::Heavy:
		return Constants::HeavyString;
	case eTFClass::Engineer:
		return Constants::EngineerString;
	case eTFClass::Medic:
		return Constants::MedicString;
	case eTFClass::Sniper:
		return Constants::SniperString;
	case eTFClass::Spy:
		return Constants::SpyString;
	default:
		return Constants::NullString;
	}
}

Vector3 CTFPlayer::GetHeadPosition()
{
	switch (m_PlayerClass)
	{
	case eTFClass::Scout:
	case eTFClass::Medic:
	case eTFClass::Soldier:
	case eTFClass::Pyro:
	case eTFClass::Spy:
	case eTFClass::Heavy:
	case eTFClass::Sniper:
		return m_BoneArray[6].GetPosition();
	case eTFClass::Demo:
		return m_BoneArray[16].GetPosition();
	case eTFClass::Engineer:
		return m_BoneArray[8].GetPosition();
	default:
		return { 0.0f,0.0f,0.0f };
	}
}

Vector3 CTFPlayer::GetChestPosition()
{
	switch (m_PlayerClass)
	{
	case eTFClass::Soldier:
	case eTFClass::Demo:
	case eTFClass::Sniper:
	case eTFClass::Medic:
	case eTFClass::Scout:
	case eTFClass::Spy:
	case eTFClass::Pyro:
		return m_BoneArray[2].GetPosition();
	case eTFClass::Engineer:
	case eTFClass::Heavy:
		return m_BoneArray[3].GetPosition();
	default:
		return { 0.0f,0.0f,0.0f };
	}
}

bool CTFPlayer::IsInCond(ETFCond Cond)
{
	auto Bits = m_ConditionBits.GetBits();
	return Bits.test(static_cast<int16_t>(Cond));
}

const uint32_t CTFPlayer::GetMaxHealth()
{
	switch (m_PlayerClass)
	{
	case eTFClass::Scout:
		return 125;
	case eTFClass::Soldier:
		return 200;
	case eTFClass::Pyro:
		return 175;
	case eTFClass::Demo:
		return 175;
	case eTFClass::Heavy:
		return 300;
	case eTFClass::Engineer:
		return 125;
	case eTFClass::Medic:
		return 150;
	case eTFClass::Sniper:
		return 125;
	case eTFClass::Spy:
		return 125;
	default:
		return 0;
	}
}
