#include "Globals.h"

Screen Globals::getScreenSize(HWND hwnd) {
    Screen s;
    RECT rect;

    if (GetWindowRect(hwnd, &rect)) {
        s.WIDTH = rect.right - rect.left;
        s.HEIGHT = rect.bottom - rect.top;
    }
    else {
        std::cout << "Erreur lors de la récupération de la taille de la fenêtre." << std::endl;
        s.WIDTH = 0;
        s.HEIGHT = 0;
    }

    return s;
}
