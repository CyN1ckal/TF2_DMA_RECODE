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
		bool FindBaseAddresses(DMA_Connection* Conn);
		bool FindGGameAddr(DMA_Connection* Conn);
	}

	namespace CBaseEntity
	{
		inline uint32_t Origin = 0x448;
		inline uint32_t Team = 0xDC;
		inline uint32_t PlayerDormant = 0x21A;
		inline uint32_t pModel = 0x88;
		inline uint32_t EntityID = 0x78;
	}

	namespace CModelInfo
	{
		inline uint32_t pModelName = 0x8;
	}

	namespace Buildings
	{
		namespace CBuilding
		{
			inline uint32_t CurrentBuildingLevel = 0x1330;
			inline uint32_t UpgradeProgress = 0x1334;
			inline uint32_t DesiredBuildingLevel = 0x1338;
			inline uint32_t CurrentHealth = 0x1380;
			inline uint32_t MaxHealth = 0x1384;
		}

		namespace Sentry
		{
			inline uint32_t NumBullets = 0x13F4;
			inline uint32_t NumRockets = 0x13FC;
		}
	}

	/* Manual */
	inline uint32_t PlayerArray = 0x48;
	inline uint32_t PlayerCurrentHealth = 0xD4;
	inline uint32_t PlayerClassID = 0x1BA0;
	inline uint32_t PlayerDeadByte = 0x7C;
	inline uint32_t PlayerRotation = 0x3B8;
	inline uint32_t PlayerConditionBits = 0x1F64;

	/* Bone stuff */
	inline uint32_t PlayerBoneMatrix = 0x838;

	/* Dynamically Resolved */
	inline uint32_t EntityList = 0x0;
	inline uint32_t MaxPlayers = 0x0;
	inline uint32_t GGame = 0x0;
	inline uint32_t LocalPlayerPtr = 0x0;
	inline uint32_t ViewProjectionMatrix = 0x0;
}