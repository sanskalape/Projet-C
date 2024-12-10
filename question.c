#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // For getcwd
#include "text_field.h"
#include "graphics.h"
#include "audio.h"
#include <ctype.h>

// Convertit une chaîne en minuscules
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void displayQuestion(SDL_Renderer *renderer, TTF_Font *font, char *filename, char *correctAnswer) {
    SDL_Color questionColor = {255, 255, 255, 255}; // Blanc pour la question
    SDL_Color textColor = {0, 0, 0, 255};          // Noir pour le texte et le curseur
    SDL_Color startColor = {139, 0, 139, 255};     // Dégradé rose foncé (début)
    SDL_Color endColor = {255, 20, 147, 255};      // Dégradé rose foncé (fin)
    SDL_Color fieldColor = {255, 255, 255, 255};   // Blanc pour l'intérieur du champ
    SDL_Color borderColor = {211, 211, 211, 255};  // Gris clair pour les contours

    TextField responseField = {"", {200, 300, 240, 40}, 1}; // Champ de réponse actif
    int running = 1;
    int isCorrect = 0; // Flag pour vérifier si la réponse est correcte
    SDL_Event event;
    Uint32 cursorStartTime = SDL_GetTicks(); // Timer pour le curseur
    int cursorVisible = 1; // État du curseur (visible ou invisible)

    // Print the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    // Lire la question depuis le fichier
    FILE *file = fopen("questions.txt", "r");
    if (!file) {
        printf("Erreur d'ouverture du fichier questions.txt\n");
        return;
    }

    char question[256];
    if (fgets(question, sizeof(question), file) == NULL) {
        printf("Erreur de lecture de la question\n");
        fclose(file);
        return;
    }
    fclose(file);

    // Activer la saisie de texte
    SDL_StartTextInput();
    playAudio(filename); // Jouer la musique au lancement

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if (x >= responseField.rect.x && x <= responseField.rect.x + responseField.rect.w &&
                    y >= responseField.rect.y && y <= responseField.rect.y + responseField.rect.h) {
                    responseField.is_active = 1;
                } else {
                    responseField.is_active = 0;
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (responseField.is_active && strlen(responseField.text) < MAX_TEXT_LENGTH - 1) {
                    strcat(responseField.text, event.text.text);
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (responseField.is_active && event.key.keysym.sym == SDLK_BACKSPACE) {
                    size_t len = strlen(responseField.text);
                    if (len > 0) {
                        responseField.text[len - 1] = '\0';
                    }
                } else if (responseField.is_active && event.key.keysym.sym == SDLK_RETURN) {
                    // Vérifier si la réponse est correcte (insensible à la casse)
                    char userAnswer[MAX_TEXT_LENGTH];
                    char correct[MAX_TEXT_LENGTH];

                    strcpy(userAnswer, responseField.text);
                    strcpy(correct, correctAnswer);
                    toLowerCase(userAnswer);
                    toLowerCase(correct);

                    if (strcmp(userAnswer, correct) == 0) {
                        isCorrect = 1; // Réponse correcte
                        cleanAudio();
                    } else {
                        isCorrect = -1; // Réponse incorrecte
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

        // Afficher la question
        SDL_Rect questionRect;
        TTF_SizeText(font, question, &questionRect.w, &questionRect.h);
        questionRect.x = (640 - questionRect.w) / 2;
        questionRect.y = 100;
        renderText(renderer, font, question, &questionRect, questionColor);

        // Afficher le champ de réponse
        SDL_SetRenderDrawColor(renderer, fieldColor.r, fieldColor.g, fieldColor.b, fieldColor.a);
        SDL_RenderFillRect(renderer, &responseField.rect);
        SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(renderer, &responseField.rect);

        // Afficher le texte dans le champ de réponse
        SDL_Rect responseTextRect = {responseField.rect.x + 5, responseField.rect.y + 5, 0, 0};
        renderText(renderer, font, responseField.text, &responseTextRect, textColor);

        // Dessiner le curseur clignotant si le champ est actif
        if (responseField.is_active && cursorVisible) {
            int cursorX = responseTextRect.x + responseTextRect.w;
            SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_RenderDrawLine(renderer, cursorX, responseField.rect.y + 5, cursorX, responseField.rect.y + responseField.rect.h - 5);
        }

        // Afficher le message de validation
        if (isCorrect == 1) {
            SDL_Rect correctRect;
            TTF_SizeText(font, "Correct !", &correctRect.w, &correctRect.h);
            correctRect.x = (640 - correctRect.w) / 2;
            correctRect.y = 400;
            renderText(renderer, font, "Correct !", &correctRect, textColor);
        } else if (isCorrect == -1) {
            SDL_Rect incorrectRect;
            TTF_SizeText(font, "Incorrect...", &incorrectRect.w, &incorrectRect.h);
            incorrectRect.x = (640 - incorrectRect.w) / 2;
            incorrectRect.y = 400;
            renderText(renderer, font, "Incorrect...", &incorrectRect, textColor);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}