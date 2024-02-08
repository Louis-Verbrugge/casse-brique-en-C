
#include "pageApresGame.h"
#include "gameLoop.h"

void pagepageApresGame(SDL_Renderer* renderer, int HEIGHT, int WIDTH, char* testAffiche, int* ouAllerApres, int* tempEnGame) {


    // chargement font:
    TTF_Font* fontVerdana = TTF_OpenFont("police_verdana.ttf", 130);
    if (!fontVerdana) {
        printf("Erreur de chargement de la police : %s", SDL_GetError());
        exit(-1);
    }


    SDL_bool quit = false;
    SDL_Event event;

    int mouseX;
    int mouseY;

    SDL_Rect fenetre = { (WIDTH / 2)-(WIDTH - (WIDTH / 3)*2)/2, HEIGHT / 5, WIDTH - (WIDTH / 3) * 2, HEIGHT - (HEIGHT / 5) * 2 };
    SDL_Surface* imageWallPapper;
    // image font:
    if (testAffiche == "tu as gagné") {
        SDL_Surface* imageWallPapper = chargementImage("image_yoda.png");
        SDL_Texture* monImageWallPapper = SDL_CreateTextureFromSurface(renderer, imageWallPapper);  //La texture monImage contient maintenant l'image importe
        SDL_FreeSurface(imageWallPapper);
        SDL_Rect emplacementImageWallPapper = { fenetre.x, fenetre.y, fenetre.w, fenetre.h };
        SDL_RenderCopy(renderer, monImageWallPapper, NULL, &emplacementImageWallPapper);
    }
    else {
        SDL_Surface* imageWallPapper = chargementImage("image_darkVador.png");
        SDL_Texture* monImageWallPapper = SDL_CreateTextureFromSurface(renderer, imageWallPapper);  //La texture monImage contient maintenant l'image importe
        SDL_FreeSurface(imageWallPapper);
        SDL_Rect emplacementImageWallPapper = { fenetre.x, fenetre.y, fenetre.w, fenetre.h };
        SDL_RenderCopy(renderer, monImageWallPapper, NULL, &emplacementImageWallPapper);
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &fenetre);

    int espacement = 10;
    int tailleButton = fenetre.w / 3;
    //boutton retour lobby:
    SDL_Surface* imageRetourLobby = chargementImage("briqueVert.png");
    SDL_Texture* monImageRetourLobby = SDL_CreateTextureFromSurface(renderer, imageRetourLobby);  //La texture monImage contient maintenant l'image importe
    SDL_FreeSurface(imageRetourLobby);
    SDL_Rect emplacementMonImageRetourLobby = { (fenetre.x + fenetre.w / 2)-espacement-tailleButton, fenetre.y + fenetre.h - (fenetre.h / 4), tailleButton, fenetre.w/15 };
    SDL_RenderCopy(renderer, monImageRetourLobby, NULL, &emplacementMonImageRetourLobby);


    //boutton refaire une game:
    SDL_Surface* imageRetourGame = chargementImage("briqueVert.png");
    SDL_Texture* monImageRetourGame = SDL_CreateTextureFromSurface(renderer, imageRetourGame);  //La texture monImage contient maintenant l'image importe
    SDL_FreeSurface(imageRetourGame);
    SDL_Rect emplacementMonImageRetourGame = { ( fenetre.x + fenetre.w / 2 ) + espacement, fenetre.y + fenetre.h - (fenetre.h / 4), tailleButton, fenetre.w / 15 };
    SDL_RenderCopy(renderer, monImageRetourGame, NULL, &emplacementMonImageRetourGame);



    SDL_Color couleurTexte = { 255, 0, 0 };
    SDL_Surface* surfaceTexteResultat;
    surfaceTexteResultat = TTF_RenderText_Blended(fontVerdana, testAffiche, couleurTexte);
    SDL_Texture* textureTextResultat;
    textureTextResultat = SDL_CreateTextureFromSurface(renderer, surfaceTexteResultat);
    SDL_FreeSurface(surfaceTexteResultat);
    int largeurTexte = fenetre.w / 2;
    SDL_Rect emplacementTextureTextResultat = { fenetre.x + (fenetre.w / 2) - (largeurTexte / 2), fenetre.y + (fenetre.h / 3), largeurTexte, largeurTexte/4 };
    SDL_RenderCopy(renderer, textureTextResultat, NULL, &emplacementTextureTextResultat);


    SDL_Color couleurChronometre = { 255, 0, 0 };
    SDL_Surface* surfaceTexteChronometre;
    surfaceTexteChronometre = TTF_RenderText_Blended(fontVerdana, "0m 0", couleurChronometre);
    SDL_Texture* textureTextChronometre;
    textureTextChronometre = SDL_CreateTextureFromSurface(renderer, surfaceTexteChronometre);
    SDL_Rect emplacementTexteChronometre = { (fenetre.x + fenetre.w / 2) - (fenetre.w/3)/2, fenetre.y + fenetre.h/2, fenetre.w / 3, 50 };
    afficheChronometre(renderer, *tempEnGame, surfaceTexteChronometre, fontVerdana, couleurChronometre, textureTextChronometre, emplacementTexteChronometre, "tu as joué : ");


    SDL_RenderPresent(renderer);


    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *ouAllerApres = -1;
                quit = true;
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN) {

                SDL_GetMouseState(&mouseX, &mouseY);
                if (isMouseOver(mouseX, mouseY, emplacementMonImageRetourLobby)) {
                    *ouAllerApres = 0;
                    quit = true;
                }

                else if (isMouseOver(mouseX, mouseY, emplacementMonImageRetourGame)) {
                    *ouAllerApres = 2;
                    quit = true;
                }
            }


        }
    }
}

