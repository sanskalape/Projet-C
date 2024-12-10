#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include "text_field.h"
#include "graphics.h"
#include "audio.h"
#include "question.h"
#include "random_audio.h"

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur d'initialisation SDL : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("Erreur d'initialisation SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Blind test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de fenêtre : %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Initialisation des champs de texte
    TextField fields[2] = {
            {"", {200, 150, 240, 40}, 1}, // Champ "Prénom"
            {"", {200, 250, 240, 40}, 0}  // Champ "Nom"
    };

    // Définition du bouton "Jouer"
    SDL_Rect buttonRect = {270, 350, 100, 50};
    int buttonClicked = 0;

    // Couleurs
    SDL_Color startColor = {139, 0, 139, 255}; // Violet foncé
    SDL_Color endColor = {255, 20, 147, 255}; // Rose foncé
    SDL_Color fieldColor = {255, 255, 255, 255}; // Blanc pour l'intérieur des champs
    SDL_Color borderColor = {211, 211, 211, 255}; // Gris clair pour les contours
    SDL_Color textColor = {0, 0, 0, 255};        // Noir pour le texte
    SDL_Color labelColor = {0, 0, 0, 255};       // Noir pour les labels
    SDL_Color buttonColor = {0, 255, 0, 255};    // Vert pour le bouton

    int running = 1;
    SDL_Event event;
    Uint32 cursorStartTime = SDL_GetTicks(); // Timer pour le curseur
    int cursorVisible = 1; // État du curseur (visible ou invisible)
    // Ajouter les variables pour le fichier audio et la réponse correcte
    char directory[] = "./"; // Dossier contenant les fichiers audio
    char selectedFile[256];
    char correctAnswer[256];

// Avant de démarrer la boucle principale, sélectionner un fichier audio
    if (!getRandomAudio(directory, selectedFile, correctAnswer)) {
        printf("Erreur lors de la sélection du fichier audio.\n");
        return 1;
    }

// Réajouter l'extension .wav au fichier sélectionné pour lecture
    strcat(selectedFile, ".wav");
    while (running) {
        while (SDL_PollEvent(&event)) {
            // Gestion des événements
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Gestion des clics
                int x = event.button.x;
                int y = event.button.y;

                for (int i = 0; i < 2; i++) {
                    if (x >= fields[i].rect.x && x <= fields[i].rect.x + fields[i].rect.w &&
                        y >= fields[i].rect.y && y <= fields[i].rect.y + fields[i].rect.h) {
                        fields[i].is_active = 1;
                    } else {
                        fields[i].is_active = 0;
                    }
                }

                if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
                    y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                    buttonClicked = 1;

                    // Enregistrer le prénom et nom dans un fichier
                    FILE *file = fopen("user_data.txt", "w");
                    if (file) {
                        fprintf(file, "Prenom: %s\nNom: %s\n", fields[0].text, fields[1].text);
                        fclose(file);
                    }

                    // Lancer le jeu avec le fichier sélectionné
                    displayQuestion(renderer, font, selectedFile, correctAnswer);
                    running = 0; // Quitte la boucle principale
                }
            } else if (event.type == SDL_TEXTINPUT) {
                for (int i = 0; i < 2; i++) {
                    if (fields[i].is_active) {
                        strcat(fields[i].text, event.text.text);
                    }
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
                for (int i = 0; i < 2; i++) {
                    if (fields[i].is_active && strlen(fields[i].text) > 0) {
                        fields[i].text[strlen(fields[i].text) - 1] = '\0';
                    }
                }
            }
        }

        // Clignotement du curseur
        if (SDL_GetTicks() - cursorStartTime > 500) {
            cursorVisible = !cursorVisible;
            cursorStartTime = SDL_GetTicks();
        }

        // Fond avec dégradé rose foncé
        drawGradient(renderer, 640, 480, startColor, endColor);

        // Affichage des champs et du bouton
        for (int i = 0; i < 2; i++) {
            SDL_SetRenderDrawColor(renderer, fieldColor.r, fieldColor.g, fieldColor.b, fieldColor.a);
            SDL_RenderFillRect(renderer, &fields[i].rect);
            SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
            SDL_RenderDrawRect(renderer, &fields[i].rect);

            SDL_Rect textRect = {fields[i].rect.x + 5, fields[i].rect.y + 5, 0, 0};
            renderText(renderer, font, fields[i].text, &textRect, textColor);

            if (fields[i].is_active && cursorVisible) {
                int cursorX = textRect.x + textRect.w;
                SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
                SDL_RenderDrawLine(renderer, cursorX, fields[i].rect.y + 5, cursorX, fields[i].rect.y + fields[i].rect.h - 5);
            }
        }
        // Labels
        SDL_Rect labelRect1 = {fields[0].rect.x, fields[0].rect.y - 30, 0, 0};
        renderText(renderer, font, "Prenom:", &labelRect1, labelColor);

        SDL_Rect labelRect2 = {fields[1].rect.x, fields[1].rect.y - 30, 0, 0};
        renderText(renderer, font, "Nom:", &labelRect2, labelColor);

        SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_RenderFillRect(renderer, &buttonRect);
        SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(renderer, &buttonRect);

        SDL_Rect buttonTextRect = {buttonRect.x + 20, buttonRect.y + 10, 0, 0};
        renderText(renderer, font, "Jouer", &buttonTextRect, textColor);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
