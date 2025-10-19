#pragma once
#ifndef AIMBOT_H
#define AIMBOT_H


#include "ImGui/imgui.h"
#include "../src/utilities/Math.h"
#include "../utilities/globals.h"

class Aimbot
{
public:

	void DrawFOV(float& FovRadius, ImU32 fovColor, HWND hwnd);
	void RunAimbot(float fovRadius, float smoothing); 

private:
	ViewAngles CalculateAngle(Vec3 localPos, Vec3 targetPos);
	void AimAtTarget(uintptr_t target, float smoothing);

};



#endif