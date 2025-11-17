#include "pch.h"	

#include "TF2/Classes/EntityClasses.h"
#include "IEntityList.h"
#include "TF2/Offsets/Offsets.h"
#include "DMA/DMA.h"
#include "TF2/TF2.h"
#include "TF2/Classes/Misc.h"
#include "Engine/Vector3.h"

bool IEntityList::Initialize(DMA_Connection* Conn)
{
	UpdateEntityAddresses(Conn);

	return true;
}

std::vector<CEntityEntry> m_RawEntityList;
bool IEntityList::UpdateEntityAddresses(DMA_Connection* Conn)
{
	if (!m_IClientEntityListAddr) [[unlikely]]
	{
		m_IClientEntityListAddr = GetIClientEntityListAddr();

		if (!m_IClientEntityListAddr) [[unlikely]]
		{
			std::println("[MyEntityList] Failed to get IClientEntityList address.");

			return false;
		}

		//std::println("[MyEntityList] IClientEntityList address: 0x{:X}", m_IClientEntityListAddr);
	}

	auto& Proc = TF2::Proc();

	CClientEntityListHeader ClientEntityListHeader = Proc.ReadMem<CClientEntityListHeader>(Conn, m_IClientEntityListAddr);

	if (!ClientEntityListHeader.NumServerEnts)
	{
		std::println("[MyEntityList] NumServerEnts is 0, cannot update entity addresses.");

		return false;
	}

	if (!ClientEntityListHeader.MaxServerEnts) [[unlikely]]
	{
		std::println("[MyEntityList] MaxServerEnts is 0, cannot update entity addresses.");

		return false;
	}

	if (ClientEntityListHeader.MaxUsedServerIndex == 0xFFFFFFFF) [[unlikely]]
	{
		std::println("[MyEntityList] MaxUsedServerIndex is invalid, cannot update entity addresses.");

		return false;
	}

	m_RawEntityList.clear();
	m_RawEntityList.resize(ClientEntityListHeader.MaxUsedServerIndex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_PrepareEx(vmsh, m_IClientEntityListAddr + sizeof(CClientEntityListHeader), m_RawEntityList.size() * sizeof(CEntityEntry), reinterpret_cast<BYTE*>(m_RawEntityList.data()), nullptr);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_AllEntityAddresses.clear();
	m_AllEntityAddresses.reserve(ClientEntityListHeader.MaxUsedServerIndex);

	for (auto& entry : m_RawEntityList)
	{
		if (entry.BaseEntitiesIndex == 0 || entry.BaseEntitiesIndex == 0xFFFF)
			continue;

		m_AllEntityAddresses.push_back(entry.pEntity);
	}

	std::println("[MyEntityList] Found {0:d} valid entity addresses.", m_AllEntityAddresses.size());

	SortEntityAddresses(Conn);

	UpdateAllCTFPlayerInfo(Conn);

	UpdateBuildingInfo(Conn, true);

	UpdateStickybombInfo(Conn);

	UpdateRocketInfo(Conn);

	UpdateAnimatingInfo(Conn);

	UpdateCurrencyPackInfo(Conn);

	return true;
}

struct VTableInfo
{
	uintptr_t ClientClassFnAddr = 0;
	uintptr_t ClientClassAddr = 0;
	uint32_t ClientClassOffset = 0;
	uint32_t ClassID = 0;
};
struct EntityInfo
{
	uintptr_t VTableAddress = 0;
	uintptr_t ModelAddress = 0;
};

// std::unordered_map< EntityAddress, EntityInfo >
std::unordered_map<uintptr_t, EntityInfo>EntityVTableMap;

// std::unordered_map< VTableAddress, VTableInfo >
std::unordered_map<uintptr_t, VTableInfo> VTableMap;

bool IEntityList::SortEntityAddresses(DMA_Connection* Conn)
{
	if (m_AllEntityAddresses.empty()) [[unlikey]]
	{
		std::println("[MyEntityList] No entity addresses found, cannot update entity classes.");

		return false;
	}

	EntityVTableMap.clear();
	EntityVTableMap.reserve(m_AllEntityAddresses.size());

	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_AllEntityAddresses.size(); i++)
	{
		auto EntAddr = m_AllEntityAddresses[i];

		EntityVTableMap[EntAddr] = { 0x0 };
		VMMDLL_Scatter_PrepareEx(vmsh, EntAddr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&EntityVTableMap[EntAddr].VTableAddress), nullptr);

		auto ModelAddr = EntAddr + Offsets::CBaseEntity::pModel;
		VMMDLL_Scatter_PrepareEx(vmsh, ModelAddr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&EntityVTableMap[EntAddr].ModelAddress), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto&& [EntAddr, VTableAddr] : EntityVTableMap)
	{
		if (VTableMap.find(VTableAddr.VTableAddress) == VTableMap.end())
		{
			VTableMap[VTableAddr.VTableAddress] = {};
		}
	}

	//std::println("[MyEntityList] Found {0:d} unique entity classes.", VTableMap.size());

	vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto&& [VTableAddr, VTableInfo] : VTableMap)
	{
		uintptr_t GetClientClassPtr = VTableAddr + (sizeof(uintptr_t) * 2);
		VMMDLL_Scatter_PrepareEx(vmsh, GetClientClassPtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&VTableInfo.ClientClassFnAddr), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto&& [VTableAddr, VTableInfo] : VTableMap)
	{
		if (!VTableInfo.ClientClassFnAddr) continue;

		VMMDLL_Scatter_PrepareEx(vmsh, VTableInfo.ClientClassFnAddr + 3, sizeof(uint32_t), reinterpret_cast<BYTE*>(&VTableInfo.ClientClassOffset), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto&& [VTableAddr, VTableInfo] : VTableMap)
	{
		if (!VTableInfo.ClientClassFnAddr) continue;

		VTableInfo.ClientClassAddr = VTableInfo.ClientClassFnAddr + VTableInfo.ClientClassOffset + 7;

		VMMDLL_Scatter_PrepareEx(vmsh, VTableInfo.ClientClassAddr + offsetof(CClientClass, ClassID), sizeof(CClientClass::ClassID), reinterpret_cast<BYTE*>(&VTableInfo.ClassID), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_CTFPlayerAddresses.clear();
	m_SentryAddresses.clear();
	m_TeleportAddresses.clear();
	m_DispenserAddresses.clear();
	m_StickybombAddresses.clear();
	m_RocketAddresses.clear();
	m_AnimatingAddresses.clear();
	m_AmmoPackAddresses.clear();
	m_CurrencyPackAddresses.clear();

	for (auto&& [EntAddr, VTableAddr] : EntityVTableMap)
	{
		auto Info = VTableMap.at(VTableAddr.VTableAddress);

		if (std::find(m_AmmoPackModelAddresses.begin(), m_AmmoPackModelAddresses.end(), VTableAddr.ModelAddress) != m_AmmoPackModelAddresses.end())
		{
			m_AmmoPackAddresses.push_back(EntAddr);
			continue;
		}

		if (std::find(m_HealhPackModelAddresses.begin(), m_HealhPackModelAddresses.end(), VTableAddr.ModelAddress) != m_HealhPackModelAddresses.end())
		{
			m_HealthPackAddresses.push_back(EntAddr);
			continue;
		}

		switch (Info.ClassID)
		{
		case ClassIDs::CTFPlayer:
			m_CTFPlayerAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CObjectTeleporter:
			m_TeleportAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CObjectSentrygun:
			m_SentryAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CObjectDispenser:
			m_DispenserAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CTFGrenadePipebombProjectile:
			m_StickybombAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CTFProjectile_Rocket:
			m_RocketAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CBaseAnimating:
			m_AnimatingAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CTFAmmoPack:
			m_AmmoPackAddresses.push_back(EntAddr);
			continue;
		case ClassIDs::CCurrencyPack:
			m_CurrencyPackAddresses.push_back(EntAddr);
			continue;
		default:
			continue;
		}
	}

	return true;
}

bool IEntityList::UpdateAllCTFPlayerInfo(DMA_Connection* Conn)
{
	std::scoped_lock CTFPlayerLock(m_CTFPlayerInfoMutex);

	if (m_CTFPlayerAddresses.empty())
	{
		std::println("[MyEntityList] m_CTFPlayerAddresses is empty!");

		return false;
	}

	m_CTFPlayerInfoMap.clear();

	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto EntAddr : m_CTFPlayerAddresses)
	{
		PrepareCTFPlayerReads(EntAddr, vmsh, m_CTFPlayerInfoMap[EntAddr]);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto&& [EntAddr, EntInfo] : m_CTFPlayerInfoMap)
	{
		PrepareCTFPlayerBoneReads(EntAddr, vmsh, EntInfo);

		if (EntAddr == m_LocalPlayerAddr)
			EntInfo.m_bIsLocalPlayer = true;
	}
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateExistingCTFPlayerInfo(DMA_Connection* Conn)
{
	std::scoped_lock CTFPlayerLock(m_CTFPlayerInfoMutex);

	if (m_CTFPlayerInfoMap.empty())
	{
		std::println("[MyEntityList] m_CTFPlayerInfoMap is empty!");

		return false;
	}

	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);
	for (auto&& [EntAddr, EntInfo] : m_CTFPlayerInfoMap)
	{
		PrepareCTFPlayerReads(EntAddr, vmsh, EntInfo);
	}
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateBuildingInfo(DMA_Connection* Conn, bool bShouldClear)
{
	std::scoped_lock BuildingLock(m_SentryInfoMutex, m_DispenserInfoMutex, m_TeleportInfoMutex);

	if (bShouldClear)
	{
		m_SentryInfoMap.clear();
		m_TeleportInfoMap.clear();
		m_DispenserInfoMap.clear();
	}

	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto SentryAddr : m_SentryAddresses)
	{
		PrepareCBaseEntityReads(SentryAddr, vmsh, m_SentryInfoMap[SentryAddr]);
		PrepareCBuildingReads(SentryAddr, vmsh, m_SentryInfoMap[SentryAddr]);
		PrepareCSentryReads(SentryAddr, vmsh, m_SentryInfoMap[SentryAddr]);
	}

	for (auto TeleportAddr : m_TeleportAddresses)
	{
		PrepareCBaseEntityReads(TeleportAddr, vmsh, m_TeleportInfoMap[TeleportAddr]);
		PrepareCBuildingReads(TeleportAddr, vmsh, m_TeleportInfoMap[TeleportAddr]);
	}

	for (auto DispenserAddr : m_DispenserAddresses)
	{
		PrepareCBaseEntityReads(DispenserAddr, vmsh, m_DispenserInfoMap[DispenserAddr]);
		PrepareCBuildingReads(DispenserAddr, vmsh, m_DispenserInfoMap[DispenserAddr]);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateStickybombInfo(DMA_Connection* Conn)
{
	std::scoped_lock StickybombLock(m_StickybombInfoMutex);

	m_StickybombInfoMap.clear();

	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto BombAddr : m_StickybombAddresses)
		PrepareCBaseEntityReads(BombAddr, vmsh, m_StickybombInfoMap[BombAddr]);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateRocketInfo(DMA_Connection* Conn)
{
	std::scoped_lock RocketLock(m_RocketInfoMutex);

	m_RocketInfoMap.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto RocketAddr : m_RocketAddresses)
		PrepareCBaseEntityReads(RocketAddr, vmsh, m_RocketInfoMap[RocketAddr]);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateHealthPackInfo(DMA_Connection* Conn)
{
	std::scoped_lock HealthPackLock(m_HealthPackInfoMutex);

	m_HealthPackInfoMap.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto HealthPackAddr : m_HealthPackAddresses)
		PrepareCBaseEntityReads(HealthPackAddr, vmsh, m_HealthPackInfoMap[HealthPackAddr]);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateAmmoPackInfo(DMA_Connection* Conn)
{
	std::scoped_lock AmmoPackLock(m_AmmoPackInfoMutex);

	m_AmmoPackInfoMap.clear();

	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto AmmoPackAddr : m_AmmoPackAddresses)
		PrepareCBaseEntityReads(AmmoPackAddr, vmsh, m_AmmoPackInfoMap[AmmoPackAddr]);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

bool IEntityList::UpdateAnimatingInfo(DMA_Connection* Conn)
{
	std::scoped_lock AnimatingLock(m_AnimatingInfoMutex);

	m_AnimatingInfoMap.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto AnimatingAddr : m_AnimatingAddresses)
		PrepareCBaseEntityReads(AnimatingAddr, vmsh, m_AnimatingInfoMap[AnimatingAddr]);

	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	return true;
}

bool IEntityList::UpdateCurrencyPackInfo(DMA_Connection* Conn)
{
	std::scoped_lock CurrencyLock(m_CurrencyPackInfoMutex);

	m_CurrencyPackInfoMap.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto CurrencyPackAddr : m_CurrencyPackAddresses)
		PrepareCBaseEntityReads(CurrencyPackAddr, vmsh, m_CurrencyPackInfoMap[CurrencyPackAddr]);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	return true;
}

uintptr_t IEntityList::GetIClientEntityListAddr()
{
	return TF2::Proc().GetClientBase() + Offsets::EntityList;
}

bool IEntityList::PrepareCTFPlayerReads(uintptr_t PlayerAddr, VMMDLL_SCATTER_HANDLE vmsh, CTFPlayerInfo& pi)
{
	if (pi.m_BoneMatrixAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, pi.m_BoneMatrixAddr, sizeof(BoneArray), (BYTE*)&pi.m_BoneArray, nullptr);

	PrepareCBaseEntityReads(PlayerAddr, vmsh, pi);

	uintptr_t HealthAddress = PlayerAddr + Offsets::PlayerCurrentHealth;
	VMMDLL_Scatter_PrepareEx(vmsh, HealthAddress, sizeof(uint32_t), (BYTE*)&pi.m_CurrentHealth, nullptr);

	uintptr_t ClassAddress = PlayerAddr + Offsets::PlayerClassID;
	VMMDLL_Scatter_PrepareEx(vmsh, ClassAddress, sizeof(uint32_t), (BYTE*)&pi.m_ClassID, nullptr);

	uintptr_t DeadAddress = PlayerAddr + Offsets::PlayerDeadByte;
	VMMDLL_Scatter_PrepareEx(vmsh, DeadAddress, sizeof(uint8_t), (BYTE*)&pi.m_DeadByte, nullptr);

	uintptr_t PlayerIDAddress = PlayerAddr + Offsets::CBaseEntity::EntityID;
	VMMDLL_Scatter_PrepareEx(vmsh, PlayerIDAddress, sizeof(uint32_t), (BYTE*)&pi.EntityID, nullptr);

	uintptr_t BoneMatrixAddr = PlayerAddr + Offsets::PlayerBoneMatrix;
	VMMDLL_Scatter_PrepareEx(vmsh, BoneMatrixAddr, sizeof(uintptr_t), (BYTE*)&pi.m_BoneMatrixAddr, nullptr);

	uintptr_t ConditionAddr = PlayerAddr + Offsets::PlayerConditionBits;
	VMMDLL_Scatter_PrepareEx(vmsh, ConditionAddr, sizeof(ConditionBits), (BYTE*)&pi.m_ConditionBits, nullptr);

	if (PlayerAddr == m_LocalPlayerAddr)
	{
		uintptr_t RotationAddr = PlayerAddr + Offsets::PlayerRotation;
		VMMDLL_Scatter_PrepareEx(vmsh, RotationAddr, sizeof(Vector3), (BYTE*)&pi.m_Rotation, nullptr);
	}

	return true;
}

bool IEntityList::PrepareCTFPlayerBoneReads(uintptr_t PlayerAddr, VMMDLL_SCATTER_HANDLE vmsh, CTFPlayerInfo& pi)
{
	if (pi.m_BoneMatrixAddr)
	{
		VMMDLL_Scatter_PrepareEx(vmsh, pi.m_BoneMatrixAddr, sizeof(BoneArray), (BYTE*)&pi.m_BoneArray, nullptr);
	}

	return true;
}

bool IEntityList::PrepareCBaseEntityReads(uintptr_t EntAddr, VMMDLL_SCATTER_HANDLE vmsh, CBaseEntityInfo& ei)
{
	uintptr_t PositionAddress = EntAddr + Offsets::CBaseEntity::Origin;
	VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), (BYTE*)&ei.m_Origin, nullptr);

	uintptr_t TeamAddress = EntAddr + Offsets::CBaseEntity::Team;
	VMMDLL_Scatter_PrepareEx(vmsh, TeamAddress, sizeof(uint32_t), (BYTE*)&ei.m_TeamID, nullptr);

	uintptr_t DormantAddr = EntAddr + Offsets::CBaseEntity::PlayerDormant;
	VMMDLL_Scatter_PrepareEx(vmsh, DormantAddr, sizeof(uint8_t), (BYTE*)&ei.m_DormantByte, nullptr);

	return true;
}

bool IEntityList::PrepareCBuildingReads(uintptr_t BuildingAddr, VMMDLL_SCATTER_HANDLE vmsh, CBuilding& bi)
{
	uintptr_t BuildingLevelAddr = BuildingAddr + Offsets::Buildings::CBuilding::CurrentBuildingLevel;
	VMMDLL_Scatter_PrepareEx(vmsh, BuildingLevelAddr, sizeof(bi.m_CurrentBuildingLevel), (BYTE*)&bi.m_CurrentBuildingLevel, nullptr);

	uintptr_t UpgradeProgressAddr = BuildingAddr + Offsets::Buildings::CBuilding::UpgradeProgress;
	VMMDLL_Scatter_PrepareEx(vmsh, UpgradeProgressAddr, sizeof(bi.m_UpgradeProgress), (BYTE*)&bi.m_UpgradeProgress, nullptr);

	/* Read current and max health at once. They are continguous. */
	uintptr_t CurrentHealthAddr = BuildingAddr + Offsets::Buildings::CBuilding::CurrentHealth;
	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddr, sizeof(bi.m_CurrentHealth) * 2, (BYTE*)&bi.m_CurrentHealth, nullptr);

	return true;
}

bool IEntityList::PrepareCSentryReads(uintptr_t BuildingAddr, VMMDLL_SCATTER_HANDLE vmsh, CSentryGun& si)
{
	uintptr_t NumBulletsAddr = BuildingAddr + Offsets::Buildings::Sentry::NumBullets;
	VMMDLL_Scatter_PrepareEx(vmsh, NumBulletsAddr, sizeof(si.NumBullets), (BYTE*)&si.NumBullets, nullptr);

	uintptr_t NumRocketsAddr = BuildingAddr + Offsets::Buildings::Sentry::NumRockets;
	VMMDLL_Scatter_PrepareEx(vmsh, NumRocketsAddr, sizeof(si.NumRockets), (BYTE*)&si.NumRockets, nullptr);

	return true;
}

std::vector<uintptr_t> ModelAddresses;
std::vector<uintptr_t> ModelNameAddresses;
std::vector<CModelName> ModelNames;

std::unordered_map<std::string, uintptr_t> ModelNameMap;

const std::vector<std::string> m_AmmoPackModelNames = {
	"models/items/ammopack_large.mdl",
	"models/items/ammopack_medium.mdl",
	"models/items/ammopack_small.mdl"
};
const std::vector<std::string> m_HealthPackModelNames = {
	"models/items/medkit_large.mdl",
	"models/items/medkit_medium.mdl",
	"models/items/medkit_small.mdl"
};
bool IEntityList::PopulateModelAddresses(DMA_Connection* Conn)
{
	if (m_AllEntityAddresses.empty())
	{
		std::println("[MyEntityList] No entity addresses found, cannot populate consumable model addresses.");
		return false;
	}

	ModelAddresses.clear();
	ModelAddresses.resize(m_AllEntityAddresses.size());

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_AllEntityAddresses.size(); i++)
	{
		auto Addr = m_AllEntityAddresses[i];
		uintptr_t pModelAddr = Addr + Offsets::CBaseEntity::pModel;
		VMMDLL_Scatter_PrepareEx(vmsh, pModelAddr, sizeof(uintptr_t), (BYTE*)&ModelAddresses[i], nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	auto TotalSize = ModelAddresses.size();

	std::sort(ModelAddresses.begin(), ModelAddresses.end());
	auto NewEnd = std::unique(ModelAddresses.begin(), ModelAddresses.end());
	ModelAddresses.erase(NewEnd, ModelAddresses.end());

	auto UniqueSize = ModelAddresses.size();

	//std::println("[MyEntityList] Found {0:d} unique model addresses out of {1:d} total.", UniqueSize, TotalSize);

	ModelNameAddresses.clear();
	ModelNameAddresses.resize(ModelAddresses.size());

	for (int i = 0; i < ModelAddresses.size(); i++)
	{
		uintptr_t pModelName = ModelAddresses[i] + Offsets::CModelInfo::pModelName;
		VMMDLL_Scatter_PrepareEx(vmsh, pModelName, sizeof(uintptr_t), (BYTE*)&ModelNameAddresses[i], nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, TF2::Proc().GetPID(), VMMDLL_FLAG_NOCACHE);

	ModelNames.clear();
	ModelNames.resize(ModelAddresses.size());

	for (int i = 0; i < ModelNameAddresses.size(); i++)
	{
		if (ModelNameAddresses[i] == 0x0) continue;

		VMMDLL_Scatter_PrepareEx(vmsh, ModelNameAddresses[i], sizeof(CModelName), (BYTE*)&ModelNames[i], nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	ModelNameMap.clear();

	for (int i = 0; i < ModelAddresses.size(); i++)
	{
		auto Addr = ModelAddresses[i];
		auto Name = std::string(ModelNames[i].Name);

		if (Name.size() < 5) continue;

		ModelNameMap[Name] = Addr;
	}

	m_AmmoPackModelAddresses.clear();

	for (auto&& ModelName : m_AmmoPackModelNames)
	{
		auto ModelAddr = ModelNameMap.find(ModelName);

		if (ModelAddr != ModelNameMap.end())
		{
			//std::println("[MyEntityList] Found Ammo Pack Model: {0:s} Address: 0x{1:X}", ModelName, ModelAddr->second);
			m_AmmoPackModelAddresses.push_back(ModelAddr->second);
		}
	}

	m_HealthPackAddresses.clear();

	for (auto&& ModelName : m_HealthPackModelNames)
	{
		auto ModelAddr = ModelNameMap.find(ModelName);

		if (ModelAddr != ModelNameMap.end())
		{
			//std::println("[MyEntityList] Found Health Pack Model: {0:s} Address: 0x{1:X}", ModelName, ModelAddr->second);
			m_HealhPackModelAddresses.push_back(ModelAddr->second);
		}
	}

	return true;
}

void IEntityList::UpdateLocalPlayerAddress(DMA_Connection* Conn)
{
	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	uintptr_t LocalPlayerPtr = Proc.GetClientBase() + Offsets::LocalPlayerPtr;
	uintptr_t LocalPlayerAddr = 0x0;
	DWORD LocalPlayersBytes = 0x0;

	VMMDLL_Scatter_PrepareEx(vmsh, LocalPlayerPtr, sizeof(uintptr_t), (BYTE*)&LocalPlayerAddr, &LocalPlayersBytes);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	if (LocalPlayersBytes != sizeof(uintptr_t))
	{
		std::println("[MyEntityList] Incomplete read of LocalPlayerPtr: {}/{}", LocalPlayersBytes, sizeof(uintptr_t));
		return;
	}

	if (LocalPlayerAddr == 0x0)
	{
		std::println("[MyEntityList] LocalPlayerAddr is null.");
		return;
	}

	LocalPlayerAddr += 0x10;
	static uintptr_t PreviousLocalPlayerAddr = 0x0;

	if (LocalPlayerAddr == PreviousLocalPlayerAddr)
		return;

	PreviousLocalPlayerAddr = LocalPlayerAddr;
	m_LocalPlayerAddr = LocalPlayerAddr;
	std::println("[MyEntityList] LocalPlayerAddr updated: 0x{:X}", m_LocalPlayerAddr);
}
