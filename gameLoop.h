
#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <math.h>

struct Couleur {
    int r;
    int g;
    int b;

};


//tout ce qui est du joueur:
struct Raquette {
    struct SDL_Rect* sdl_Rect;
    int vitesse;
    struct Couleur* couleur;
    SDL_Rect* sdl_Rect_SpriteImage;
};

struct Balle {
    struct SDL_Rect* sdl_Rect;
    double vitesseX;
    double vitesseY;
    double vitesseInit;
    struct Couleur* couleur; // je pense on peut supprimer cette ligne
    bool balleTire; // boolean true si la balla a ete tire par le joueur sinon false
};

struct Rocket {
    struct SDL_Rect* sdl_Rect;
    double vitesseY;
    SDL_Rect* sdl_Rect_SpriteImage;
    double degats;
    int nomBonus; // c'est que utilisé dans les Bonus et non les rockets
};

// tout ce que est brique
struct Brique {
    struct SDL_Rect* sdl_Rect;
    struct Couleur* color;
    double vie;
    SDL_Texture* imageBrique;
};


struct ElementBrique {
    struct Brique* elem;
    struct ElementBrique* nextElem;
};


struct ElementRocket {
    struct Rocket* elem;
    struct ElementRocket* nextElem;
};


struct PaquetBrique {
    struct ElementBrique* firstElem;
};

struct PaquetRocket {
    struct ElementRocket* firstElem;
};

void afficheChronometre(SDL_Renderer* renderer, int tempPasse, SDL_Surface* surfaceTexteResultat, TTF_Font* fontVerdana, SDL_Color couleurTexte, SDL_Texture* textureTextResultat, SDL_Rect emplacementText, char* texteAvant);
void gameLoop(SDL_Renderer* renderer, int HEIGHT, int WIDTH, struct ChoixNiveau choixSettingSelect, int* ouAllerApres, int* tempEnGame, char* textAffiche);
SDL_Surface* chargementImage(char* path);

#endif // GAMELOOP_H