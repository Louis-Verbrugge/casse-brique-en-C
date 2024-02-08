

#ifndef LISTECHAINE_H
#define LISTECHAINE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



struct ElementRocket* GetElementAtRocket(struct PaquetRocket* paquet, int index);

void AddElementPaquetRocket(struct PaquetRocket* paquet, struct Rocket* rocket);

int Count_PaquetRocket(struct PaquetRocket* paquet);

void RemoveRocket(struct PaquetRocket* paquet, int index);



struct ElementBrique* GetElementAtBrique(struct PaquetBrique* paquet, int index);

void AddElementPaquetBrique(struct PaquetBrique* paquet, struct Brique* brique);

int Count_PaquetBrique(struct PaquetBrique* paquet);

void RemoveBrique(struct PaquetBrique* paquet, int index);



#endif // LISTECHAINE_H

