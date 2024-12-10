#include "audio.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL2/SDL_mixer.h>

void playAudio(char *filename) {
    // Initialiser SDL2_Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur d'initialisation SDL_Mixer : %s\n", Mix_GetError());
        return;
    }

    // Charger le fichier audio
    Mix_Chunk *sound = Mix_LoadWAV(filename);
    if (!sound) {
        printf("Erreur de chargement du fichier audio '%s' : %s\n", filename, Mix_GetError());
        Mix_CloseAudio();
        return;
    }

    // Jouer le fichier audio
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        printf("Erreur de lecture audio : %s\n", Mix_GetError());
    } else {
        printf("Audio lancé avec succès : %s\n", filename);
    }

    // Ne pas bloquer l'exécution ici, on laisse l'audio jouer en arrière-plan
}


void cleanAudio() {
    // Fermer SDL2_Mixer
    Mix_CloseAudio();
}
