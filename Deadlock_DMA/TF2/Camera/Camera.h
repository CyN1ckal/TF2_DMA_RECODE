#pragma once

#include "Engine/Matrix.h"

class Camera
{
public:
	static void UpdateViewProjectionMatrix(DMA_Connection* Conn);

	static Matrix44 GetViewProjectionMatrix()
	{
		std::scoped_lock MatrixLock(m_ViewProjectionMatrixMutex);
		return m_ViewProjectionMatrix;
	}

private:
	static inline std::mutex m_ViewProjectionMatrixMutex;	
	static inline Matrix44 m_ViewProjectionMatrix;
};