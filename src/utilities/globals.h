#pragma once
#include <windows.h>
#include "Imgui/imgui.h"


struct ViewAngles
{
    float yaw; // X in cartesian 
    float pitch; // Y in cartesian
};

class Globals
{
public:
    Globals(HWND hwnd);

    int getWindowWidth();
    int getWindowHeight();
    static ImVec2 getMiddleOfTheScreen(HWND hwnd);

private:
    HWND hwnd; // `hwnd` appartient uniquement à l'instance de `Globals`
};