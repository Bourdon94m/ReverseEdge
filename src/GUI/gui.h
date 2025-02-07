#pragma once
#include <unordered_map>
#include <string>
#include "ImGui/imgui.h"
#include "../../Include/GLFW/glfw3.h"

class ImGuiWrapper {
public:
    ImGuiWrapper() = default;

    void InitGui(GLFWwindow* window);
    void RenderGui();
    void ShutdownGui();

    bool LoadFont(const char* fontPath, float size, const std::string& fontName, ImGuiIO& io);
    bool LoadIconFont(const char* fontPath, float size, const std::string& fontName, ImGuiIO& io, const ImWchar* glyphRanges = nullptr);
    void UseFont(const std::string& fontName);
    void PopFont();

private:
    std::unordered_map<std::string, ImFont*> fonts; // Stocke les polices chargées
};