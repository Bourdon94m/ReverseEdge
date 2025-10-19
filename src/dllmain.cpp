#include <Windows.h>
#include <iostream>
#include <gl/GL.h>
#include "ImGui/imgui.h"
#include "MINHOOK/MinHook.h"
#include "ImGui/backends/imgui_impl_win32.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ESP/ESP.h"
#include "AIMBOT/Aimbot.h"
#include "GUI/gui.h"
#include <MISC/MISC.h>
#include <thread>

#pragma comment(lib, "opengl32.lib")



// Variables globales
bool g_initialized = false;
bool g_wallhack = false;
HWND g_hwnd = nullptr;
HDC g_hdc = nullptr;
HGLRC g_hglrc = nullptr;
ImGuiWrapper* gui;
bool showMenu = true;
ESP esp;
Aimbot aimbot;

// Déclaration du gestionnaire de messages ImGui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Hook pour WndProc pour capturer les entrées
WNDPROC original_wndproc = nullptr;
LRESULT CALLBACK Hooked_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (g_initialized && showMenu)
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
            return true; // Bloque les événements souris/clavier si ImGui les utilise

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse || io.WantCaptureKeyboard)
            return true; // Ne passe pas l'événement au jeu
    }

    return CallWindowProc(original_wndproc, hwnd, msg, wparam, lparam);
}

void InitImGui(HWND hwnd, HDC hdc)
{
    // Créer et sauvegarder le contexte OpenGL
    g_hglrc = wglCreateContext(hdc);
    HGLRC old_context = wglGetCurrentContext();
    HDC old_dc = wglGetCurrentDC();

    // Activer notre contexte
    wglMakeCurrent(hdc, g_hglrc);

    // Initialiser ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Initialiser les backends ImGui
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init();

    // Hook le WndProc pour capturer les entrées
    original_wndproc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Hooked_WndProc);

    // Restaurer le contexte original
    wglMakeCurrent(old_dc, old_context);
}

void CleanupImGui()
{
    // Sauvegarder le contexte actuel
    HGLRC old_context = wglGetCurrentContext();
    HDC old_dc = wglGetCurrentDC();

    // Activer notre contexte
    wglMakeCurrent(g_hdc, g_hglrc);

    // Libérer ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Restaurer le WndProc original
    if (g_hwnd && original_wndproc)
        SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)original_wndproc);

    // Restaurer le contexte original
    wglMakeCurrent(old_dc, old_context);

    // Supprimer notre contexte
    if (g_hglrc)
        wglDeleteContext(g_hglrc);
}

// Fonction de swap buffer originale
typedef BOOL(WINAPI* wglSwapBuffers_t)(HDC);
wglSwapBuffers_t original_wglSwapBuffers = nullptr;

// Fonction de swap buffer hookée
BOOL WINAPI Hooked_wglSwapBuffers(HDC hdc)
{

    // Capturer la fenêtre si pas encore fait
    if (!g_hwnd) {
        g_hwnd = WindowFromDC(hdc);
        g_hdc = hdc;

        if (g_hwnd && !g_initialized) 
        {
            InitImGui(g_hwnd, hdc);
            g_initialized = true;
        }
    }

    if (g_initialized) {
        // Sauvegarder le contexte actuel
        HGLRC old_context = wglGetCurrentContext();
        HDC old_dc = wglGetCurrentDC();

        // Activer notre contexte
        wglMakeCurrent(hdc, g_hglrc);


        // Afficher le menu si nécessaire
        if (showMenu) {
            gui->RenderGui();
        }

        if (!showMenu) // if menu closed we draw functionnalities
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // ✅ Toujours dessiner les ESP et le FOV, même si le menu est fermé
            ImDrawList* drawList = ImGui::GetBackgroundDrawList();

            if (vars.espBox) {
                esp.DrawBox(vars.showTeam, vars.boxEnnemiColor, vars.boxTeamColor);
            }

            if (vars.showHealthBarEsp)
            {
                esp.DrawHealthBar();
            }

            if (vars.linesESP) 
            {
                esp.DrawLines(vars.showTeam, vars.boxEnnemiColor);
            }

            if (vars.bunnyHop)
            {
                std::thread(MISC::BunnyHop).detach();
            }

            

            if (vars.enableAimbot) 
            {
                aimbot.DrawFOV(vars.fovRadius, vars.fovColor, g_hwnd);

                if (GetAsyncKeyState(0x56) & 0x8000)
                {
                    aimbot.RunAimbot(vars.fovRadius, vars.smooth);
                }
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        }
       
        // Restaurer le contexte original
        wglMakeCurrent(old_dc, old_context);
    }

    // Appeler la fonction originale
    return original_wglSwapBuffers(hdc);
}

// Thread principal du hack
DWORD WINAPI HackThread(HMODULE hModule)
{

    FILE* file;
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);

    // Attendre que le jeu s'initialise
    Sleep(2000);

    // Initialiser MinHook
    if (MH_Initialize() != MH_OK)
    {
        std::cerr << "Failed to initialize MinHook\n";
        return 1;
    }

    // Trouver l'adresse de wglSwapBuffers
    HMODULE opengl_module = GetModuleHandle("opengl32.dll");
    if (!opengl_module)
    {
        std::cerr << "Failed to get OpenGL32.dll handle\n";
        return 1;
    }

    wglSwapBuffers_t wglSwapBuffers_addr = (wglSwapBuffers_t)GetProcAddress(opengl_module, "wglSwapBuffers");
    if (!wglSwapBuffers_addr)
    {
        std::cerr << "Failed to get wglSwapBuffers address\n";
        return 1;
    }

    // Créer et activer le hook
    if (MH_CreateHook(wglSwapBuffers_addr, &Hooked_wglSwapBuffers, (LPVOID*)&original_wglSwapBuffers) != MH_OK)
    {
        std::cerr << "Failed to create hook for wglSwapBuffers\n";
        return 1;
    }

    if (MH_EnableHook(wglSwapBuffers_addr) != MH_OK)
    {
        std::cerr << "Failed to enable hook for wglSwapBuffers\n";
        return 1;
    }

    // Boucle principale - attendre END pour quitter
    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            FreeLibraryAndExitThread(hModule, 0);
            break;
        }

        if (GetAsyncKeyState(VK_F5) & 1)
        {
            // Toggle button
            showMenu ? showMenu = false : showMenu = true;
        }
    }

    FreeConsole();

  
    return 0;
}

// Point d'entrée DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
    }
    return TRUE;
}