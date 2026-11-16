#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "towerdefend.h"
#include "TListePlayer.h"
#include "Save.h"

void saveGameBin(TListePlayer player, TListePlayer horde, int** tabParcours)
{
    // On ouvre le fichier en (write binary)
    FILE* f = fopen(SORTIE2, "wb");
    if (!f)
    {
        return;
    }
    //on sauvegarde en tout premier dans le fichier le nombre d'unite dans le player puis le nombre dans la horde
    int nbP,nbH;
    nbH = countUnite(horde);
    nbP = countUnite(player);

    fwrite(&nbP, sizeof(int), 1, f);
    fwrite(&nbH, sizeof(int), 1, f);

    // On parcours la liste Player
    TListePlayer ptrP = player;
    while(ptrP != NULL)
    {
        fwrite(ptrP->pdata, sizeof(Tunite), 1, f);
        ptrP = ptrP->suiv;
    }

    // On parcours la liste Horde
    TListePlayer ptrH = horde;
    while(ptrH != NULL)
    {
        fwrite(ptrH->pdata, sizeof(Tunite), 1, f);
        ptrH = ptrH->suiv;
    }

    // Sauvegarde du chemin
    for (int i = 0; i < NBCOORDPARCOURS; i++)
    {
        for(int j = 0; j<2; j++)
        {
            fwrite(&tabParcours[i][j], sizeof(int), 1, f);
        }
    }

    fclose(f);
}

TplateauJeu loadGameBin(TListePlayer *player, TListePlayer *horde, int*** tabParcours)
{
    // On ouvre le fichier en (read binary)
    FILE* f = fopen(SORTIE2, "rb");
    if (!f)
    {
        return NULL;
    }

    // On lit la taille des listes Player et Horde
    int nbP, nbH;
    fread(&nbP, sizeof(int), 1, f);
    fread(&nbH, sizeof(int), 1, f);



    // On récupère la liste player
    initTListePlayer(player);
    for(int i=0; i<nbP; i++)
    {
        Tunite* unite = malloc(sizeof(Tunite));
        fread(unite, sizeof(Tunite), 1, f);
        AjouterUnite(player, unite);
    }

    // On récupère la liste horde
    initTListePlayer(horde);
    for(int i=0; i<nbH; i++)
    {
        Tunite* unite = malloc(sizeof(Tunite));
        fread(unite, sizeof(Tunite), 1, f);
        AjouterUnite(horde, unite);
    }

    // Allocation du chemin
    freeChemin(*tabParcours);
    *tabParcours = initChemin();

    // Récupération du chemin
    for (int i = 0; i <= NBCOORDPARCOURS; i++)
    {
        for(int j = 0; j<2; j++)
        {
            fread(&(*tabParcours)[i][j], sizeof(int),1, f);
        }
    }

    fclose(f);

    // Création du plateau de jeu
    TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
    PositionnePlayerOnPlateau(*player, jeu);
    PositionnePlayerOnPlateau(*horde, jeu);


    return jeu;
}

void saveGameTxt(TListePlayer player, TListePlayer horde, int** tabParcours)
{
    FILE* f = fopen(SORTIE1, "w");
    if (!f)
    {
        return ;
    }

    // On save les tailles des listes
    fprintf(f, "%d %d\n", countUnite(player), countUnite(horde));

    // On récupère la liste player
    TListePlayer ptrP = player;
    while(ptrP != NULL)
    {
        fprintf(f, "%d %d %d %d %f %d %d %f %d %d %d\n",
                    ptrP->pdata->nom,
                    ptrP->pdata->cibleAttaquable,
                    ptrP->pdata->maposition,
                    ptrP->pdata->pointsDeVie,
                    ptrP->pdata->vitesseAttaque,
                    ptrP->pdata->degats,
                    ptrP->pdata->portee,
                    ptrP->pdata->vitessedeplacement,
                    ptrP->pdata->posX,
                    ptrP->pdata->posY,
                    ptrP->pdata->peutAttaquer);
        ptrP = ptrP->suiv;
    }

    // On récupère la liste horde
    TListePlayer ptrH = horde;
    while(ptrH != NULL)
    {
        fprintf(f, "%d %d %d %d %f %d %d %f %d %d %d\n",
                    ptrH->pdata->nom,
                    ptrH->pdata->cibleAttaquable,
                    ptrH->pdata->maposition,
                    ptrH->pdata->pointsDeVie,
                    ptrH->pdata->vitesseAttaque,
                    ptrH->pdata->degats,
                    ptrH->pdata->portee,
                    ptrH->pdata->vitessedeplacement,
                    ptrH->pdata->posX,
                    ptrH->pdata->posY,
                    ptrH->pdata->peutAttaquer);
        ptrH = ptrH->suiv;
    }

    // chemin
    for (int i = 0; i < NBCOORDPARCOURS; i++) {
        for (int j = 0; j < 2; j++) {
            fprintf(f, "%d\n", tabParcours[i][j]);
        }
    }
    fclose(f);
}

TplateauJeu loadGameTxt(TListePlayer *player, TListePlayer *horde, int*** tabParcours)
{
    FILE* f = fopen(SORTIE1, "r");
    if (!f)
    {
        return NULL;
    }
    printf("entre");
    // On save les tailles des listes
    int nbP = 0, nbH = 0;
    fscanf(f, "%d %d\n", &nbP, &nbH);

    // On récupère la liste player
    initTListePlayer(player);
    for(int i=0; i<nbP; i++)
    {
        Tunite* unite = malloc(sizeof(Tunite));
        fscanf(f, "%d %d %d %d %f %d %d %f %d %d %d\n",
                    &unite->nom,
                    &unite->cibleAttaquable,
                    &unite->maposition,
                    &unite->pointsDeVie,
                    &unite->vitesseAttaque,
                    &unite->degats,
                    &unite->portee,
                    &unite->vitessedeplacement,
                    &unite->posX,
                    &unite->posY,
                    &unite->peutAttaquer);
        AjouterUnite(player, unite);
    }
    // On récupère la liste horde
    initTListePlayer(horde);
    for(int i=0; i<nbH; i++)
    {
        Tunite* unite = malloc(sizeof(Tunite));
        fscanf(f, "%d %d %d %d %f %d %d %f %d %d %d\n",
                    &unite->nom,
                    &unite->cibleAttaquable,
                    &unite->maposition,
                    &unite->pointsDeVie,
                    &unite->vitesseAttaque,
                    &unite->degats,
                    &unite->portee,
                    &unite->vitessedeplacement,
                    &unite->posX,
                    &unite->posY,
                    &unite->peutAttaquer);

        AjouterUnite(horde, unite);
    }
    // Allocation du chemin
    freeChemin(*tabParcours);
    *tabParcours = initChemin();

    // chemin
    for (int i = 0; i < NBCOORDPARCOURS; i++) {
        for (int j = 0; j < 2; j++) {
            fscanf(f, "%d\n", &(*tabParcours)[i][j]);
        }
    }
        printf("sort du 3eme for");

    fclose(f);

    // Création du plateau de jeu
    TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
    printf("1\n");
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
    printf("2\n");
    PositionnePlayerOnPlateau(*player, jeu);
    printf("3\n");
    PositionnePlayerOnPlateau(*horde, jeu);
    printf("4\n");
    return jeu;
}
