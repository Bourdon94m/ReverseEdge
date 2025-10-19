#include "gui.h"
#include <iostream>

#include "ESP/ESP.h"
#include "AIMBOT/Aimbot.h"
#include "ImGui/backends/imgui_impl_win32.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include <MISC/MISC.h>

ESP esp2;
Aimbot aimbot2;
CheatVars vars;
HWND hwnd2 = FindWindowA(NULL, "AssaultCube");
ImVec2 middle = Globals::getMiddleOfTheScreen(hwnd2);



bool ImGuiWrapper::isTeamCheck()
{
    return vars.showTeam;
}

bool ImGuiWrapper::isMenuOpen()
{
    return vars.showMenu;
}

bool ImGuiWrapper::showHealthBar()
{
    return vars.showHealthBarEsp;
}


void ImGuiWrapper::InitGui(HWND hwnd) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();  // Supprime toutes les polices existantes
    ImFont* myFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Arial.ttf", 20.0f);
    io.FontDefault = myFont;  // Définit cette police comme celle par défaut
    io.Fonts->Build();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init("#version 130");
}




void ImGuiWrapper::RenderGui() {
    // Préparer une nouvelle frame ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Appliquer un style moderne
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 15.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(5, 5);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);

    // Personnalisation des couleurs
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.95f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.83f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.83f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.90f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.83f, 1.00f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.83f, 1.00f, 1.00f);

    // Position et taille de la fenêtre ImGui
    ImGui::SetWindowPos(middle);
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Always);

    // Début de la fenêtre principale
    ImGui::Begin("ReverseEdge",
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar
    );

   

    // Création du panneau de gauche dans un Child window
    ImGui::BeginChild("Left Panel", ImVec2(150, 0), true);
    static int selected = 0;
    if (ImGui::Selectable("Aimbot", selected == 0)) selected = 0;
    if (ImGui::Selectable("Visuals", selected == 1)) selected = 1;
    if (ImGui::Selectable("Misc", selected == 2)) selected = 2;
    if (ImGui::Selectable("Config", selected == 3)) selected = 3;
    ImGui::EndChild();

    // Panneau de droite pour le contenu
    ImGui::SameLine();
    ImGui::BeginChild("Content", ImVec2(0, 0), true);
    switch (selected) {


    case 0: // Aimbot
        ImGui::Text("Aimbot Settings");
        ImGui::Separator();
        ImGui::Checkbox("Enable Aimbot", &vars.enableAimbot);

        if (vars.enableAimbot)
        {
            ImGui::SliderFloat("FOV", &vars.fovRadius, 1.0f, 180.0f, "%.1f");
            ImGui::SliderFloat("Smooth", &vars.smooth, 1.0f, 100.0f, "%.1f");
            ImGui::Combo("Target", &vars.selectedConfig, "Head\0Chest\0Nearest\0");
            ImGui::Checkbox("Visible Only", &vars.showMenu);
            ImGui::Checkbox("Auto Fire", &vars.showMenu);
        }

        break;
        
    case 1: // Visuals
        ImGui::Text("Visual Settings");
        ImGui::Separator();
        ImGui::Checkbox("Box ESP", &vars.espBox);
        ImGui::Checkbox("Team check", &vars.showTeam);
        ImGui::SameLine(200);
        ImGui::ColorEdit3("BoxTeamColor", vars.boxTeamColor, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("HealthBar", &vars.showHealthBarEsp);


        ImGui::Checkbox("Lines ESP", &vars.linesESP);
        ImGui::SameLine(200);
        ImGui::ColorEdit3("BoxColor", vars.boxEnnemiColor, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("Skeleton", &vars.showMenu);
        ImGui::Checkbox("Health Bar", &vars.showMenu);
        ImGui::Checkbox("Name ESP", &vars.showMenu);
        ImGui::Checkbox("Weapon ESP", &vars.showMenu);
        ImGui::SliderFloat("ESP Distance", &vars.espDistance, 0.0f, 1000.0f, "%.0f m");
        break;

    case 2: // Misc
        ImGui::Text("Misc Features");
        ImGui::Separator();
        if (ImGui::Checkbox("Bunny Hop", &vars.bunnyHop)) MISC::BunnyHop;
        ImGui::Checkbox("Auto Strafe", &vars.showMenu);
        ImGui::Checkbox("No Flash", &vars.showMenu);
        ImGui::SliderFloat("Flash Alpha", &vars.smooth, 0.0f, 255.0f, "%.0f");
        ImGui::Checkbox("Radar Hack", &vars.showMenu);
        ImGui::Checkbox("Spectator List", &vars.showMenu);
        break;

    case 3: // Config
        ImGui::Text("Configuration");
        ImGui::Separator();
        if (ImGui::Button("Save Config", ImVec2(120, 30))) { /* Sauvegarder */ }
        ImGui::SameLine();
        if (ImGui::Button("Load Config", ImVec2(120, 30))) { /* Charger */ }
        ImGui::Spacing();
        ImGui::Text("Configs:");
        break;

    }


    if (vars.espBox) {
        esp2.DrawBox(vars.showTeam, vars.boxEnnemiColor, vars.boxTeamColor);
    }

    if (vars.showHealthBarEsp)
    {
        esp2.DrawHealthBar();
    }

    if (vars.linesESP) {
        esp2.DrawLines(vars.showTeam, vars.boxEnnemiColor);
    }
    

    if (vars.enableAimbot) {
        aimbot2.DrawFOV(vars.fovRadius, vars.fovColor, hwnd2);

        if (GetAsyncKeyState(0x56) & 0x8000)
        {
            aimbot2.RunAimbot(vars.fovRadius, vars.smooth);
        } 
        
    }

    //if (vars.bunnyHop)
    //{
    //    MISC::BunnyHop();
    //} 

    
    ImGui::EndChild();  
    
    ImGui::End(); // Fin de la fenêtre "ReverseEdge"
    
    // Rendu final d'ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::ShutdownGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


bool ImGuiWrapper::LoadFont(const char* fontPath, float size, const std::string& fontName, ImGuiIO& io) {
    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, size);
    if (!font) {
        std::cerr << "Erreur : Impossible de charger la police '" << fontPath << "'." << std::endl;
        return false;
    }

    // Stocker la police dans une map pour une réutilisation ultérieure
    fonts[fontName] = font;
    std::cout << "Police '" << fontName << "' chargée avec succès." << std::endl;
    return true;
}

bool ImGuiWrapper::LoadIconFont(const char* fontPath, float size, const std::string& fontName, ImGuiIO& io, const ImWchar* glyphRanges) {
    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, size, nullptr, glyphRanges);
    if (!font) {
        std::cerr << "Erreur : Impossible de charger la police d'icônes '" << fontPath << "'." << std::endl;
        return false;
    }

    // Stocker la police d'icônes dans une map pour une réutilisation ultérieure
    fonts[fontName] = font;
    std::cout << "Police d'icônes '" << fontName << "' chargée avec succès." << std::endl;
    return true;
}

void ImGuiWrapper::UseFont(const std::string& fontName) {
    if (fonts.find(fontName) != fonts.end()) {
        ImGui::PushFont(fonts[fontName]);
    }
    else {
        std::cerr << "Erreur : Police '" << fontName << "' non trouvée." << std::endl;
    }
}

void ImGuiWrapper::PopFont() {
    ImGui::PopFont();
}