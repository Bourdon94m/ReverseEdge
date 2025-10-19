#include <Windows.h>
#include "ESP.h"
#include <iostream>
#include "../framework.h"
#include "../utilities/globals.h"
#include "ImGui/imgui.h"
#include "GUI/gui.h"


HWND hwnd = FindWindowA(NULL, "AssaultCube");
Globals glob(hwnd);

int WIDTH = glob.getWindowWidth();
int HEIGHT = glob.getWindowHeight();


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
    Vec3 playerPos = *(Vec3*)(localPlayer + 0x04);

    std::cout << "[X Y Z]" << playerPos.x << " , " << playerPos.y << " , " << playerPos.z << std::endl;
    
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

                if (myPlayerTeam == entTeam) {
                    if (!showTeam) continue; // Ne pas afficher les alliés si showTeam == false
                    entityColor = ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], 1.0f)); // Teammate

                }
                else
                {
                    entityColor = ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], 1.0f)); // Ennemy

                }

                draw_list->AddLine(
                    ImVec2(WIDTH / 2, 0),
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
int ESP::GetPlayersInGame() 
{
    return *(int*)(addr.numOfPlayers); 
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
            int entTeam = *(short*)(ent + off.i_team);
            int myPlayerTeam = *(short*)(localPlayer + off.i_team);

            // Récupération des positions 3D
            Vec3 entHeadPos = *(Vec3*)(ent + 0x04); // Position de la tête
            Vec3 entFeetPos = *(Vec3*)(ent + 0x28); // Position des pieds


            entHeadPos.z += 1.0f;

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
                    entityColor = ImGui::GetColorU32(ImVec4(TeamColor[0], TeamColor[1], TeamColor[2], 1.0f));
                }

                else
                {
                    entityColor = ImGui::GetColorU32(ImVec4(EnnemiColor[0], EnnemiColor[1], EnnemiColor[2], 1.0f));
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


void ESP::DrawHealthBar()
{
    // Vérification préalable des pointeurs
    if (!addr.ViewMatrix) return;
    Matrix ViewMatrix = *(Matrix*)(addr.ViewMatrix);
    uintptr_t entListBase = *(uintptr_t*)(ModuleBaseAddress + addr.entList);


    

    Vec2 ScreenHeadCoords;
    Vec2 ScreenFeetCoords;
    


    if (!entListBase) return;

    for (int i = 0; i < 32; i++) {
        // Vérification plus robuste

        __try {

            uintptr_t ent = *(uintptr_t*)(entListBase + i * 0x4);
            if (!ent) continue;

            int entHealth = *(int*)(ent + off.health);
            int maxEntHealth = 100;
            int healthPercent = entHealth / maxEntHealth;
            if (entHealth <= 0 || entHealth > 100) continue;

            // Team check
            if ( *(short*)(ent + off.i_team) == *(short*)(localPlayer + off.i_team) ) 
            {
                if (!ImGuiWrapper::isTeamCheck)
                {
                    continue;
                }
            }

            
            

            Vec3 playerHeadPos = *(Vec3*)(ent + 0x04); // Player head position
            Vec3 playerFeetPos = *(Vec3*)(ent + 0x28); // Player feet position

            ScreenFeetCoords.Y += 2.0f;


            if (WorldToScreen(playerHeadPos, ScreenHeadCoords, ViewMatrix.VMatrix, WIDTH, HEIGHT) &&
                WorldToScreen(playerFeetPos, ScreenFeetCoords, ViewMatrix.VMatrix, WIDTH, HEIGHT))
            {

                float boxHeight = abs(ScreenFeetCoords.Y - ScreenHeadCoords.Y); // Hauteur du joueur
                float width = boxHeight / 2.0f; // Largeur basée sur la hauteur 

                ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
                ImU32 entityColor;


                draw_list->AddRectFilled(
                    ImVec2(ScreenHeadCoords.X - width - 5.0f, ScreenFeetCoords.Y - boxHeight),   // coin haut gauche
                    ImVec2(ScreenHeadCoords.X - width - 5.0f, ScreenFeetCoords.Y),               // coin bas droit
                    ImGui::GetColorU32(ImVec4(0, 255, 0, 255)),
                    0.0f
                );


            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            continue;
        }

    }
}