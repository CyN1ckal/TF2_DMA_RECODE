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

	if (m_EntityAddress == IEntityList::m_LocalPlayerAddr)
		SetLocalPlayer();
}

void CTFPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	CBaseEntity::QuickRead(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneArrayAddress, sizeof(BoneArray), reinterpret_cast<BYTE*>(&m_BoneArray), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CTFPlayer::SetLocalPlayer()
{
	m_Flags |= 0x2;
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
