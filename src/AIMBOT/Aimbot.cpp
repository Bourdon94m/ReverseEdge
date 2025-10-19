#include "Aimbot.h"
#include "ESP/ESP.h"
#include <iostream>
#include <vector>

HWND hwndAimbot = FindWindowA(NULL, "AssaultCube");
Globals globals(hwndAimbot);


const float M_PI = 3.141592653589793;
const int HEIGHT = globals.getWindowHeight();
const int WIDTH = globals.getWindowWidth();

Address address;
Offsets offsets;
uintptr_t ModuleBaseAddress2 = (uintptr_t)GetModuleHandle("ac_client.exe");
uintptr_t localPlayer2 = *(uintptr_t*)(ModuleBaseAddress2 + address.localPlayer);




void Aimbot::DrawFOV(float& FovRadius, ImU32 fovColor, HWND hwnd)
{
	ImVec2 middle = Globals::getMiddleOfTheScreen(hwnd);
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();


	draw_list->AddCircle(middle, FovRadius, fovColor, 0, 2.0f);
}

void Aimbot::RunAimbot(float fovRadius, float smoothing)
{
    if (!localPlayer2) return; 

    uintptr_t bestTarget = NULL;
    float minDistance = FLT_MAX;

    Matrix ViewMatrix = *(Matrix*)(address.ViewMatrix);
    uintptr_t entListBase = *(uintptr_t*)(ModuleBaseAddress2 + address.entList);

    if (!entListBase) return;

    for (int i = 0; i < 32; i++) {

        __try 
        {
            uintptr_t ent = *(uintptr_t*)(entListBase + i * 0x4);
            if (!ent) continue;

            int entHealth = *(int*)(ent + offsets.health);
            if (entHealth <= 0 || entHealth > 100) continue;


            short entTeam = *(short*)(ent + offsets.i_team);
            short playerTeam = *(short*)(localPlayer2 + offsets.i_team);
            if (playerTeam == entTeam) continue; 


            Vec3 entPos = *(Vec3*)(ent + 0x04); // Position 3D de l'ennemi

            Vec2 screenPos;
            if (WorldToScreen(entPos, screenPos, ViewMatrix.VMatrix, WIDTH, HEIGHT)) {
                float distanceToCenter = sqrt(pow(screenPos.X - WIDTH / 2, 2) + pow(screenPos.Y - HEIGHT / 2, 2));

                if (distanceToCenter < fovRadius) { // ✅ Vérifie si l'ennemi est dans la FOV
                    if (distanceToCenter < minDistance) { // ✅ Prend l'ennemi le plus proche du centre
                        minDistance = distanceToCenter;
                        bestTarget = ent;
                    }
                }
            }
        }

        __except (EXCEPTION_EXECUTE_HANDLER) // If any reading / writing access 
        {
            continue;
        }

        
        
       
    }


    // BUG here
    // ✅ Appliquer l'aimbot sur la meilleure cible trouvée
    if (bestTarget) 
    {
        AimAtTarget(bestTarget, smoothing);
    }
    
}

ViewAngles Aimbot::CalculateAngle(Vec3 localPos, Vec3 targetPos)
{
	float deltaX = targetPos.x - localPos.x;
    float deltaY = targetPos.y - localPos.y;
    float deltaZ = targetPos.z - localPos.z;



    std::cout << "X : " << deltaX << ", Y : " << deltaY << ", Z : " << deltaZ << std::endl;
    
    float yaw = atan2f(deltaY, deltaX) * (180.0f / M_PI);
    yaw += 90.0f; 
    float distance2D = sqrt(deltaX * deltaX + deltaY * deltaY);
    float pitch = atan2f(deltaZ, distance2D) * (180.0f / M_PI);

    return { yaw, pitch };

}

void Aimbot::AimAtTarget(uintptr_t target, float smoothing)
{
    if (!target)
    {
        std::cout << "Target is null" << std::endl;
        return;
    }

    Vec3 targetPos = *(Vec3*)(target + 0x04); // Ennemy pos X Y Z
    Vec3 localPlayerPos = *(Vec3*)(localPlayer2 + 0x04); // Player X Y Z
    targetPos.z += 0.3f;

    ViewAngles newAngle = CalculateAngle(localPlayerPos, targetPos);

    // Get current angles
    ViewAngles currentAngles = *(ViewAngles*)(localPlayer2 + 0x34);

    float smoothFactor = smoothing; // We adjust the smoothing of the aimbot
    currentAngles.yaw += (newAngle.yaw - currentAngles.yaw) / smoothFactor;
    currentAngles.pitch += (newAngle.pitch - currentAngles.pitch) / smoothFactor;

    // Apply the angles update
    *(ViewAngles*)(localPlayer2 + 0x34) = currentAngles;


}