#include <Windows.h>
#include "ESP.h"
#include <iostream>
#include "../framework.h"
#include "../utilities/globals.h"


const int WIDTH = 1920;
const int HEIGHT = 1080;


// Adresse de base du module (ac_client.exe)
uintptr_t ModuleBaseAddress = (uintptr_t)GetModuleHandle("ac_client.exe");
Offsets off;
Address addr;

uintptr_t* localPlayerAddress = (uintptr_t*)addr.localPlayer;
int numOfPlayersInGame = *(int*)(addr.numOfPlayers);

// Fonction pour récupérer la position de l'ennemi
Vec3 ESP::GetEnnemyPos()
{
    for (int i = 0; i < numOfPlayersInGame; i++)
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
void ESP::DrawLines()
{
    // Vérification préalable des pointeurs
    if (!addr.ViewMatrix) return;
    Matrix ViewMatrix = *(Matrix*)(addr.ViewMatrix);

    uintptr_t entListBase = *(uintptr_t*)(ModuleBaseAddress + addr.entList);
    
    if (!entListBase) return;
    
    for (int i = 0; i < numOfPlayersInGame; i++) {
        // Vérification plus robuste
        __try {
            uintptr_t ent = *(uintptr_t*)(entListBase + i * 0x4);
            if (!ent) continue;

            int entHealth = *(int*)(ent + off.health);
            if (entHealth <= 0 || entHealth > 100) continue;

            Vec3 entPos = *(Vec3*)(ent + 0x4);

            Vec2 ScreenCoords;
            if (WorldToScreen(entPos, ScreenCoords, ViewMatrix.VMatrix, WIDTH, HEIGHT)) {
                ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
                draw_list->AddLine(
                    ImVec2(WIDTH / 2, HEIGHT),
                    ImVec2(ScreenCoords.X, ScreenCoords.Y),
                    ImGui::GetColorU32(ImVec4(1, 0, 0, 1)),
                    2.0f
                );
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            // Gestion de l'exception
            continue;
        }
    }
}

// Fonction pour obtenir le nombre de joueurs dans le jeu
int ESP::GetPlayersInGame() {
    return *(int*)(addr.numOfPlayers); // Lire directement le nombre de joueurs dans le jeu
}
