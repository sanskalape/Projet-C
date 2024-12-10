#include "text_field.h"
#include <stdio.h>
#include <string.h>

// Fonction pour dessiner du texte (mise à jour pour gérer les chaînes vides)
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *rect, SDL_Color color) {
    if (strlen(text) == 0) {
        return; // Ne pas dessiner si le texte est vide
    }
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) {
        printf("Erreur de rendu du texte : %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur de création de texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
