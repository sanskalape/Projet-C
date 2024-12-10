#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include <SDL.h>
#include <SDL_ttf.h>

#define MAX_TEXT_LENGTH 100

typedef struct {
    char text[MAX_TEXT_LENGTH];
    SDL_Rect rect;
    int is_active;
} TextField;

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *rect, SDL_Color color);

#endif

