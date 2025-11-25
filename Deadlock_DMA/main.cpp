#include "pch.h"

#include "TF2/TF2.h"
#include "GUI/Main Window/Main Window.h"

#include "DMA/DMA Thread.h"

#ifdef CATCH_ENABLE
#include "Tests/All Tests.h"
#endif

std::atomic<bool> bRunning{ true };
#ifndef CATCH_ENABLE
int main()
{
	DMA_Connection* Conn = nullptr;

	try
	{
		Conn = DMA_Connection::GetInstance();
	}
	catch (const std::exception& e)
	{
		std::println("[FATAL] Exception: {}", e.what());
		system("pause");
		return -1;
	}

	TF2::Initialize(Conn);

	std::println("Press END to exit...");

#ifndef DLL_BUILD
	MainWindow::Initialize();
#endif

	std::thread DMAThreadHandle(DMAThread, Conn, &TF2::Proc());

	while (bRunning)
	{
		if (GetAsyncKeyState(VK_END) & 0x1) bRunning = false;
#ifndef DLL_BUILD
		MainWindow::OnFrame();
		FrameMark;
#endif
#ifdef DLL_BUILD
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
	}

	DMAThreadHandle.join();

	Conn->EndConnection();

	system("pause");

	return 0;
}
#endif

#ifdef DLL_BUILD
DWORD WINAPI StartingThread(HMODULE hMod)
{
	return main();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::println("[DLL] TF2 DMA Injected");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartingThread, hModule, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
#endif