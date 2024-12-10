#include "graphics.h"

// Fonction pour dessiner un dégradé
void drawGradient(SDL_Renderer *renderer, int width, int height, SDL_Color startColor, SDL_Color endColor) {
    for (int y = 0; y < height; y++) {
        float ratio = (float)y / height;
        Uint8 r = (Uint8)((1 - ratio) * startColor.r + ratio * endColor.r);
        Uint8 g = (Uint8)((1 - ratio) * startColor.g + ratio * endColor.g);
        Uint8 b = (Uint8)((1 - ratio) * startColor.b + ratio * endColor.b);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, width, y);
    }
}
