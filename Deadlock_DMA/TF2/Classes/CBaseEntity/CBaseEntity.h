#pragma once
#include "Engine/Vector3.h"

class CBaseEntity
{
public:
	Vector3 m_Origin{ 0.0f,0.0f,0.0f };
	uintptr_t m_EntityAddress{ 0 };
	uintptr_t m_ModelAddress{ 0 };
	uint32_t m_BytesRead{ 0 };
	uint32_t m_CurrentHealth{ 0 };
	uint32_t m_TeamID{ 0 };
	uint32_t m_PlayerIndex{ 0 };
	uint8_t m_Flags{ 0 };
	int8_t m_DormantByte{ 0x0 };

public:
	CBaseEntity(uintptr_t EntityAddress) : m_EntityAddress(EntityAddress)
	{
		if (m_EntityAddress == 0)
			SetInvalid();
	};
	void SetInvalid() {
		m_Flags |= 0x1;
	}
	bool IsInvalid() {
		return (m_Flags & 0x1);
	}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, bool bReadHealth = true);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	bool IsFriendly();
	bool IsDormant();
	float DistanceFromLocalPlayer(bool bInMeters = true);
	bool IsBlu();
	bool IsRed();
	bool IsSpectator();
};