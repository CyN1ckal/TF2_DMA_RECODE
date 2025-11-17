#include "pch.h"

#include "DMA Thread.h"

#include "GUI/Keybinds/Keybinds.h"

#include "TF2/Camera/Camera.h"

#include "TF2/IEntityList/IEntityList.h"

template <typename T, typename F>
class CTimer
{
public:
	CTimer(T _Interval, F _Function) : m_Interval(_Interval), m_Function(_Function) {}

	void Tick(std::chrono::steady_clock::time_point CurrentTime)
	{
		if (std::chrono::duration_cast<T>(CurrentTime - m_LastExecutionTime) >= m_Interval)
		{
			m_Function();
			m_LastExecutionTime = CurrentTime;
		}
	}
private:
	T m_Interval{};
	F m_Function{};
	std::chrono::steady_clock::time_point m_LastExecutionTime{};
};

extern std::atomic<bool> bRunning;

void DMAThread(DMA_Connection* Conn, Process* Deadlock)
{
#ifdef TRACY_ENABLE
	tracy::SetThreadName("DMA Thread");
#endif
	IEntityList::Initialize(Conn);

	CTimer LocalPlayerAddressTimer(std::chrono::seconds(1), [&Conn] { IEntityList::UpdateLocalPlayerAddress(Conn); });
	CTimer UpdateEntityAddressesTimer(std::chrono::milliseconds(500), [&Conn] { IEntityList::UpdateEntityAddresses(Conn); });
	CTimer UpdatePlayers(std::chrono::milliseconds(4), [&Conn] { IEntityList::UpdateExistingCTFPlayerInfo(Conn); });
	CTimer ViewmatrixTimer(std::chrono::milliseconds(2), [&Conn] { Camera::UpdateViewProjectionMatrix(Conn); });
	CTimer HotkeyTimer(std::chrono::milliseconds(10), [&Conn] { Keybinds::OnFrame(Conn); });

	while (bRunning)
	{
		auto CurrentTime = std::chrono::steady_clock::now();
		LocalPlayerAddressTimer.Tick(CurrentTime);
		UpdateEntityAddressesTimer.Tick(CurrentTime);
		UpdatePlayers.Tick(CurrentTime);
		ViewmatrixTimer.Tick(CurrentTime);
		HotkeyTimer.Tick(CurrentTime);
	}
}