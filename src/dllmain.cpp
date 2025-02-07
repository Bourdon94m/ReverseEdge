#include <windows.h>
#include <iostream>
#include <gl/GL.h>
#include <MINHOOK/MinHook.h>
#include "../Include/GLFW/glfw3.h"
#include "../Include/GLAD/glad.h"
#include "GUI/gui.h"


// Déclarer les variables globales
GLFWwindow* g_Window = nullptr;
ImGuiWrapper* gui = nullptr;


// Typedef et pointeur vers la fonction originale
typedef int(__fastcall* wglSwapBuffersPtr)(HDC);
wglSwapBuffersPtr originalSwapBuffers = nullptr;

// Detour function
int __fastcall HookedwglSwapBuffers(HDC hdc)
{
    if (!wglGetCurrentContext() || !gui)
        return originalSwapBuffers(hdc);

    // Démarrer la nouvelle frame ImGui
    gui->InitGui(g_Window);

    // Récupérer le Background Draw List d'ImGui pour dessiner directement sur le jeu
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    // Exemples de dessin dans le jeu :
    // 1. Ligne rouge
    draw_list->AddLine(ImVec2(50, 80), ImVec2(200, 200), IM_COL32(255, 0, 0, 255), 2.0f);

    // 2. Cercle bleu
    draw_list->AddCircle(ImVec2(300, 300), 50, IM_COL32(0, 0, 255, 255), 0, 3.0f);

    // 3. Texte vert (exemple d'affichage de FPS)
    draw_list->AddText(ImVec2(20, 20), IM_COL32(0, 255, 0, 255), "FPS: 60");

    // Fin du rendu ImGui
    gui->ShutdownGui();

    return originalSwapBuffers(hdc);
}



DWORD WINAPI HackThread(HMODULE hModule)
{
    // Récupérer l'adresse de wglSwapBuffers
    uintptr_t AddressToHook = (uintptr_t)GetProcAddress(GetModuleHandle("opengl32.dll"), "wglSwapBuffers");
    if (!AddressToHook)
    {
        std::cerr << "Failed to find wglSwapBuffers address!\n";
        return 1;
        
    }

    // Initialiser GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW init failed\n";
        return -1;
    }

    // Configurer et créer la fenêtre GLFW
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    g_Window = glfwCreateWindow(500, 500, "ReverseEdge", NULL, NULL);
    if (!g_Window)
    {
        std::cout << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    // Centrer la fenêtre
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    glfwSetWindowPos(g_Window, (mode->width - 500) / 2, (mode->height - 500) / 2);

    // Activer OpenGL
    glfwMakeContextCurrent(g_Window);
    if (!gladLoadGL()) return -1;

    // Initialiser ImGui
    gui = new ImGuiWrapper();
    gui->InitGui(g_Window);

    // Initialiser MinHook
    if (MH_Initialize() != MH_OK)
    {
        std::cerr << "Failed to initialise MinHook\n";
        return 1;
    }

    // Créer le hook
    if (MH_CreateHook((LPVOID)AddressToHook, &HookedwglSwapBuffers, (LPVOID*)&originalSwapBuffers) != MH_OK)
    {
        std::cerr << "Failed to create hook for wglSwapBuffers\n";
        return 1;
    }

    if (originalSwapBuffers == nullptr)
    {
        std::cerr << "Error: originalSwapBuffers is still nullptr!\n";
        return 1;
    }

    // Activer le hook
    if (MH_EnableHook((LPVOID)AddressToHook) != MH_OK)
    {
        std::cerr << "Failed to enable hook for wglSwapBuffers\n";
        return 1;
    }

    // Boucle principale
    bool showMenu = true;
    while (!glfwWindowShouldClose(g_Window))
    {
        if (GetAsyncKeyState(VK_F10) & 1)
        {
            showMenu = !showMenu;
            if (showMenu) glfwShowWindow(g_Window);
            else glfwHideWindow(g_Window);
        }

        glfwPollEvents();
    }

    // Nettoyage
    MH_DisableHook((LPVOID)AddressToHook);
    MH_Uninitialize();
    gui->ShutdownGui();
    delete gui;
    glfwTerminate();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
    }
    return TRUE;
}