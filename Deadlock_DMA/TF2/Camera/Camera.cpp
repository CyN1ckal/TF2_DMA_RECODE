#include "pch.h"

#include "Camera.h"

#include "TF2/Offsets/Offsets.h"

void Camera::UpdateViewProjectionMatrix(DMA_Connection* Conn)
{
	auto& Proc = TF2::Proc();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	uintptr_t ViewProjectionAddr = Proc.GetModuleAddress("engine.dll") + Offsets::ViewProjectionMatrix;
	Matrix44 ViewProjectionMatrix;
	DWORD MatrixBytes = 0x0;

	VMMDLL_Scatter_PrepareEx(vmsh, ViewProjectionAddr, sizeof(Matrix44), (BYTE*)&ViewProjectionMatrix, &MatrixBytes);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	if (MatrixBytes != sizeof(Matrix44))
	{
		std::println("[!] MatrixBytes != sizeof(MyMatrix)");
		return;
	}

	std::scoped_lock MatrixLock(m_ViewProjectionMatrixMutex);
	m_ViewProjectionMatrix = ViewProjectionMatrix;
}