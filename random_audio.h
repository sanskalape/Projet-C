#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include "text_field.h"
#include "graphics.h"
#include "audio.h"
#include "question.h"
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

int getRandomAudio(char *directory, char *selectedFile, char *correctAnswer);