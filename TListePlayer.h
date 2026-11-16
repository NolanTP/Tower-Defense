#ifndef TLISTEPLAYER_H_INCLUDED
#define TLISTEPLAYER_H_INCLUDED

#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1

#include <stdbool.h>
#include "towerdefend.h"

void initTListePlayer(TListePlayer *player);
void estVide(TListePlayer player);
void afficheTListePlayer(TListePlayer *player);

#endif
