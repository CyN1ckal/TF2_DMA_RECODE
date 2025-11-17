#pragma once
#include <vector>
#include "TF2/Constants/eTFClass.h"

using BonePair = std::pair<uint16_t, uint16_t>;

std::vector<BonePair> MedicBonePairs = {
	{6,5},
	{5,0},
	{0,45},
	{0,46},
	{45,15},
	{46,16},
	{15,34},
	{16,35},
	{5,9},
	{5,10},
	{10,12},
	{9,11},
	{11,31},
	{12,32}
};

std::vector <BonePair> EngineerBonePairs = {
	{8,7},
	{7,12},
	{7,15},
	{12,13},
	{15,16},
	{7,0},
	{0,10},
	{0,2},
	{10,17},
	{2,18},
	{13,20},
	{16,19}
};

std::vector <BonePair> SoldierBonePairs = {
	{6,5},
	{5,0},
	{0,13},
	{0,14},
	{13,15},
	{14,16},
	{5,9},
	{5,10},
	{9,11},
	{11,19},
	{10,12},
	{13,15},
	{12,20},
	{16,18},
	{15,17}
};

std::vector <BonePair> HeavyBonePairs = {
	{6,5},
	{5,0},
	{0,13},
	{13,29},
	{0,14},
	{14,30},
	{5,9},
	{5,10},
	{9,11},
	{10,12},
	{11,27},
	{12,28}
};

std::vector <BonePair> DemoBonePair = {
	{16,15},
	{15,0},
	{0,9},
	{0,11},
	{11,12},
	{12,26},
	{9,10},
	{10,25},
	{5,6},
	{5,8},
	{6,19},
	{19,17},
	{8,14},
	{14,20}
};

std::vector <BonePair> ScoutBonePair = {
	{6,5},
	{5,0},
	{0,14},
	{14,16},
	{16,18},
	{0,13},
	{13,15},
	{15,17},
	{5,10},
	{10,12},
	{12,20},
	{5,9},
	{9,11},
	{11,19}
};

std::vector <BonePair> PyroBonePair = {
	{6,4},
	{4,0},
	{0,16},
	{16,17},
	{0,20},
	{20,21},
	{4,8},
	{8,9},
	{9,10},
	{4,12},
	{12,13},
	{13,14}
};

std::vector <BonePair> SniperBonePair = {
	{6,4},
	{4,0},
	{0,13},
	{13,15},
	{15,17},
	{0,14},
	{14,16},
	{16,18},
	{4,9},
	{9,11},
	{11,19},
	{4,10},
	{10,12},
	{12,20}
};

std::vector <BonePair> SpyBonePair = {
	{6,4},
	{4,0},
	{0,18},
	{0,19},
	{18,20},
	{19,21},
	{4,10},
	{4,11},
	{11,13},
	{10,12},
	{12,14},
	{13,15}
};

std::vector<BonePair>* GetBonePairsForClass(eTFClass Class)
{
	switch (Class)
	{
	case eTFClass::Scout:
		return &ScoutBonePair;
	case eTFClass::Soldier:
		return &SoldierBonePairs;
	case eTFClass::Pyro:
		return &PyroBonePair;
	case eTFClass::Demo:
		return &DemoBonePair;
	case eTFClass::Heavy:
		return &HeavyBonePairs;
	case eTFClass::Engineer:
		return &EngineerBonePairs;
	case eTFClass::Medic:
		return &MedicBonePairs;
	case eTFClass::Sniper:
		return &SniperBonePair;
	case eTFClass::Spy:
		return &SpyBonePair;
	default:
		return nullptr;
	}
}