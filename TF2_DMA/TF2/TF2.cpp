#include "pch.h"

#include "TF2.h"

#include "Offsets/Offsets.h"

#include "DMA/Input Manager.h"

bool TF2::Initialize(DMA_Connection* Conn)
{
	if (c_keys::InitKeyboard(Conn))
		std::println("[Input Manager] Initialized successfully.");

	TF2_Proc.GetProcessInfo("tf_win64.exe", Conn);

	Offsets::ResolveDynamicOffsets(Conn);

	UpdateGameWindowSize(Conn);

	return true;
}

#include "Classes/Misc.h"
void TF2::UpdateGameWindowSize(DMA_Connection* Conn)
{
	auto& Proc = TF2::Proc();
	GameSize Size = Proc.ReadMem<GameSize>(Conn, Proc.GetEngineBase() + Offsets::GGame + offsetof(GGame, GameSizeX));

	if (Size.GameWidth == 0 || Size.GameHeight == 0)
	{
		std::println("[!] Game Size is invalid! X: {0:d}, Y: {1:d}", Size.GameWidth, Size.GameHeight);
		return;
	}

	m_Size = Size;

	std::println("[+] Game Size detected as {0:d}x{1:d}", m_Size.GameWidth, m_Size.GameHeight);

	return;
}