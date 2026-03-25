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
	MyColorEdit4("Dormant", Dormant);
	MyColorEdit4("Unknown", Unknown);
	MyColorEdit4("Skeleton", Skeleton);
	MyColorEdit4("Highlighted Skeleton", HighlightedSkeleton);
	MyColorEdit4("Max FOV", MaxFOV);
	MyColorEdit4("Deadzone FOV", DeadzoneFOV);
	MyColorEdit4("Health Pack", HealthPack);
	MyColorEdit4("Ammo Pack", AmmoPack);
	MyColorEdit4("Money", Money);
	MyColorEdit4("Rocket", Rocket);
	MyColorEdit4("Sticky", Sticky);
	MyColorEdit4("Value Bar Background", ValueBarBackground);

	ImGui::End();
}
