

#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"



bool isMouseOver(int mouseX, int mouseY, SDL_Rect destText);
void gameLobby(SDL_Renderer* renderer, int WIDTH, int HEIGHT, int* ouAllerApres);

#endif // GAMELOBBY_H
