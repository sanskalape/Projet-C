#include <stdio.h>
#include <string.h>
#include "text_field.h"
#include "graphics.h"
#include "audio.h"
#include "question.h"
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

int getRandomAudio(char *directory, char *selectedFile, char *correctAnswer) {
    struct dirent *entry;
    char files[100][256]; // Stocker jusqu'à 100 fichiers
    int count = 0;

    DIR *dir = opendir(directory);
    if (!dir) {
        printf("Erreur : Impossible d'ouvrir le répertoire %s\n", directory);
        return 0;
    }

    // Lister les fichiers audio
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".wav")) {
            strncpy(files[count], entry->d_name, 255);
            count++;
        }
    }
    closedir(dir);

    if (count == 0) {
        printf("Erreur : Aucun fichier audio trouvé dans %s\n", directory);
        return 0;
    }

    // Choisir un fichier aléatoire
    srand((unsigned int)time(NULL));
    int randomIndex = rand() % count;

    strncpy(selectedFile, files[randomIndex], 255);

    // Extraire le nom sans l'extension
    char *dot = strrchr(selectedFile, '.');
    if (dot) *dot = '\0'; // Retirer ".wav"

    strncpy(correctAnswer, selectedFile, 255);

    return 1;
}