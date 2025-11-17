#include "pch.h"
#include "Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"

void MyColorEdit4(const char* label, ImColor& color)
{
	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4(label, &color.Value.x);
}

void ColorPicker::RenderColorPicker()
{
	ImGui::Begin("Color Picker");

	MyColorEdit4("Red Team", RedTeam);
	MyColorEdit4("Blue Team", BluTeam);
	MyColorEdit4("Spectator", Spectator);
	MyColorEdit4("Dormant", Dormant);
	MyColorEdit4("Local Player", LocalPlayer);
	MyColorEdit4("Incomplete", Incomplete);
	MyColorEdit4("Spy Alert", SpyAlert);
	MyColorEdit4("Unknown", Unknown);
	MyColorEdit4("Health Bar Background", HealthBarBackground);
	MyColorEdit4("Health Bar Foreground", HealthBarForeground);
	MyColorEdit4("Health Bar Overheal", HealthBarOverheal);
	MyColorEdit4("Skeleton", Skeleton);
	MyColorEdit4("Highlighted", Highlighted);
	MyColorEdit4("FOV Circle", FOVCircle);
	MyColorEdit4("Health Pack", HealthPack);
	MyColorEdit4("Ammo Pack", AmmoPack);
	MyColorEdit4("Money", Money);

	ImGui::End();
}
