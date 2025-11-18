#include "pch.h"

#include "CBaseEntity.h"

#include "TF2/Offsets/Offsets.h"

#include "TF2/IEntityList/IEntityList.h"

void CBaseEntity::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, bool bReadHealth)
{
	if (IsInvalid())
		return;

	uintptr_t PlayerIndexAddress = m_EntityAddress + Offsets::CBaseEntity::PlayerID;
	VMMDLL_Scatter_PrepareEx(vmsh, PlayerIndexAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_PlayerIndex), nullptr);

	uintptr_t DeadByteAddress = m_EntityAddress + Offsets::CBaseEntity::DeadByte;
	VMMDLL_Scatter_PrepareEx(vmsh, DeadByteAddress, sizeof(int8_t), reinterpret_cast<BYTE*>(&m_DeadByte), nullptr);

	uintptr_t ModelPtrAddress = m_EntityAddress + Offsets::CBaseEntity::pModel;
	VMMDLL_Scatter_PrepareEx(vmsh, ModelPtrAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ModelAddress), nullptr);

	if (bReadHealth)
	{
		uintptr_t CurrentHealthAddress = m_EntityAddress + Offsets::CBaseEntity::CurrentHealth;
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);
	}

	uintptr_t TeamIDAddress = m_EntityAddress + Offsets::CBaseEntity::TeamID;
	VMMDLL_Scatter_PrepareEx(vmsh, TeamIDAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_TeamID), nullptr);

	uintptr_t DormantByteAddress = m_EntityAddress + Offsets::CBaseEntity::DormantByte;
	VMMDLL_Scatter_PrepareEx(vmsh, DormantByteAddress, sizeof(int8_t), reinterpret_cast<BYTE*>(&m_DormantByte), nullptr);

	uintptr_t OriginAddress = m_EntityAddress + Offsets::CBaseEntity::Origin;
	VMMDLL_Scatter_PrepareEx(vmsh, OriginAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_Origin), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEntity::Finalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}

void CBaseEntity::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	uintptr_t DormantByteAddress = m_EntityAddress + Offsets::CBaseEntity::DormantByte;
	VMMDLL_Scatter_PrepareEx(vmsh, DormantByteAddress, sizeof(int8_t), reinterpret_cast<BYTE*>(&m_DormantByte), nullptr);

	uintptr_t DeadByteAddress = m_EntityAddress + Offsets::CBaseEntity::DeadByte;
	VMMDLL_Scatter_PrepareEx(vmsh, DeadByteAddress, sizeof(int8_t), reinterpret_cast<BYTE*>(&m_DeadByte), nullptr);

	uintptr_t OriginAddress = m_EntityAddress + Offsets::CBaseEntity::Origin;
	VMMDLL_Scatter_PrepareEx(vmsh, OriginAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_Origin), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CBaseEntity::QuickFinalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}

bool CBaseEntity::IsFriendly()
{
	return IEntityList::m_LocalPlayerTeamID == m_TeamID;
}

bool CBaseEntity::IsDormant()
{
	return m_DormantByte;
}

float CBaseEntity::DistanceFromLocalPlayer(bool bInMeters)
{
	auto LocalPlayerPos = IEntityList::GetLocalPlayerPos();
	auto Distance = m_Origin.Distance(LocalPlayerPos);

	if (bInMeters)
		Distance /= HammerUnitPerMeter;

	return Distance;
}

bool CBaseEntity::IsBlu()
{
	return m_TeamID == 3;
}

bool CBaseEntity::IsRed()
{
	return m_TeamID == 2;
}

bool CBaseEntity::IsSpectator()
{
	return m_TeamID == 1;
}

bool CBaseEntity::IsAlive()
{
	return m_DeadByte == 0;
}

bool CBaseEntity::IsDead()
{
	return m_DeadByte;
}
