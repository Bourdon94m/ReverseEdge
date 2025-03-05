#include "Math.h"


bool WorldToScreen(const Vec3& worldPos, Vec2& screen, float matrix[16], int screenWidth, int screenHeight) {
    float clipX = worldPos.x * matrix[0] + worldPos.y * matrix[4] + worldPos.z * matrix[8] + matrix[12];
    float clipY = worldPos.x * matrix[1] + worldPos.y * matrix[5] + worldPos.z * matrix[9] + matrix[13];
    float clipW = worldPos.x * matrix[3] + worldPos.y * matrix[7] + worldPos.z * matrix[11] + matrix[15];

    if (clipW < 0.1f) return false; // Si l'entité est derrière la caméra, on ne l'affiche pas

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    screen.X = (screenWidth / 2.0f) * (ndcX + 1.0f);
    screen.Y = (screenHeight / 2.0f) * (1.0f - ndcY); // Correction de l'axe Y

    return true;
}


