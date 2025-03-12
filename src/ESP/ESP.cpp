#include <Windows.h>
#include "ESP.h"
#include <iostream>
#include "../framework.h"
#include "../utilities/globals.h"
#include "ImGui/imgui.h"


HWND hwnd = FindWindowA(NULL, "AssaultCube");

Globals glo;
static int WIDTH =  glo.getWindowWidth(hwnd);
static int HEIGHT = glo.getWindowHeight(hwnd);


// Adresse de base du module (ac_client.exe)
uintptr_t ModuleBaseAddress = (uintptr_t)GetModuleHandle("ac_client.exe");
Offsets off;
Address addr;

int numOfPlayersInGame = *(int*)(addr.numOfPlayers);
uintptr_t localPlayer = ModuleBaseAddress + addr.localPlayer;

int myPlayerTeam = *(int*)(localPlayer + off.i_team);


// Fonction pour récupérer la position de l'ennemi
Vec3 ESP::GetEnnemyPos()
{
    for (int i = 0; i < 32; i++)
    {
        // Accéder à la liste des entités
        uintptr_t entListPtr = *(uintptr_t*)(addr.entList);
        uintptr_t ent = *(uintptr_t*)(entListPtr + i * 0x4);

        if (!ent) continue; // Vérifier si l'entité est valide

        // Lire la santé et la position de l'entité
        int entHealth = *(int*)(ent + off.health);
        Vec3 entPos = *(Vec3*)(ent + 0x4);

        // Vérifier que l'entité est en vie
        if (entHealth > 100 || entHealth < 1) continue;

        std::cout << "Ent[" << i << "]" << " Health : " << entHealth << std::endl;
        std::cout << "Ent[" << i << "]" << " Pos : " << "{ " << entPos.x << ", " << entPos.y << " }" << std::endl;

        // Retourner la position de l'entité si c'est le dernier joueur
        if (i == numOfPlayersInGame - 1)
        {
            return entPos;
        }
    }

    std::cout << "Failed !" << std::endl;
    return { 0, 0, 0 }; // Retourne une position nulle si aucune entité n'est trouvée
}

// Fonction pour dessiner des lignes vers les ennemis
void ESP::DrawLines(bool showTeam, float* color)
{
    // Vérification préalable des pointeurs
    if (!addr.ViewMatrix) return;
    Matrix ViewMatrix = *(Matrix*)(addr.ViewMatrix);

    uintptr_t entListBase = *(uintptr_t*)(ModuleBaseAddress + addr.entList);
    
    if (!entListBase) return;
    
    for (int i = 0; i < 32; i++) {
        // Vérification plus robuste
        __try {
            uintptr_t ent = *(uintptr_t*)(entListBase + i * 0x4);
            if (!ent) continue;

            int entHealth = *(int*)(ent + off.health);
            int entTeam = *(int*)(ent + off.i_team);
            if (entHealth <= 0 || entHealth > 100) continue;

            Vec3 entPos = *(Vec3*)(ent + 0x04); // Player position

            Vec2 ScreenCoords;
            if (WorldToScreen(entPos, ScreenCoords, ViewMatrix.VMatrix, WIDTH, HEIGHT)) {
                ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
                ImU32 entityColor;

                if (entTeam == myPlayerTeam) {
                    if (!showTeam) continue; // Ne pas afficher les alliés si showTeam == false
                    entityColor = ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], 1.0f)); // Teammate

                }
                else
                {
                    entityColor = ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], 1.0f)); // Ennemy

                }

                draw_list->AddLine(
                    ImVec2(WIDTH / 2, HEIGHT),
                    ImVec2(ScreenCoords.X, ScreenCoords.Y),
                    entityColor,
                    2.0f
                );
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            continue;
        }

    }
}

// Fonction pour obtenir le nombre de joueurs dans le jeu
int ESP::GetPlayersInGame() {
    return *(int*)(addr.numOfPlayers); // Lire directement le nombre de joueurs dans le jeu
}


void ESP::DrawBox(bool showTeam, float* EnnemiColor, float* TeamColor)
{
    if (!addr.ViewMatrix) return;
    Matrix ViewMatrix = *(Matrix*)(addr.ViewMatrix);

    uintptr_t entListBase = *(uintptr_t*)(ModuleBaseAddress + addr.entList);
    if (!entListBase) return;

    for (int i = 0; i < 32; i++)
    {
        __try {
            uintptr_t ent = *(uintptr_t*)(entListBase + i * 0x4);
            if (!ent) continue;

            int entHealth = *(int*)(ent + off.health);
            int entTeam = *(int*)(ent + off.i_team);
            int myPlayerTeam = *(int*)(localPlayer + off.i_team);

            // Récupération des positions 3D
            Vec3 entHeadPos = *(Vec3*)(ent + 0x04); // Position de la tête
            Vec3 entFeetPos = *(Vec3*)(ent + 0x28); // Position des pieds


            entHeadPos.z += 2.0f;

            if (entHealth <= 0 || entHealth > 100) continue;

            // Convertir les coordonnées monde -> écran
            Vec2 screenHead, screenFeet;

          
            if (WorldToScreen(entHeadPos, screenHead, ViewMatrix.VMatrix, WIDTH, HEIGHT) &&
                WorldToScreen(entFeetPos, screenFeet, ViewMatrix.VMatrix, WIDTH, HEIGHT))
            {

                float height = abs(screenFeet.Y - screenHead.Y + 4.0f); // Hauteur du joueur
                float width = height / 2.0f; // Largeur basée sur la hauteur 

                ImDrawList* drawList = ImGui::GetBackgroundDrawList();
                ImU32 entityColor;

                if (entTeam == myPlayerTeam)
                {
                    if (!showTeam) continue;
                    entityColor = ImGui::GetColorU32(ImVec4(EnnemiColor[0], EnnemiColor[1], EnnemiColor[2], 1.0f));
                }

                else
                {
                    entityColor = ImGui::GetColorU32(ImVec4(TeamColor[0], TeamColor[1], TeamColor[2], 1.0f)); 
                }

                // Dessin de la boîte bien centrée autour du joueur
                drawList->AddRect(
                    ImVec2(screenHead.X - width / 2, screenHead.Y),   // Haut gauche
                    ImVec2(screenHead.X + width / 2, screenFeet.Y),   // Bas droit
                    entityColor,
                    0.0f,
                    0,
                    3.0f
                );
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            continue;
        }
    }
}
