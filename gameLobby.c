

#include "gameLobby.h"


bool isMouseOver(int mouseX, int mouseY, SDL_Rect destText) {
    return (destText.x <= mouseX && mouseX <= destText.x + destText.w) && (destText.y <= mouseY && mouseY <= destText.y + destText.h);
}

void gameLobby(SDL_Renderer* renderer, int WIDTH, int HEIGHT, int *ouAllerApres) {

    SDL_bool quit = false;
    SDL_Event event;

    //chargement image wallPaper:
    SDL_Surface* image = IMG_Load("wallPaper.jpg");
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        exit(-1);
    }

    SDL_Rect dest = { 0, 0, HEIGHT, WIDTH };
    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer, image);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(image);

    // chargement font:
    TTF_Font* font = TTF_OpenFont("policeEcriture.TTF", 130);
    if (!font) {
        printf("Erreur de chargement de la police : %s", SDL_GetError());
        exit(-1);
    }

    // texte "PLAY":
    SDL_Color colorText = { 255, 0, 0, 255 };
    SDL_Rect destText = { 40, 50, 150, 50 };
    SDL_Surface* surface;
    SDL_Texture* textureText;



    // texte "SETTING":
    SDL_Color colorTextSetting = { 255, 0, 0, 255 };
    SDL_Rect destTextSetting = { 40, 170, 180, 50 };
    SDL_Surface* surfaceSetting;
    SDL_Texture* textureTextSetting;



    int mouseX;
    int mouseY;

    bool surBoutonPlay = false;
    bool surBoutonSetting = false;

    bool modification = true; // true pour affiche le fenetre la prememiere fois

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *ouAllerApres = -1; // ferme le jeu
                quit = true;
                break;
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (surBoutonPlay) {
                    printf("play\n");
                    *ouAllerApres = 2;
                    quit = true;
                    break;
                }
                else if (surBoutonSetting) {
                    printf("Setting\n");
                    *ouAllerApres = 1;
                    quit = true;
                    break;
                }
            }
        }

        if (modification) {

            surface = TTF_RenderText_Blended(font, "PLAY", colorText);
            textureText = SDL_CreateTextureFromSurface(renderer, surface);
            //SDL_FreeSurface(image);
            SDL_FreeSurface(surface);



            surfaceSetting = TTF_RenderText_Blended(font, "SETTING", colorTextSetting);
            textureTextSetting = SDL_CreateTextureFromSurface(renderer, surfaceSetting);
            //SDL_FreeSurface(image);
            SDL_FreeSurface(surfaceSetting);



            modification = false;
            // mise a jour de la fenetre:
            SDL_RenderCopy(renderer, monImage, NULL, &dest);
            SDL_RenderCopy(renderer, textureText, NULL, &destText);
            SDL_RenderCopy(renderer, textureTextSetting, NULL, &destTextSetting);


            SDL_RenderPresent(renderer);
        }

        //test pour voir si on passe sur le texte:
        SDL_GetMouseState(&mouseX, &mouseY);
        if (isMouseOver(mouseX, mouseY, destText)) {

            colorText.g = 255;
            colorText.b = 255;
            surBoutonPlay = true;
            modification = true;
        }
        else if (surBoutonPlay) {

            colorText.g = 0;
            colorText.b = 0;
            surBoutonPlay = false;
            modification = true;

        }
        else if (isMouseOver(mouseX, mouseY, destTextSetting)) {
            colorTextSetting.g = 255;
            colorTextSetting.b = 255;
            surBoutonSetting = true;
            modification = true;

        }
        else if (surBoutonSetting) {
            colorTextSetting.g = 0;
            colorTextSetting.b = 0;
            surBoutonSetting = false;
            modification = true;
        }

        SDL_Delay(16);  // pour avoir 60FPS 
    }

    SDL_FreeFormat(font);
}
