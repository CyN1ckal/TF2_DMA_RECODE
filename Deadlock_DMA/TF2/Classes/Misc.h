#pragma once
#include "pch.h"

class CModelName
{
public:
	char Name[96];
};

class GGame
{
public:
	char pad_0000[64]; //0x0000
	uint32_t GameSizeX; //0x0040
	uint32_t GameSizeY; //0x0044
};

class CClientEntityListHeader
{
public:
	char pad_0000[40]; //0x0000
	uint32_t NumServerEnts; //0x0028
	uint32_t MaxServerEnts; //0x002C
	uint32_t NumNonNetworkable; //0x0030
	uint32_t MaxUsedServerIndex; //0x0034
};

class CEntityEntry
{
public:
	uintptr_t pEntity; //0x0000
	uint32_t BaseEntitiesIndex; //0x0008
	uint32_t Pad; //0x000C
};

class CClientClass
{
public:
	char pad_0000[16]; //0x0000
	char* ClassName; //0x0010
	char pad_0018[16]; //0x0018
	int32_t ClassID; //0x0028
};

enum class ClassIDs : int
{
	CBaseAnimating = 1,
	CBaseDoor = 6,
	CBaseEntity = 7,
	CCurrencyPack = 31,
	CLightGlow = 79,
	CObjectCartDispenser = 85,
	CObjectDispenser = 86,
	CObjectSentrygun = 88,
	CObjectTeleporter = 89,
	CTFAmmoPack = 181,
	CTFGrenadePipebombProjectile = 217,
	CTFPlayer = 247,
	CTFProjectile_Rocket = 264,
	CTFTeam = 318,
	CTFViewModel = 320,
	CWorld = 353,
};