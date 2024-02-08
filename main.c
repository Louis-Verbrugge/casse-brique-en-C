
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL_mixer.h>

#include "listeChaine.h"
#include "gameLobby.h"
#include "gameLoop.h"
#include "setting.h"
#include "pageApresGame.h"

    


int main(int ac, char** av) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    //Init SDL:
    if (0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }

    // Initialiser SDL_mixer:
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_mixer : %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

   //TEST:
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);


    // Charger la musique
    Mix_Music* musique = Mix_LoadMUS("musique.mp3");
    if (!musique) {
        fprintf(stderr, "Erreur lors du chargement de la musique : %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    //chargement bruit bonus:
    Mix_Chunk* sound1 = Mix_LoadWAV("son_perd_une_vie.wav");

    if (!sound1) {
        printf("Erreur lors du chargement des effets sonores : %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    Mix_PlayMusic(musique, -1);

    SDL_DisplayMode current;
    if (SDL_GetCurrentDisplayMode(0, &current) != 0) {
        fprintf(stderr, "Erreur lors de la récupération de la résolution de l'écran : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }


    int HEIGHT = current.h;
    int WIDTH = current.w;

    window = SDL_CreateWindow("Ma Fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur lors de l'initialisation de SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;

    }

    if (NULL == window) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (NULL == renderer) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
    }


    int tempEnGame;
    char* textAffiche = "";
    int sonMusic = 128;
    int sonEffect = 128;

    struct ChoixNiveau choixSettingSelect = { "moyen", 12, 7, 90, 80, 30 };


    int ouAllerApres = 0; //    -1 == leave    ||    0 == lobby    ||    1 == setting    ||    2 == gameLoop    ||    3 == pageApresGame    ||



    while (ouAllerApres != -1) {

        switch (ouAllerApres) {

        case 0:
            gameLobby(renderer, HEIGHT, WIDTH, &ouAllerApres);
            break;

        case 1:
            gameSetting(renderer, WIDTH, HEIGHT, &choixSettingSelect, &ouAllerApres, &sonMusic, &sonEffect);
            break;

        case 2:
            gameLoop(renderer, HEIGHT, WIDTH, choixSettingSelect, &ouAllerApres, &tempEnGame, &textAffiche);
            //ouAllerApres = 3;
            break;

        case 3:
            pagepageApresGame(renderer, HEIGHT, WIDTH, textAffiche, &ouAllerApres, &tempEnGame);
            break;
        }
    }

    printf("hello");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}

