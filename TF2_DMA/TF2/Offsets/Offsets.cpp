#include "pch.h"

#include "Offsets.h"

#include "TF2/Classes/Misc.h"

#include "Zydis/Zydis.h"

bool Offsets::ResolveDynamicOffsets(DMA_Connection* Conn)
{
	using namespace Internal;

	m_Scan.Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), "engine.dll");

	FindMaxPlayersOffset(Conn);
	FindViewMatrixOffset(Conn);
	FindGGameAddr(Conn);

	m_Scan.Close();

	m_Scan.Initialize(Conn->GetHandle(), TF2::Proc().GetPID(), "client.dll");

	FindEntityListOffset(Conn);
	FindLocalPlayerPtrOffset(Conn);

	m_Scan.Close();

	return false;
}

bool Offsets::Internal::FindEntityListOffset(DMA_Connection* Conn)
{
	const std::string Pattern = "\x48\x89\x05????\x0F\x57\xC0\x48\x8D\x05????\x44\x89\x05";
	const std::string Mask = "xxx????xxxxxx????xxx";

	PatternInfo pi;
	pi.Pattern = Pattern;
	pi.Mask = Mask;
	pi.ModuleName = "client.dll";

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);

	uintptr_t RuntimeAddress = SectionOffset + m_Scan.GetModuleBase() + m_Scan.GetSectionBase();

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("FindEntityListOffset Disassemble");

	uintptr_t Displacement = Instruction.operands[0].mem.disp.value;

	Offsets::EntityList = Displacement + Instruction.info.length + RuntimeAddress - m_Scan.GetModuleBase();

	std::println("[+] Offsets::EntityList {0:X}", Offsets::EntityList);

	return 1;
}

bool Offsets::Internal::FindMaxPlayersOffset(DMA_Connection* Conn)
{
	const std::string Pattern = "\x0F\x4C\x3D";
	const std::string Mask = "xxx";

	PatternInfo pi;
	pi.Pattern = Pattern;
	pi.Mask = Mask;
	pi.ModuleName = "engine.dll";

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);

	uintptr_t RuntimeAddress = SectionOffset + m_Scan.GetModuleBase() + m_Scan.GetSectionBase();

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("FindMaxPlayersOffset Disassemble");

	uintptr_t Displacement = Instruction.operands[1].mem.disp.value;

	Offsets::MaxPlayers = Displacement + Instruction.info.length + RuntimeAddress - m_Scan.GetModuleBase();

	std::println("[+] Offsets::MaxPlayers {0:X}", Offsets::MaxPlayers);

	return 1;
}

bool Offsets::Internal::FindGGameAddr(DMA_Connection* Conn)
{
	const std::string Pattern = "\x48\x8B\x0D????\x83\xE7";
	const std::string Mask = "xxx????xx";

	PatternInfo pi;
	pi.Pattern = Pattern;
	pi.Mask = Mask;
	pi.ModuleName = "engine.dll";

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);

	uintptr_t RuntimeAddress = SectionOffset + m_Scan.GetModuleBase() + m_Scan.GetSectionBase();

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("FindMaxPlayersOffset Disassemble");

	uintptr_t Displacement = Instruction.operands[1].mem.disp.value;

	Offsets::GGame = Displacement + Instruction.info.length + RuntimeAddress - m_Scan.GetModuleBase();

	std::println("[+] Offsets::GGame {0:X}", Offsets::GGame);

	return 1;
}

bool Offsets::Internal::FindLocalPlayerPtrOffset(DMA_Connection* Conn)
{
	const std::string Pattern = "\x48\x3B\x1D????\x75";
	const std::string Mask = "xxx????x";

	PatternInfo pi;
	pi.Pattern = Pattern;
	pi.Mask = Mask;
	pi.ModuleName = "client.dll";

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);

	uintptr_t RuntimeAddress = SectionOffset + m_Scan.GetModuleBase() + m_Scan.GetSectionBase();

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("FindLocalPlayerPtrOffset Disassemble");

	uintptr_t Displacement = Instruction.operands[1].mem.disp.value;

	Offsets::LocalPlayerPtr = Displacement + Instruction.info.length + RuntimeAddress - m_Scan.GetModuleBase();

	std::println("[+] Offsets::LocalPlayerPtr {0:X}", Offsets::LocalPlayerPtr);

	return 1;
}

bool Offsets::Internal::FindViewMatrixOffset(DMA_Connection* Conn)
{
	const std::string Pattern = "\x48\x8D\x0D????\xE8????\x44\x0F\x28\x84\x24";
	const std::string Mask = "xxx????x????xxxxx";

	PatternInfo pi;
	pi.Pattern = Pattern;
	pi.Mask = Mask;
	pi.ModuleName = "engine.dll";

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);

	uintptr_t RuntimeAddress = SectionOffset + m_Scan.GetModuleBase() + m_Scan.GetSectionBase();

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("FindViewMatrixOffset Disassemble");

	uintptr_t Displacement = Instruction.operands[1].mem.disp.value;

	Offsets::ViewProjectionMatrix = Displacement + Instruction.info.length + RuntimeAddress - m_Scan.GetModuleBase();

	std::println("[+] Offsets::ViewProjectionMatrix {0:X}", Offsets::ViewProjectionMatrix);

	return 1;
}