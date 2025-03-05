#pragma once
#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include <unordered_map>
#include <string>
#include "ImGui/imgui.h"



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

private:
    std::unordered_map<std::string, ImFont*> fonts; // Stocke les polices chargées
};

#endif
