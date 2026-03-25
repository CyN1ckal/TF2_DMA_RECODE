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

void DMAThread(DMA_Connection* Conn, Process* TF2)
{
#ifdef TRACY_ENABLE
	tracy::SetThreadName("DMA Thread");
#endif
	IEntityList::Initialize(Conn);

	CTimer LocalPlayerAddressTimer(std::chrono::seconds(1), [&Conn] { IEntityList::UpdateLocalPlayerAddress(Conn); });
	CTimer UpdateEntityAddressesTimer(std::chrono::milliseconds(500), [&Conn] { IEntityList::UpdateEntityAddresses(Conn); });
	CTimer UpdatePlayers(std::chrono::milliseconds(2), [&Conn] { IEntityList::UpdateExistingCTFPlayerInfo(Conn); });
	CTimer UpdateBuildings(std::chrono::milliseconds(250), [&Conn] {IEntityList::UpdateExistingBuildings(Conn);  });
	CTimer UpdateExplosives(std::chrono::milliseconds(25), [&Conn] {IEntityList::UpdateExistingExplosives(Conn);  });
	CTimer UpdateConsumables(std::chrono::milliseconds(50), [&Conn] {IEntityList::UpdateExistingConsumables(Conn);  });
	CTimer ViewmatrixTimer(std::chrono::milliseconds(1), [&Conn] { Camera::UpdateViewProjectionMatrix(Conn); });
	CTimer HotkeyTimer(std::chrono::milliseconds(10), [&Conn] { Keybinds::OnFrame(Conn); });
	CTimer ModelRefresh(std::chrono::seconds(10), [&Conn] { IEntityList::PopulateModelAddresses(Conn); });

	while (bRunning)
	{
		auto CurrentTime = std::chrono::steady_clock::now();
		LocalPlayerAddressTimer.Tick(CurrentTime);
		UpdateEntityAddressesTimer.Tick(CurrentTime);
		UpdatePlayers.Tick(CurrentTime);
		UpdateExplosives.Tick(CurrentTime);
		UpdateConsumables.Tick(CurrentTime);
		UpdateBuildings.Tick(CurrentTime);
		ViewmatrixTimer.Tick(CurrentTime);
		HotkeyTimer.Tick(CurrentTime);
		ModelRefresh.Tick(CurrentTime);
	}
}