#pragma once
#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include <unordered_map>
#include <string>
#include "ImGui/imgui.h"


struct CheatVars {

    // Global
    bool showMenu = true;
    bool showTeam = false;

    // Misc
    bool bunnyHop = false;
    int selectedConfig = 0;

    // ESP
    bool espBox = false;
    bool linesESP = false;
    float boxEnnemiColor[4] = { 255.0f, 0.0f, 0.0f, 1.0f };
    float boxTeamColor[4] = { 0.0f, 255.0f, 0.0f, 1.0f };
    bool skeleton = false;
    float espDistance = 500.0f;
    bool boxESP = false;
    bool showHealthBarEsp = false;

    // AIM
    bool enableAimbot = false;
    bool showFov = false;
    float fovRadius = 10.0f;
    ImU32 fovColor = IM_COL32(255, 0, 0, 255);
    float smooth = 5.0f;


};

// Vars
extern CheatVars vars;


class ImGuiWrapper {
public:
    ImGuiWrapper() = default;

    void InitGui(HWND hwnd);
    void RenderGui();
    void ShutdownGui();

    bool LoadFont(const char* fontPath, float size, const std::string& fontName, ImGuiIO& io);
    bool LoadIconFont(const char* fontPath, float size, const std::string& fontName, ImGuiIO& io, const ImWchar* glyphRanges = nullptr);
    void UseFont(const std::string& fontName);
    void PopFont();

    // MISC
    static bool isTeamCheck();
    static bool isMenuOpen();
    static bool showHealthBar();


private:
    std::unordered_map<std::string, ImFont*> fonts; // Stocke les polices chargées
};

#endif
