#pragma once

#include "TF2/Classes/CBaseEntity/CBaseEntity.h"
#include "TF2/Classes/CTFPlayer/CTFPlayer.h"
#include "TF2/Classes/CSentryGun/CSentryGun.h"
#include "TF2/Classes/CTeleporter/CTeleporter.h"
#include "TF2/Classes/CDispenser/CDispenser.h"
#include "TF2/Classes/CRocket/CRocket.h"
#include "TF2/Classes/CStickybomb/CStickybomb.h"
#include "TF2/Classes/CAmmoPack/CAmmoPack.h"
#include "TF2/Classes/CHealthPack/CHealthPack.h"

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

	inline uintptr_t m_IClientEntityListAddr{ 0x0 };

	void UpdateLocalPlayerAddress(DMA_Connection* Conn);
	Vector3 GetLocalPlayerPos();
	inline std::mutex m_LocalPlayerMutex{};
	inline uintptr_t m_LocalPlayerAddr{ 0x0 };
	inline uint32_t m_LocalPlayerTeamID{ 0 };
	inline Vector3 m_LocalPlayerPos{ 0.0f,0.0f,0.0f };

	inline std::vector<uintptr_t> m_AllEntityAddresses;

	bool UpdateAllCTFPlayerInfo(DMA_Connection* Conn);
	bool UpdateExistingCTFPlayerInfo(DMA_Connection* Conn);
	inline std::mutex m_PlayersMutex{};
	inline std::vector<CTFPlayer> m_Players{};
	inline std::vector<uintptr_t> m_CTFPlayerAddresses;

	bool UpdateAllBuildings(DMA_Connection* Conn);
	bool UpdateExistingBuildings(DMA_Connection* Conn);
	using Buildings = std::variant<CSentryGun, CTeleporter, CDispenser>;
	inline std::mutex m_BuildingMutex{};
	inline std::vector<Buildings> m_Buildings{};
	inline std::vector<uintptr_t> m_SentryAddresses;
	inline std::vector<uintptr_t> m_TeleporterAddresses;
	inline std::vector<uintptr_t> m_DispenserAddresses;

	bool UpdateAllExplosives(DMA_Connection* Conn);
	bool UpdateExistingExplosives(DMA_Connection* Conn);
	using Explosives = std::variant<CRocket, CStickybomb>;
	inline std::mutex m_ExplosiveMutex{};
	inline std::vector<Explosives> m_Explosives{};
	inline std::vector<uintptr_t> m_RocketAddresses;
	inline std::vector<uintptr_t> m_StickybombAddresses;

	bool UpdateAllConsumables(DMA_Connection* Conn);
	bool UpdateExistingConsumables(DMA_Connection* Conn);
	using Consumables = std::variant<CHealthPack, CAmmoPack>;
	inline std::mutex m_ConsumableMutex{};
	inline std::vector<Consumables> m_Consumables{};
	inline std::vector<uintptr_t> m_HealthPackAddresses{};
	inline std::vector<uintptr_t> m_AmmoPackAddresses{};
};