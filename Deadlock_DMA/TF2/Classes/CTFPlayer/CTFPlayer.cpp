#include "pch.h"

#include "CTFPlayer.h"

#include "TF2/Offsets/Offsets.h"

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
}

void CTFPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	CBaseEntity::QuickRead(vmsh);

	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneArrayAddress, sizeof(BoneArray), reinterpret_cast<BYTE*>(&m_BoneArray), reinterpret_cast<DWORD*>(&m_BytesRead));
}
