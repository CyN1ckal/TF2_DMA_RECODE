#pragma once
#include "DMA/MultiScan.h"

#pragma once
#include <cstdint>

namespace Offsets
{
	bool ResolveDynamicOffsets(DMA_Connection* Conn);

	namespace Internal
	{
		inline MultiScan m_Scan;
		bool FindLocalPlayerPtrOffset(DMA_Connection* Conn);
		bool FindEntityListOffset(DMA_Connection* Conn);
		bool FindMaxPlayersOffset(DMA_Connection* Conn);
		bool FindViewMatrixOffset(DMA_Connection* Conn);
		bool FindGGameAddr(DMA_Connection* Conn);
	}

	namespace CBaseEntity
	{
		inline std::ptrdiff_t PlayerID = 0x78;
		inline std::ptrdiff_t DeadByte = 0x7C;
		inline std::ptrdiff_t pModel = 0x88;
		inline std::ptrdiff_t CurrentHealth = 0xD4;
		inline std::ptrdiff_t TeamID = 0xDC;
		inline std::ptrdiff_t DormantByte = 0x21A;
		inline std::ptrdiff_t Origin = 0x448;
	}

	namespace CTFPlayer
	{
		inline std::ptrdiff_t BoneArray = 0x838;
		inline std::ptrdiff_t ConditionBits = 0x1F64;
		inline std::ptrdiff_t ClassID = 0x1BA0;
	}

	namespace CGenericBuilding
	{
		inline std::ptrdiff_t CurrentBuildingLevel{ 0x1330 };
		inline std::ptrdiff_t UpgradeProgress{ 0x1334 };
		inline std::ptrdiff_t DesiredBuildingLevel{ 0x1338 };
		inline std::ptrdiff_t CurrentHealth{ 0x1380 };
		inline std::ptrdiff_t MaxHealth{ 0x1384 };
	}

	namespace CSentryGun
	{
		inline std::ptrdiff_t NumBullets = 0x13F4;
		inline std::ptrdiff_t NumRockets = 0x13FC;
	}

	namespace CModelInfo
	{
		inline uint32_t pModelName = 0x8;
	}

	/* Manual */
	inline uint32_t PlayerArray = 0x48;
	inline uint32_t PlayerRotation = 0x3B8;

	/* Bone stuff */
	inline uint32_t PlayerBoneMatrix = 0x838;

	/* Dynamically Resolved */
	inline uint32_t EntityList = 0x0;
	inline uint32_t MaxPlayers = 0x0;
	inline uint32_t GGame = 0x0;
	inline uint32_t LocalPlayerPtr = 0x0;
	inline uint32_t ViewProjectionMatrix = 0x0;
}