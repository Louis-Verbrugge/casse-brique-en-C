



#ifndef SETTING_H
#define SETTING_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>

struct ChoixNiveau {
    char choixDuNiveau[10];
    int nbBriqueX;
    int nbBriqueY;
    int tailleRaquetteX;
    int tailleBonusX;
    int tailleBalle;

};

TTF_Font* chargementFont(char* path, int size);
SDL_Surface* chargementImage(char* path);
void gameSetting(SDL_Renderer* renderer, int WIDTH, int HEIGHT, struct ChoixNiveau* choixSettingSelect, int* ouAllerApres);

#endif // SETTING_H
