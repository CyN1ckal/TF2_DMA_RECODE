#pragma once

class ESP
{
private:
	static inline bool bMasterToggle{ true };
	static inline bool bSettings{ true };

public:
	static void OnFrame();
	static void RenderSettings();
};