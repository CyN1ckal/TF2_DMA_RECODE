#pragma once

#include "pch.h"

#include <bitset>

struct ConditionBits
{
	uint8_t Data[20];

	std::bitset<128> GetBits() const
	{
		std::bitset<128> bits;
		for (int i = 0; i < 20; ++i)
		{
			bits |= std::bitset<128>(Data[i] << (i * 8));
		}
		return bits;
	}
};