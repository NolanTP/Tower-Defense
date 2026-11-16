#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "towerdefend.h"
#include "TListePlayer.h"

void initTListePlayer(TListePlayer *player)
{
    *player = NULL;
}

void estVide(TListePlayer player)
{
    if(player == NULL)
    {
        printf("Est vide\n");
    }
    else
    {
        printf("N'est pas vide\n");
    }
}

void afficheTListePlayer(TListePlayer *player)
{
    if (*player == NULL)
    {
        printf("Est vide\n");
        return;
    }

    TListePlayer ptr = *player;

    while (ptr != NULL)
    {
        printf(" %d ", ptr->pdata->nom);
        ptr = ptr->suiv;
    }
    printf("\n");
}
