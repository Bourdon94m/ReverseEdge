#include "gui.h"
#include <iostream>
#include "ImGui/backends/imgui_impl_glfw.h"
#include "../../Include/ImGui/imgui.h"
#include "../../Include/ImGui/backends/imgui_impl_opengl3.h"

bool showMenu = true;

struct CheatVars {
    bool showMenu = true;
    bool enableAimbot = false;
    float fov = 90.0f;
    float smooth = 5.0f;
    bool boxESP = false;
    float boxColor[3] = { 1.0f, 0.0f, 0.0f };
    bool skeleton = false;
    float espDistance = 500.0f;
    bool bhop = false;
    int selectedConfig = 0;
} vars;


void ImGuiWrapper::InitGui(GLFWwindow* window) {
    // Vérification de la version d'ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Configuration de ImGuiIO (optionnel)
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Activer la navigation au clavier

    // Load font
    LoadFont("../../Fonts/Roboto.ttf", 15.0f, "Roboto Thin", io);

    // Style d'ImGui (optionnel)
    ImGui::StyleColorsDark();

    // Initialisation des backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}



void ImGuiWrapper::RenderGui() {
    // Préparer une nouvelle frame ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
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
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);

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
        ImGui::Checkbox("Enable Aimbot", &vars.showMenu);
        ImGui::SliderFloat("FOV", &vars.fov, 0.0f, 180.0f, "%.1f°");
        ImGui::SliderFloat("Smooth", &vars.smooth, 1.0f, 100.0f, "%.1f");
        ImGui::Combo("Target", &vars.selectedConfig, "Head\0Chest\0Nearest\0");
        ImGui::Checkbox("Visible Only", &vars.showMenu);
        ImGui::Checkbox("Auto Fire", &vars.showMenu);
        break;

    case 1: // Visuals
        ImGui::Text("Visual Settings");
        ImGui::Separator();
        ImGui::Checkbox("Box ESP", &vars.showMenu);
        ImGui::SameLine(200);
        ImGui::ColorEdit3("##BoxColor", (float*)&vars.showMenu, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("Skeleton", &vars.showMenu);
        ImGui::Checkbox("Health Bar", &vars.showMenu);
        ImGui::Checkbox("Name ESP", &vars.showMenu);
        ImGui::Checkbox("Weapon ESP", &vars.showMenu);
        ImGui::SliderFloat("ESP Distance", &vars.espDistance, 0.0f, 1000.0f, "%.0f m");
        break;

    case 2: // Misc
        ImGui::Text("Misc Features");
        ImGui::Separator();
        ImGui::Checkbox("Bunny Hop", &vars.showMenu);
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
    ImGui::EndChild();

    ImGui::End(); // Fin de la fenêtre "ReverseEdge"

    // Rendu final d'ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::ShutdownGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
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