#include "pch.h"

#include "Aimbot.h"

#include "DMA/Input Manager.h"

#include "GUI/Keybinds/Keybinds.h"

#include "TF2/IEntityList/IEntityList.h"
#include "TF2/Camera/Camera.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Color Picker/Color Picker.h"

void Aimbot::RenderSettings()
{
	ImGui::Begin("Aimbot Settings");

	if (m_Device.isConnected())
		ImGui::Text("Makcu Connected");
	else
	{
		ImGui::Text("Makcu Not Connected");

		if (ImGui::Button("Connect Makcu"))
		{
			auto Devices = makcu::Device::findDevices();
			for (auto& Device : Devices)
				if (m_Device.connect(Device.port)) break;
		}
	}

	ImGui::Checkbox("Enable Aimbot", &bMasterToggle);
	ImGui::SameLine();
	ImGui::Checkbox("Aim for Head", &bAimHead);
	ImGui::SliderFloat("Max FOV", &fMaxFOV, 1.0f, 500.0f);
	ImGui::SliderFloat("Deadzone FOV", &fDeadzoneFOV, 1.0f, 20.0f);
	ImGui::SliderFloat("Dampen", &fDampen, 0.01, 1.0f);

	ImGui::End();
}

void Aimbot::RenderFOVCircles()
{
	if (bMasterToggle == false) return;

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();

	ImVec2 CenterScreen = { WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f) };
	DrawList->AddCircle(CenterScreen, fDeadzoneFOV, ColorPicker::DeadzoneFOV, 0, 2.0f);
	DrawList->AddCircle(CenterScreen, fMaxFOV, ColorPicker::MaxFOV, 0, 2.0f);
}

void Aimbot::OnDMAFrame(DMA_Connection* Conn)
{
	if (bMasterToggle == false || m_Device.isConnected() == false)
		return;

	std::println("[Aimbot] Aimbot Activated");

	Vector2 PreviousDelta{};

	auto LastExecutionTime = std::chrono::high_resolution_clock::now();

	do
	{
		auto CurrentTime = std::chrono::high_resolution_clock::now();
		auto TimeDelta = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastExecutionTime).count();
		if (TimeDelta < 3) continue;
		LastExecutionTime = CurrentTime;

		Camera::UpdateViewProjectionMatrix(Conn);
		IEntityList::UpdateExistingCTFPlayerInfo(Conn);

		Vector2 MouseDelta{};
		if (PreviousTargetEntityAddress == 0x0)
			MouseDelta = GetBestMouseDelta();
		else
			MouseDelta = GetMouseDeltaToTarget(PreviousTargetEntityAddress);

		if (MouseDelta == PreviousDelta) continue;
		PreviousDelta = MouseDelta;

		MouseDelta *= fDampen;

		m_Device.mouseMove(MouseDelta.x, MouseDelta.y);
	} while (c_keys::IsKeyDown(Conn, Keybinds::m_AimbotHotkey));

	PreviousTargetEntityAddress = 0x0;

	std::println("[Aimbot] Aimbot Ended");
}

Vector2 Aimbot::GetBestMouseDelta()
{
	std::scoped_lock lk(IEntityList::m_PlayersMutex);

	Vector2 CenterScreen = { Fuser::ScreenSize.x * 0.5f, Fuser::ScreenSize.y * 0.5f };

	float ClosestDistance = FLT_MAX;
	Vector2 BestDelta = { 0.0f, 0.0f };

	for (auto& Player : IEntityList::m_Players)
	{
		if (Player.IsInvalid()) continue;
		if (Player.IsLocalPlayer()) continue;
		if (Player.IsFriendly()) continue;
		if (Player.IsDormant()) continue;
		if (Player.IsDead()) continue;

		Vector2 ScreenPos{};
		if (!WorldToScreen((bAimHead) ? Player.GetHeadPosition() : Player.GetChestPosition(), ScreenPos))
			continue;

		Vector2 Delta = ScreenPos - CenterScreen;
		float Distance = sqrtf((Delta.x * Delta.x) + (Delta.y * Delta.y));

		if (Distance > fMaxFOV) continue;
		if (Distance < fDeadzoneFOV) continue;

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			BestDelta = Delta;
			PreviousTargetEntityAddress = Player.m_EntityAddress;
		}
	}

	return BestDelta;
}

Vector2 Aimbot::GetMouseDeltaToTarget(uintptr_t TargetEntityAddress)
{
	std::scoped_lock lk(IEntityList::m_PlayersMutex);

	Vector2 CenterScreen = { Fuser::ScreenSize.x * 0.5f, Fuser::ScreenSize.y * 0.5f };

	float ClosestDistance = FLT_MAX;

	for (auto& Player : IEntityList::m_Players)
	{
		if (Player.m_EntityAddress != TargetEntityAddress) continue;
		if (Player.IsInvalid()) continue;
		if (Player.IsLocalPlayer()) continue;
		if (Player.IsFriendly()) continue;
		if (Player.IsDormant()) continue;
		if (Player.IsDead()) continue;

		Vector2 ScreenPos{};
		if (!WorldToScreen((bAimHead) ? Player.GetHeadPosition() : Player.GetChestPosition(), ScreenPos))
			continue;

		Vector2 Delta = ScreenPos - CenterScreen;
		return Delta;
	}

	return Vector2();
}
