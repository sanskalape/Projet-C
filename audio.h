#ifndef AUDIO_H
#define AUDIO_H
#include <SDL.h>
#include <SDL2/SDL_mixer.h>

// Jouer un fichier audio (fichier WAV ou MP3)
void playAudio(char *filename);

// Nettoyer SDL2_Mixer
void cleanAudio();

#endif