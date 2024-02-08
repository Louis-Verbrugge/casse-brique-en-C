
#include "gameLoop.h"




// pour les Rocket:
struct ElementRocket* GetElementAtRocket(struct PaquetRocket* paquet, int index) {
    if (index < 0 || index >= Count_PaquetRocket(paquet)) {
        return NULL;
    }
    struct ElementRocket* element = paquet->firstElem;
    for (int i = 0; i < index; i++) {
        element = element->nextElem;
    }
    return element;
}


void AddElementPaquetRocket(struct PaquetRocket* paquet, struct Rocket* rocket) {
    struct ElementRocket* new_elem = malloc(sizeof(struct ElementRocket));
    new_elem->elem = rocket;
    new_elem->nextElem = NULL;

    if (paquet->firstElem == NULL) {
        paquet->firstElem = new_elem;
        return;
    }
    struct ElementRocket* e = paquet->firstElem;
    while (e->nextElem != NULL) {
        e = e->nextElem;
    }
    e->nextElem = new_elem;
}


int Count_PaquetRocket(struct PaquetRocket* paquet) {
    int nb_mechant = 1;

    struct ElementRocket* element = paquet->firstElem;
    if (element == NULL) {
        return 0;
    }

    while (element->nextElem != NULL) {
        nb_mechant++;
        element = element->nextElem;
    }
    return nb_mechant;
}
void RemoveRocket(struct PaquetRocket* paquet, int index) {

    struct elem* new_elem = GetElementAtRocket(paquet, index);

    if (0 == index)
    {
        paquet->firstElem = paquet->firstElem->nextElem;
    }
    else if (index == Count_PaquetRocket(paquet) - 1)
    {
        GetElementAtRocket(paquet, index - 1)->nextElem = NULL;
    }
    else
    {
        GetElementAtRocket(paquet, index - 1)->nextElem = GetElementAtRocket(paquet, index + 1);
    }
    free(new_elem);
}








// pour les Brique:
struct ElementBrique* GetElementAtBrique(struct PaquetBrique* paquet, int index) {
    if (index < 0 || index >= Count_PaquetBrique(paquet)) {
        return NULL;
    }
    struct ElementBrique* element = paquet->firstElem;
    for (int i = 0; i < index; i++) {
        element = element->nextElem;
    }
    return element;
}


void AddElementPaquetBrique(struct PaquetBrique* paquet, struct Brique* brique) {
    struct ElementBrique* new_elem = malloc(sizeof(struct ElementBrique));
    new_elem->elem = brique;
    new_elem->nextElem = NULL;

    if (paquet->firstElem == NULL) {
        paquet->firstElem = new_elem;
        return;
    }

    struct ElementBrique* e = paquet->firstElem;
    while (e->nextElem != NULL) {
        e = e->nextElem;
    }

    e->nextElem = new_elem;
}



int Count_PaquetBrique(struct PaquetBrique* paquet) {
    int nb_mechant = 1;

    struct ElementBrique* element = paquet->firstElem;
    if (element == NULL) {
        return 0;
    }

    while (element->nextElem != NULL) {
        nb_mechant++;
        element = element->nextElem;
    }
    return nb_mechant;
}




void RemoveBrique(struct PaquetBrique* paquetBrique, int index) {

    struct elem* new_elem = GetElementAtBrique(paquetBrique, index);

    if (0 == index)
    {
        paquetBrique->firstElem = paquetBrique->firstElem->nextElem;
    }
    else if (index == Count_PaquetBrique(paquetBrique) - 1)
    {
        GetElementAtBrique(paquetBrique, index - 1)->nextElem = NULL;
    }
    else
    {
        GetElementAtBrique(paquetBrique, index - 1)->nextElem = GetElementAtBrique(paquetBrique, index + 1);
    }
    free(new_elem);

}