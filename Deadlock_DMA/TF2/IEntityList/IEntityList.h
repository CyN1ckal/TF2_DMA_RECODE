#pragma once

#include "TF2/Classes/EntityClasses.h"

namespace IEntityList
{
	bool Initialize(DMA_Connection* Conn);

	bool UpdateEntityAddresses(DMA_Connection* Conn);
	bool SortEntityAddresses(DMA_Connection* Conn);

	bool UpdateAllCTFPlayerInfo(DMA_Connection* Conn);
	bool UpdateExistingCTFPlayerInfo(DMA_Connection* Conn);

	bool UpdateBuildingInfo(DMA_Connection* Conn, bool bShouldClear);
	bool UpdateStickybombInfo(DMA_Connection* Conn);
	bool UpdateRocketInfo(DMA_Connection* Conn);
	bool UpdateHealthPackInfo(DMA_Connection* Conn);
	bool UpdateAmmoPackInfo(DMA_Connection* Conn);
	bool UpdateAnimatingInfo(DMA_Connection* Conn);
	bool UpdateCurrencyPackInfo(DMA_Connection* Conn);

	uintptr_t GetIClientEntityListAddr();
	bool PrepareCTFPlayerReads(uintptr_t PlayerAddr, VMMDLL_SCATTER_HANDLE vmsh, CTFPlayerInfo& pi);
	bool PrepareCTFPlayerBoneReads(uintptr_t PlayerAddr, VMMDLL_SCATTER_HANDLE vmsh, CTFPlayerInfo& pi);
	bool PrepareCBaseEntityReads(uintptr_t BuildingAddr, VMMDLL_SCATTER_HANDLE vmsh, CBaseEntityInfo& pi);
	bool PrepareCBuildingReads(uintptr_t BuildingAddr, VMMDLL_SCATTER_HANDLE vmsh, CBuilding& bi);
	bool PrepareCSentryReads(uintptr_t BuildingAddr, VMMDLL_SCATTER_HANDLE vmsh, CSentryGun& si);

	inline std::vector<uintptr_t> m_HealhPackModelAddresses;
	inline std::vector<uintptr_t> m_AmmoPackModelAddresses;
	bool PopulateModelAddresses(DMA_Connection* Conn);

	void UpdateLocalPlayerAddress(DMA_Connection* Conn);
	inline uintptr_t m_IClientEntityListAddr = 0x0;
	inline uintptr_t m_LocalPlayerAddr = 0x0;
	inline Vector3 m_LocalPlayerPos = { 0.0f,0.0f,0.0f };

	inline std::vector<uintptr_t> m_AllEntityAddresses;

	inline std::vector<uintptr_t> m_CTFPlayerAddresses;
	inline std::vector<uintptr_t> m_SentryAddresses;
	inline std::vector<uintptr_t> m_TeleportAddresses;
	inline std::vector<uintptr_t> m_DispenserAddresses;
	inline std::vector<uintptr_t> m_StickybombAddresses;
	inline std::vector<uintptr_t> m_RocketAddresses;
	inline std::vector<uintptr_t> m_AnimatingAddresses;
	inline std::vector<uintptr_t> m_AmmoPackAddresses;
	inline std::vector<uintptr_t> m_HealthPackAddresses;
	inline std::vector<uintptr_t> m_CurrencyPackAddresses;

	inline std::mutex m_CTFPlayerInfoMutex;
	inline std::unordered_map<uintptr_t, CTFPlayerInfo> m_CTFPlayerInfoMap;

	inline std::mutex m_SentryInfoMutex;
	inline std::unordered_map<uintptr_t, CSentryGun> m_SentryInfoMap;

	inline std::mutex m_TeleportInfoMutex;
	inline std::unordered_map<uintptr_t, CBuilding> m_TeleportInfoMap;

	inline std::mutex m_DispenserInfoMutex;
	inline std::unordered_map<uintptr_t, CBuilding> m_DispenserInfoMap;

	inline std::mutex m_StickybombInfoMutex;
	inline std::unordered_map<uintptr_t, CBaseEntityInfo> m_StickybombInfoMap;

	inline std::mutex m_RocketInfoMutex;
	inline std::unordered_map<uintptr_t, CBaseEntityInfo> m_RocketInfoMap;

	inline std::mutex m_HealthPackInfoMutex;
	inline std::unordered_map<uintptr_t, CBaseEntityInfo> m_HealthPackInfoMap;

	inline std::mutex m_AmmoPackInfoMutex;
	inline std::unordered_map<uintptr_t, CBaseEntityInfo> m_AmmoPackInfoMap;

	inline std::mutex m_CurrencyPackInfoMutex;
	inline std::unordered_map<uintptr_t, CBaseEntityInfo> m_CurrencyPackInfoMap;

	inline std::mutex m_AnimatingInfoMutex;
	inline std::unordered_map<uintptr_t, CBaseEntityInfo> m_AnimatingInfoMap;
};