#include "globals.h"


Globals::Globals(HWND hwnd) : hwnd(hwnd) {}  // Correct, hwnd appartient maintenant � l'instance


// R�cup�re la largeur de la fen�tre
int Globals::getWindowWidth() {
    RECT rect;
    if (GetClientRect(hwnd, &rect)) {
        return rect.right - rect.left;
    }
    return 0; // Retourne 0 si l'op�ration �choue
}

// R�cup�re la hauteur de la fen�tre
int Globals::getWindowHeight() {
    RECT rect;
    if (GetClientRect(hwnd, &rect)) {
        return rect.bottom - rect.top;
    }
    return 0; // Retourne 0 si l'op�ration �choue
}

ImVec2 Globals::getMiddleOfTheScreen(HWND hwnd)
{
    RECT rect;
    if (GetClientRect(hwnd, &rect)) {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        return ImVec2(width / 2.0f, height / 2.0f);
    }
    return ImVec2(0, 0); // Retourne un point (0,0) en cas d'erreur
}

