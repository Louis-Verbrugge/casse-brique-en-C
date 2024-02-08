

#include "gameLoop.h"
#include "listeChaine.h"
#include "setting.h"





SDL_Surface* chargementImage(char* path) {
    SDL_Surface* image = IMG_Load(path);
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        exit(-1);
    }
    return image;
}



bool testCollisionMur(int HEIGHT, int WIDTH, struct Balle balle) {
    return (balle.sdl_Rect->x <= 0 && balle.vitesseX < 0) ||
        (balle.sdl_Rect->x + balle.sdl_Rect->w >= WIDTH && balle.vitesseX > 0);
}

bool testCollisionPlafond(int HEIGHT, int WIDTH, struct Balle balle) {
    return (balle.sdl_Rect->y <= 0 && balle.vitesseY < 0);// ||
        //balle.sdl_Rect->y + balle.sdl_Rect->h >= HEIGHT - BORDER_MAP && balle.vitesseY > 0;
}



struct Balle testCollisionRaquetteBalle(struct Balle balle, struct Raquette raquette) {

    if (SDL_HasIntersection(balle.sdl_Rect, raquette.sdl_Rect)) {

        int posXCollisionBalleCenter = balle.sdl_Rect->x + (balle.sdl_Rect->w / 2);
        double sinu;

        if ((raquette.sdl_Rect->y + balle.vitesseY) >= (balle.sdl_Rect->y + balle.sdl_Rect->h) &&
            balle.vitesseY > 0) {

            double coss;

            // si la balle tombe du cote droite de la raquette:
            if (posXCollisionBalleCenter > raquette.sdl_Rect->x + (raquette.sdl_Rect->w / 2)) {

                coss = ((posXCollisionBalleCenter - (raquette.sdl_Rect->w / 2) - raquette.sdl_Rect->x) * 100) / (raquette.sdl_Rect->w / 2);

            }
            // si la balle tombe du cote gauche de la raquette:
            else if (posXCollisionBalleCenter < raquette.sdl_Rect->x + (raquette.sdl_Rect->w / 2)) {
                coss = ((posXCollisionBalleCenter - raquette.sdl_Rect->x) * 100) / (raquette.sdl_Rect->w / 2);
                coss = -100 + coss;
            }
            else {
                // la balle est tombe au milieu 
                coss = 0;
            }

            coss = coss / 100;

            // creation zone morte, a gauche au milieu et a droite pour facilite la vie du joueur:
            if (coss < -0.8) coss = -0.8;

            else if (-0.05 <= coss && coss <= 0.05) coss = 0;

            else if (0.8 < coss) coss = 0.8;


            // je cherche son sin:
            sinu = sqrt(1 - coss * coss);

            // 4 c'est un nombre aleatoire pour la vitesse
            coss = coss * 6;
            sinu = (sinu * 6) * -1;


            balle.vitesseX = coss;
            balle.vitesseY = sinu;
        }
        else {
            // j inverse le X, car la balle a ete touche le cote de la raquette

            // si la balle est a gauche de la raquette:
            if ((balle.sdl_Rect->x < raquette.sdl_Rect->x && balle.vitesseX > 0) ||
                (balle.sdl_Rect->x + balle.sdl_Rect->w > raquette.sdl_Rect->x + raquette.sdl_Rect->w && balle.vitesseX < 0)) {

                balle.vitesseX *= -1;
            }

        }

    }
    return balle;
}



struct Balle testCollisionAvecBrique(struct PaquetBrique* paquetBrique, struct Balle balle, bool* siCreationBonus, int* posXBriqueCasse, int* posYBriqueCasse) {
    bool largeur = false;
    bool hauteur = false;
    bool collision = false;

    int decallage = 0;


    for (int i = 0; i + decallage < Count_PaquetBrique(paquetBrique); i++) {
        if (SDL_HasIntersection(GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect, balle.sdl_Rect)) {

            // test si collision a droite:
            if (balle.vitesseX > 0 && !collision) {
                if (GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect->x + balle.vitesseX > balle.sdl_Rect->x + balle.sdl_Rect->w) {
                    balle.vitesseX *= -1;
                }
                else {
                    balle.vitesseY *= -1;
                }
            }
            else if (!collision) {
                if (GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect->x + GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect->w + balle.vitesseX <
                    balle.sdl_Rect->x) {
                    balle.vitesseX * -1;
                }
                else {
                    balle.vitesseY *= -1;
                }
            }

            GetElementAtBrique(paquetBrique, i + decallage)->elem->vie -= 1;

            // je cherche le point du mieux de la brique pour pouvoir créer un bonus apres:
            *posXBriqueCasse = GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect->x + (GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect->w / 2);
            *posYBriqueCasse = GetElementAtBrique(paquetBrique, i + decallage)->elem->sdl_Rect->y;

            if (GetElementAtBrique(paquetBrique, i + decallage)->elem->vie <= 0) {
                RemoveBrique(paquetBrique, i + decallage);
                *siCreationBonus = true;
            }

            decallage--;
            collision = true;



        }
    }
    return balle;
}



void creationTireDeRocket(SDL_Renderer* renderer, struct PaquetRocket* paquetTirRocket, struct Raquette raquette) {

    int vitesseY = -15;
    double degat = 0.5;

    SDL_Surface* imageRocket = chargementImage("rocket.png");
    SDL_Texture* monImageRocket = SDL_CreateTextureFromSurface(renderer, imageRocket);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(imageRocket);


    //creation rocket gauche
    SDL_Rect* rectRocketGauche = malloc(sizeof(struct SDL_Rect));
    struct Rocket* rocketGauche = malloc(sizeof(struct Brique));
    //creation rocket droite:
    SDL_Rect* rectRocketDroite = malloc(sizeof(struct SDL_Rect));
    struct Rocket* rocketDroite = malloc(sizeof(struct Rocket));


    rectRocketGauche->x = raquette.sdl_Rect->x;
    rectRocketGauche->y = raquette.sdl_Rect->y;
    rectRocketGauche->w = 10;
    rectRocketGauche->h = 15;

    rocketGauche->sdl_Rect = rectRocketGauche;
    rocketGauche->vitesseY = vitesseY;
    rocketGauche->sdl_Rect_SpriteImage = monImageRocket;
    rocketGauche->degats = degat;



    //creation rocket droite:
    rectRocketDroite->x = raquette.sdl_Rect->x + raquette.sdl_Rect->w - 10; // 10 la largeur de la rocket
    rectRocketDroite->y = raquette.sdl_Rect->y;
    rectRocketDroite->w = 10;
    rectRocketDroite->h = 15;


    rocketDroite->sdl_Rect = rectRocketDroite;
    rocketDroite->vitesseY = vitesseY;
    rocketDroite->sdl_Rect_SpriteImage = monImageRocket;
    rocketDroite->degats = degat;


    AddElementPaquetRocket(paquetTirRocket, rocketGauche);
    AddElementPaquetRocket(paquetTirRocket, rocketDroite);



}




void creationMapBrique(SDL_Renderer* renderer, struct PaquetBrique* paquetBrique, int HEIGHT, int WIDTH, int nbBriqueLargeur, int nbBriqueHauteur) {
    int debutBriqueX = WIDTH / 8;
    int debutBriqueY = HEIGHT / 8;
    int tailleBriqueX = (WIDTH - (WIDTH / 4)) / nbBriqueLargeur;
    int tailleBriqueY = (HEIGHT / 3) / nbBriqueHauteur;

    //int tailleBriqueY = debutBriqueY / 3;


    SDL_Surface* imageBriqueVert = chargementImage("briqueVert.png");
    SDL_Texture* monImageBriqueVert = SDL_CreateTextureFromSurface(renderer, imageBriqueVert);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(imageBriqueVert);

    SDL_Surface* imageBriqueBleu = chargementImage("briqueBleu.png");
    SDL_Texture* monImageBriqueBleu = SDL_CreateTextureFromSurface(renderer, imageBriqueBleu);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(imageBriqueBleu);

    SDL_Surface* imageBriqueRouge = chargementImage("briqueRouge.png");
    SDL_Texture* monImageBriqueRouge = SDL_CreateTextureFromSurface(renderer, imageBriqueRouge);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(imageBriqueRouge);

    //3 car il y a que 3 couleur differente
    SDL_Texture* couleurBriqueNBvie[3] = { monImageBriqueVert, monImageBriqueBleu, monImageBriqueRouge };

    int vieBrique = 1;


    for (int y = 0; y < nbBriqueHauteur; y++) {
        for (int x = 0; x < nbBriqueLargeur; x++) {

            if (y >= nbBriqueHauteur / 2 && vieBrique == 1) {
                vieBrique--;
            }

            SDL_Rect* rectBrique = malloc(sizeof(struct SDL_Rect));
            rectBrique->x = debutBriqueX + tailleBriqueX * x;
            rectBrique->y = debutBriqueY + tailleBriqueY * y;
            rectBrique->w = tailleBriqueX;
            rectBrique->h = tailleBriqueY;

            struct Couleur* couleurBrique = malloc(sizeof(struct Couleur));
            couleurBrique->r = 0;
            couleurBrique->g = 255;
            couleurBrique->b = 0;

            struct Brique* brique = malloc(sizeof(struct Brique));
            brique->color = couleurBrique;
            brique->sdl_Rect = rectBrique;
            brique->vie = vieBrique;
            brique->imageBrique = couleurBriqueNBvie[vieBrique];


            AddElementPaquetBrique(paquetBrique, brique);
        }
       
    }
}

void creationBonus(SDL_Renderer* renderer, int WIDTH, struct PaquetRocket* paquetBonus, struct ChoixNiveau choixSettingSelect, char* nomBonus[], int* posXBriqueCasse, int* posYBriqueCasse, int listeBoolBonus[]) {



    int bonusAlea = rand() % 3;

    // je fais ca pour verif les bonus pour pas avoir de probleme
    while ((bonusAlea == 1 && listeBoolBonus[1]) || (bonusAlea == 2 && listeBoolBonus[2])) {
        bonusAlea = rand() % 3;

    }


    char monImage[50];
    sprintf_s(monImage, sizeof(monImage), "bonus%s.png", nomBonus[bonusAlea]);
    printf("%s\n", monImage);

    SDL_Surface* imageRocket = chargementImage(monImage);
    SDL_Texture* monImageRocket = SDL_CreateTextureFromSurface(renderer, imageRocket);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(imageRocket);



    //creation bonus
    int vitesseY = -15;


    SDL_Rect* rectBonus = malloc(sizeof(struct SDL_Rect));

    int hauteur = choixSettingSelect.tailleBonusX / 4;

    rectBonus->x = *posXBriqueCasse - choixSettingSelect.tailleBonusX / 2;
    rectBonus->y = *posYBriqueCasse;
    rectBonus->w = choixSettingSelect.tailleBonusX;
    rectBonus->h = hauteur;

    struct Rocket* bonus = malloc(sizeof(struct Brique));

    bonus->sdl_Rect = rectBonus;
    bonus->vitesseY = 5;
    bonus->sdl_Rect_SpriteImage = monImageRocket;
    bonus->nomBonus = bonusAlea;

    AddElementPaquetRocket(paquetBonus, bonus);

}

//bool
void testCollisionEntreRaquetteBonus(struct Raquette raquette, struct PaquetRocket* paquetBonus, int listeBoolBonus[], int listeTimeBonus[], Mix_Chunk* son_recuperation_bonus) {


    int decallage = 0;

    for (int i = 0; i + decallage < Count_PaquetRocket(paquetBonus, i); i++) {
        if (SDL_HasIntersection(raquette.sdl_Rect, GetElementAtRocket(paquetBonus, i + decallage)->elem->sdl_Rect)) {

            Mix_PlayChannel(-1, son_recuperation_bonus, 0);


            switch (GetElementAtRocket(paquetBonus, i + decallage)->elem->nomBonus)
            {
            case 0: // bonus rocket
                printf("lett s go rocket !");
                listeBoolBonus[0] = true;
                listeTimeBonus[0] = SDL_GetTicks();
                raquette.sdl_Rect_SpriteImage->x = raquette.sdl_Rect_SpriteImage->w; // je change l'emplacement de l'image dans le sprite
                break;

            case 1: // bonnus agrandir raquette
                raquette.sdl_Rect->w *= 1.5;
                listeBoolBonus[1] = true;
                listeTimeBonus[1] = SDL_GetTicks();
                break;

            case 2:  //  bonus retrecir raquette
                raquette.sdl_Rect->w *= 0.5;
                listeBoolBonus[2] = true;
                listeTimeBonus[2] = SDL_GetTicks();
                break;
            }


            // je supprime le bonus:
            RemoveRocket(paquetBonus, i + decallage);
            decallage--;
        }
    }
    //return false;
}

void afficheCoeurVie(SDL_Renderer* renderer, SDL_Texture* monImageCoeur, SDL_Rect emplacementCoeur, int nbVie) {
    int espacement = 10;
    for (int i = 0; i < nbVie; i++) {
        SDL_RenderCopy(renderer, monImageCoeur, NULL, &emplacementCoeur);
        emplacementCoeur.x += emplacementCoeur.w + espacement;
    }
}

void testCollisionEntreRocketBrique(struct PaquetRocket* paquetTirRocket, struct PaquetBrique* paquetBrique) {

    int decallageRocket = 0;


    for (int i = 0; i + decallageRocket < Count_PaquetRocket(paquetTirRocket); i++) {

        for (int y = 0; y < Count_PaquetBrique(paquetBrique); y++) {


            if (SDL_HasIntersection(GetElementAtRocket(paquetTirRocket, i + decallageRocket)->elem->sdl_Rect, GetElementAtBrique(paquetBrique, y)->elem->sdl_Rect)) {


                GetElementAtBrique(paquetBrique, y)->elem->vie -= GetElementAtRocket(paquetTirRocket, i + decallageRocket)->elem->degats;

                //je supprime la rocket la rocket
                RemoveRocket(paquetTirRocket, i + decallageRocket);

                decallageRocket--;

                if (GetElementAtBrique(paquetBrique, y)->elem->vie <= 0) {
                    RemoveBrique(paquetBrique, y);
                }
                break;
            }
        }
    }
}

bool TestSiFinGame(int HEIGHT, struct Balle balle) {
    // true si il la balle est en dessous de la map
    return balle.sdl_Rect->y > HEIGHT;
}

void afficheChronometre(SDL_Renderer* renderer, int tempPasse, SDL_Surface* surfaceTexteResultat, TTF_Font* fontVerdana, SDL_Color couleurTexte, SDL_Texture* textureTextResultat, SDL_Rect emplacementText, char* texteAvant) {
    char textAffiche[100]; // Assurez-vous que la taille est suffisamment grande
    char secondeChar[20];
    char minuteChar[20];
    int seconde = 0;
    int minute = 0;

    seconde = tempPasse / 1000;
    while (seconde >= 60) {
        seconde -= 60;
        minute++;
    }

    sprintf_s(minuteChar, sizeof(minuteChar), "%d", minute);
    sprintf_s(secondeChar, sizeof(secondeChar), "%d", seconde);

    //sprintf_s(secondeChar, sizeof(secondeChar), "%d", seconde);
    //strcpy(textAffiche, "Score :");
    strcpy(textAffiche, texteAvant);
    strcat(textAffiche, minuteChar);
    strcat(textAffiche, "m ");
    strcat(textAffiche, secondeChar);
    strcat(textAffiche, "s");


    surfaceTexteResultat = TTF_RenderText_Blended(fontVerdana, textAffiche, couleurTexte);
    textureTextResultat = SDL_CreateTextureFromSurface(renderer, surfaceTexteResultat);
    SDL_FreeSurface(surfaceTexteResultat);

    SDL_RenderCopy(renderer, textureTextResultat, NULL, &emplacementText);





}


void gameLoop(SDL_Renderer* renderer, int HEIGHT, int WIDTH, struct ChoixNiveau choixSettingSelect, int* ouAllerApres, int* tempEnGame, char** textAffiche) {


    // chargement font:
    TTF_Font* fontVerdana = TTF_OpenFont("police_verdana.ttf", 130);
    if (!fontVerdana) {
        printf("Erreur de chargement de la police : %s", SDL_GetError());
        exit(-1);
    }

    Mix_Chunk* sound1 = Mix_LoadWAV("son_perd_une_vie.wav");
    if (!sound1) {
        printf("Erreur lors du chargement des effets sonores : %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }
    Mix_Chunk* son_recuperation_bonus = Mix_LoadWAV("son_recuperation_bonus.wav");
    if (!son_recuperation_bonus) {
        printf("Erreur lors du chargement des effets sonores : %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    *ouAllerApres = 3;


    SDL_bool quit = false;
    SDL_Event event;

    //pour animation sprite sheet raquette
    int lastrender = SDL_GetTicks();
    int tempAnimationRaquette = 100; // 1 en seconde
    int imageSpriteSheet = 0;
    int nbVie = 3;

    //temp dernier spon rocket
    int lastRocket = SDL_GetTicks();
    int tempEntreRocket = 100; // 1 en seconde

    //pour le chronomomete:
    int chronometre = SDL_GetTicks();
    SDL_Color couleurTexte = { 255, 0, 0 };
    SDL_Surface* surfaceTexteResultat;
    surfaceTexteResultat = TTF_RenderText_Blended(fontVerdana, "0", couleurTexte);
    SDL_Texture* textureTextResultat = SDL_CreateTextureFromSurface(renderer, surfaceTexteResultat);
    SDL_Rect emplacementTextureTextResultat = { WIDTH/2-(WIDTH / 20)/2, 10,  WIDTH / 20, (WIDTH / 20)/3};


   //init coeur:
    SDL_Rect emplacementCoeur = { 10, ((WIDTH / 20) / 3)/2, (WIDTH / 20) / 3, (WIDTH / 20) / 3 };
    SDL_Surface* imageCoeur = chargementImage("image_coeur.png");
    SDL_Texture* monImageCoeur = SDL_CreateTextureFromSurface(renderer, imageCoeur);  //La texture monImage contient maintenant l'image importe
    SDL_FreeSurface(imageCoeur);


    // init raquette:
    int tailleRaquetteY = choixSettingSelect.tailleRaquetteX / 4;
    SDL_Rect possitionraquette1 = { 200, HEIGHT-HEIGHT/5, choixSettingSelect.tailleRaquetteX, tailleRaquetteY };
    struct Couleur raquetteCouleur = { 255, 0, 0 };
    SDL_Rect possitionSpriteImage = { 0, 0, 485, 128 };
    struct Raquette raquette = { &possitionraquette1, 8, &raquetteCouleur , &possitionSpriteImage };

    //init balle:
    int tailleBalle = choixSettingSelect.tailleBalle;
    SDL_Rect possitionBalle1 = { raquette.sdl_Rect->x + (raquette.sdl_Rect->w / 2) - (tailleBalle / 2), raquette.sdl_Rect->y - tailleBalle , tailleBalle, tailleBalle };
    struct Couleur balleCouleur = { 255, 255, 0 };
    struct Balle balle1 = { &possitionBalle1 , 3, -3, 3, &raquetteCouleur , false };

    //image wallPaper:
    SDL_Surface* imageWallPapper = chargementImage("wallPaper.jpg");
    SDL_Rect dest = { 0, 0, WIDTH, HEIGHT };
    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer, imageWallPapper);  //La texture monImage contient maintenant l'image importe
    SDL_FreeSurface(imageWallPapper);

    //init image balle:
    SDL_Surface* image = chargementImage("balle.png");
    SDL_Texture* monImageBalle = SDL_CreateTextureFromSurface(renderer, image);  //La texture monImage contient maintenant l'image importe
    SDL_FreeSurface(image);

    //init image raquette:
    SDL_Surface* imageRaquette = chargementImage("eclair_modif.png");
    SDL_Texture* monImageRaquette = SDL_CreateTextureFromSurface(renderer, imageRaquette);  //La texture monImage contient maintenant l'image import e
    SDL_FreeSurface(imageRaquette);


    //init struct des briques:
    struct PaquetBrique paquetBrique = { NULL };

    //init strcut des tire rocket:
    struct PaquetRocket paquetTirRocket = { NULL };

    //init strcut des bonus:
    struct PaquetRocket paquetBonus = { NULL };

    creationMapBrique(renderer, &paquetBrique, HEIGHT, WIDTH, choixSettingSelect.nbBriqueX, choixSettingSelect.nbBriqueY);


    bool moveLeft = false;
    bool moveRight = false;
    bool raquetteBouge = false;

    //bonus:
    bool* siCreationBonus = false;

    bool* bonusRocket = false;
    bool* bonusAgrandirRaquette = false;
    bool* bonusDiminuRaquette = false;
    int listeBoolBonus[] = { bonusRocket , bonusAgrandirRaquette, bonusDiminuRaquette };

    int* posXBriqueCasse;
    int* posYBriqueCasse;


    int* debutBonusRocket = SDL_GetTicks();
    int* debutBonusAgrandirRaquette = SDL_GetTicks();
    int* debutBonusDiminuRaquette = SDL_GetTicks();
    int listeTimeBonus[] = { debutBonusRocket , debutBonusAgrandirRaquette, debutBonusDiminuRaquette };


    char* nomBonus[] = { "Rocket", "AugmenteRaquette", "DinimuRaquette" };
    int nbAleatoireBonus;


    int dernierTourBoucle = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                *ouAllerApres = -1;
                break;
            }

            else if (event.type == SDL_KEYDOWN) {

                switch (event.key.keysym.sym)
                {
                case SDLK_d: // haut
                    moveRight = true;
                    break;

                case SDLK_q:
                    moveLeft = true;
                    break;

                case SDLK_SPACE:
                    balle1.balleTire = true;
                    break;

                }


            }
            else if (event.type == SDL_KEYUP) {

                switch (event.key.keysym.sym)
                {
                case SDLK_d: // haut
                    moveRight = false;
                    break;

                case SDLK_q:
                    moveLeft = false;
                    break;

                }
            }
        }

        if (SDL_GetTicks() - dernierTourBoucle > 16) {

            //animation image raquette:
            if (SDL_GetTicks() - lastrender > tempAnimationRaquette) {
                raquette.sdl_Rect_SpriteImage->y += raquette.sdl_Rect_SpriteImage->h;
                imageSpriteSheet++;
                if (imageSpriteSheet > 2) {
                    raquette.sdl_Rect_SpriteImage->y = 0;
                    imageSpriteSheet = 0;
                }

                lastrender = SDL_GetTicks();
            }


            //ceation rocket:
            if (listeBoolBonus[0]) {
                if (SDL_GetTicks() - lastRocket > tempEntreRocket) {
                    creationTireDeRocket(renderer, &paquetTirRocket, raquette);
                    lastRocket = SDL_GetTicks();
                }
            }


            //mouvement des bonus:
            for (int i = 0; i < Count_PaquetRocket(&paquetBonus); i++) {
                GetElementAtRocket(&paquetBonus, i)->elem->sdl_Rect->y += GetElementAtRocket(&paquetBonus, i)->elem->vitesseY;
            }

            //mouvement tire rocket:
            for (int i = 0; i < Count_PaquetRocket(&paquetTirRocket); i++) {
                GetElementAtRocket(&paquetTirRocket, i)->elem->sdl_Rect->y += GetElementAtRocket(&paquetTirRocket, i)->elem->vitesseY;
            }

            //mouvement de la raquette:
            if (moveLeft) {
                raquette.sdl_Rect->x -= raquette.vitesse;
                if (raquette.sdl_Rect->x < 0) {
                    raquette.sdl_Rect->x = 0;
                }
                raquetteBouge = true;

            }
            else if (moveRight) {
                raquette.sdl_Rect->x += raquette.vitesse;
                if (raquette.sdl_Rect->x + raquette.sdl_Rect->w > WIDTH) {
                    raquette.sdl_Rect->x = WIDTH - raquette.sdl_Rect->w;
                }
                raquetteBouge = true;
            }

            //mouvement de la balle:
            if (raquetteBouge && !balle1.balleTire) {
                // je test si la balle pas ete encore tire pour quelle reste collé a la raquette:
                balle1.sdl_Rect->x = raquette.sdl_Rect->x + (raquette.sdl_Rect->w / 2) - (tailleBalle / 2);
                raquetteBouge = false;
            }

            else if (balle1.balleTire) {
                //mouvement de la balle 
                balle1.sdl_Rect->x += balle1.vitesseX;
                balle1.sdl_Rect->y += balle1.vitesseY;
            }


            // test collision balle avec le mur:
            if (testCollisionMur(HEIGHT, WIDTH, balle1)) {
                balle1.vitesseX *= -1;
            }
            // test collision balle avec le plafond:
            if (testCollisionPlafond(HEIGHT, WIDTH, balle1)) {
                balle1.vitesseY *= -1;
            }


            balle1 = testCollisionAvecBrique(&paquetBrique, balle1, &siCreationBonus, &posXBriqueCasse, &posYBriqueCasse);

            if (siCreationBonus) {
                srand(time(NULL));

                nbAleatoireBonus = rand() % 2;
                if (nbAleatoireBonus == 1) {
                    creationBonus(renderer, WIDTH, &paquetBonus, choixSettingSelect, nomBonus, &posXBriqueCasse, &posYBriqueCasse, listeBoolBonus);

                }
                siCreationBonus = false;
            }

            balle1 = testCollisionRaquetteBalle(balle1, raquette);


            testCollisionEntreRaquetteBonus(raquette, &paquetBonus, listeBoolBonus, listeTimeBonus, son_recuperation_bonus);

            testCollisionEntreRocketBrique(&paquetTirRocket, &paquetBrique);


            //affiche wallPapper:
            SDL_RenderCopy(renderer, monImage, NULL, &dest);

            //afficeh chronometer:
            afficheChronometre(renderer, SDL_GetTicks()-chronometre, surfaceTexteResultat, fontVerdana, couleurTexte, textureTextResultat, emplacementTextureTextResultat, "Score : ");


            //affiche rocket:
            for (int i = 0; i < Count_PaquetRocket(&paquetTirRocket); i++) {
                SDL_RenderCopy(renderer, GetElementAtRocket(&paquetTirRocket, i)->elem->sdl_Rect_SpriteImage, NULL, GetElementAtRocket(&paquetTirRocket, i)->elem->sdl_Rect);
            }

            //affiche joueur:
            SDL_RenderCopy(renderer, monImageRaquette, raquette.sdl_Rect_SpriteImage, raquette.sdl_Rect);

            //affiche balle:
            SDL_RenderCopy(renderer, monImageBalle, NULL, balle1.sdl_Rect);


            //affiche brique:
            for (int i = 0; i < Count_PaquetBrique(&paquetBrique); i++) {
                SDL_RenderCopy(renderer, GetElementAtBrique(&paquetBrique, i)->elem->imageBrique, NULL, GetElementAtBrique(&paquetBrique, i)->elem->sdl_Rect);
            }

            //affiche Bonus:
            for (int i = 0; i < Count_PaquetRocket(&paquetBonus); i++) {
                SDL_RenderCopy(renderer, GetElementAtRocket(&paquetBonus, i)->elem->sdl_Rect_SpriteImage, NULL, GetElementAtRocket(&paquetBonus, i)->elem->sdl_Rect);
            }

            //affiche coeur:
            afficheCoeurVie(renderer, monImageCoeur, emplacementCoeur, nbVie);

            SDL_RenderPresent(renderer);


            //verif si il y a des rocket qui sont hors de la map pour les supprimer:

            for (int i = 0; i < Count_PaquetRocket(&paquetTirRocket); i++) {
                if (0 > (GetElementAtRocket(&paquetTirRocket, i)->elem->sdl_Rect->y + GetElementAtRocket(&paquetTirRocket, i)->elem->sdl_Rect->h)) {
                    RemoveRocket(&paquetTirRocket, i);
                }

            }


            //test si il faut supprimer les bonus car le temps et passé:
            for (int i = 0; i < 3; i++) {
                if (listeBoolBonus[i]) {
                    if (SDL_GetTicks() - listeTimeBonus[i] > 10000) { // 10000 = 10s

                        // je refait retrecir la raquette:
                        if (i == 0) {
                            raquette.sdl_Rect_SpriteImage->x = 0;
                        }
                        else if (i == 1) {
                            raquette.sdl_Rect->w *= 0.5;
                        }
                        else if (i == 2) {
                            raquette.sdl_Rect->w *= 1.5;
                        }

                        listeBoolBonus[i] = false;

                    }
                }
            }


            if (TestSiFinGame(HEIGHT, balle1)) {
                Mix_PlayChannel(-1, sound1, 0);
                nbVie--;
                possitionBalle1.x = raquette.sdl_Rect->x + (raquette.sdl_Rect->w / 2) - (tailleBalle / 2);
                possitionBalle1.y = raquette.sdl_Rect->y - tailleBalle;
                balle1.balleTire = false;
                balle1.vitesseX = 3;
                balle1.vitesseY = 3;
                if (nbVie <= 0) {
                    quit = true;
                    *ouAllerApres = 0;
                }
            }
            if (Count_PaquetBrique(&paquetBrique) == 0) {
                quit = true;
            }


            dernierTourBoucle = SDL_GetTicks();
        }

    }
    if (nbVie <= 0) {
        *textAffiche = "tu as perdu";
    }
    else {
        *textAffiche = "tu as gagné";
    }


    *tempEnGame = SDL_GetTicks() - chronometre;
}

