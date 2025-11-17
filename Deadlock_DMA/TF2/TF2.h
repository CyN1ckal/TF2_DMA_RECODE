#pragma once

constexpr float HammerUnitPerMeter = 52.0f;

struct GameSize
{
	uint32_t GameWidth{ 0 };
	uint32_t GameHeight{ 0 };
};

class TF2
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static Process& Proc() { return TF2_Proc; }
	static const GameSize& GetGameSize() { return m_Size; }
private:
	static void UpdateGameWindowSize(DMA_Connection* Conn);

private:
	static inline Process TF2_Proc{};
	static inline GameSize m_Size{};
};