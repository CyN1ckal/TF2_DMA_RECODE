#pragma once

#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include "TF2/Classes/CTFPlayer/CTFPlayer.h"
#include "TF2/Classes/CSentryGun/CSentryGun.h"
#include "TF2/Classes/CTeleporter/CTeleporter.h"
#include "TF2/Classes/CDispenser/CDispenser.h"

#include <variant>

namespace IEntityList
{
	bool Initialize(DMA_Connection* Conn);
	bool UpdateEntityAddresses(DMA_Connection* Conn);
	bool SortEntityAddresses(DMA_Connection* Conn);
	uintptr_t GetIClientEntityListAddr();

	inline std::vector<uintptr_t> m_HealhPackModelAddresses;
	inline std::vector<uintptr_t> m_AmmoPackModelAddresses;
	bool PopulateModelAddresses(DMA_Connection* Conn);

	void UpdateLocalPlayerAddress(DMA_Connection* Conn);
	inline uintptr_t m_IClientEntityListAddr = 0x0;
	inline uintptr_t m_LocalPlayerAddr = 0x0;
	inline Vector3 m_LocalPlayerPos = { 0.0f,0.0f,0.0f };

	inline std::vector<uintptr_t> m_AllEntityAddresses;

	bool UpdateAllCTFPlayerInfo(DMA_Connection* Conn);
	bool UpdateExistingCTFPlayerInfo(DMA_Connection* Conn);
	inline std::mutex m_PlayersMutex{};
	inline std::vector<uintptr_t> m_CTFPlayerAddresses;
	inline std::vector<CTFPlayer> m_Players{};

	bool UpdateAllBuildings(DMA_Connection* Conn);
	bool UpdateExistingBuildings(DMA_Connection* Conn);
	using Buildings = std::variant<CSentryGun, CTeleporter, CDispenser>;
	inline std::mutex m_BuildingMutex{};
	inline std::vector<uintptr_t> m_SentryAddresses;
	inline std::vector<uintptr_t> m_TeleporterAddresses;
	inline std::vector<uintptr_t> m_DispenserAddresses;
	inline std::vector<Buildings> m_Buildings{};
};