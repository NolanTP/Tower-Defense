#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED

#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1

#define SORTIE1 "sortie.txt"
#define SORTIE2 "sortie.bin"

#include <stdbool.h>
#include "towerdefend.h"


void saveGameBin(TListePlayer player, TListePlayer horde, int** tabParcours);
TplateauJeu loadGameBin(TListePlayer *player, TListePlayer *horde, int*** tabParcours);

void saveGameTxt(TListePlayer player, TListePlayer horde, int** tabParcours);
TplateauJeu loadGameTxt(TListePlayer *player, TListePlayer *horde, int*** tabParcours);

#endif
