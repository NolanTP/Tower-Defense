#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "towerdefend.h"
#include "TListePlayer.h"
#include "maSDL.h"


//typedef Tunite* ** TplateauJeu;

TplateauJeu AlloueTab2D(int largeur, int hauteur){
    TplateauJeu jeu;
    jeu = (Tunite***)malloc(sizeof(Tunite**)*largeur);
    for (int i=0;i<largeur;i++){
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*)*hauteur);
    }
    return jeu;  //tab2D contenant des pointeurs
}

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur){
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }
}


/*
void ecritCheminVersleHaut  : permet d'initilaiser le tab chemin de distance cases (avec des coord x y) dans une direction, à partir d'un point x y donné

int **chemin  : tab de coordonnées x y du chemin
int *ichemin  : indice de la case du chemin d'où on part
int *xdepart, int *ydepart : valeur en x y de départ pouri la premiere case
int distance  : distance sur laquelle on va écrire des coordonnées dans le tab chemin
int *distanceMaxRestante : securité pour ne pas sortir de la plage d'indice de chemin
*/
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int y;
        for (y=*ydepart;y>*ydepart-distance;y--){
            chemin[*ichemin][X]= *xdepart;
            chemin[*ichemin][Y]= y;
            (*ichemin)++;
        }
        *ydepart=y;
    }
    else printf("erreur longueur chemin\n");
}

void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x<*xdepart+distance;x++){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}

void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x>*xdepart-distance;x--){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}

int **initChemin(){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }
    /*
    int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;

    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 5, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    */
    return chemin;  //tab2D contenant des pointeurs
}

void afficheCoordonneesParcours(int **chemin, int nbcoord){
    printf("Liste coordonnees: ");
    for (int i=0; i<nbcoord; i++){
        printf("(%d, %d)",chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonnees\n");
}

void freeChemin(int **tab){
    for (int j=0;j<NBCOORDPARCOURS;j++){
        free(tab[j]);  //libere chaque case, qui est un tableau de 2 cases
    }
    free(tab);
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[7]={"s", "a", "r", "A", "C", "D", "G"};

    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
                // A ne pas donner aux etudiants
            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(".");  //cad pas d'unité sur cette case
        }
        printf("\n");
    }
}

Tunite *creeTourSol(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 30;
    nouv->portee = 5;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

Tunite *creeTourAir(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 25;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 0;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

Tunite *creeDragon(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 220;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 70;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

Tunite *creeArcher(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

Tunite *creeGargouille(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 100;
    nouv->vitesseAttaque = 0.6;
    nouv->degats = 90;
    nouv->portee = 1;
    nouv->vitessedeplacement = 3;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

Tunite *creeChevalier(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 400;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 250;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

void avancerUnites(TplateauJeu jeu, int** chemin)
{
    // On parcours de haut en bas
    for(int i=NBCOORDPARCOURS-2; i>=0; i--) // -2 car il n'y a pas de case avant la premiere
    {
        int x = chemin[i][X];
        int y = chemin[i][Y];

        if(jeu[x][y] != NULL)
        {
            Tunite* unite = jeu[x][y];
            jeu[x][y] = NULL;
            int v = unite->vitessedeplacement;

            for(int deplacement=v; deplacement>=0; deplacement--)
            {
                if((i + deplacement <= NBCOORDPARCOURS-1) && (jeu[chemin[i + deplacement][X]][chemin[i + deplacement][Y]] == NULL))
                {
                    x = chemin[i + deplacement][X];
                    y = chemin[i + deplacement][Y];

                    unite->posX = x;
                    unite->posY = y;

                    jeu[x][y] = unite;
                    break;
                }
                if (deplacement == 0)
                {
                    jeu[x][y] = unite;
                }
            }
        }
    }
}

void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite)
{
    // Pointeur de la liste chaînée player
    TListePlayer ptr = *player;

    // Créer la cellule
    TListePlayer cell = (TListePlayer)malloc(sizeof(struct T_cell));
    cell->pdata = nouvelleUnite;
    cell->suiv = NULL;

    // Si la liste est vide, alors on ajoute simplement l'unité
    if(*player == NULL)
    {
        *player = cell;
        return;
    }

    // Déplacer le pointeur vers la dernière case
    while(ptr->suiv != NULL)
    {
        ptr = ptr->suiv;
    }

    // Ajout de l'unité
    ptr->suiv = cell;
}

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite)
{
    TListePlayer ptr = *player;

    // 1er cas : La liste est vide
    if(*player == NULL)
    {
        return;
    }

    //2eme cas : l'unité est la première
    if((ptr->pdata->posX == UniteDetruite->posX) && (ptr->pdata->posY == UniteDetruite->posY))
    {
        *player = ptr->suiv;
        free(ptr->pdata);
        free(ptr);
        ptr = NULL;
        return;
    }

    //3eme cas : l'unité se trouve au milieu de la liste
    while (ptr != NULL
        && ptr->suiv != NULL
        && (ptr->suiv->pdata->posX != UniteDetruite->posX
         || ptr->suiv->pdata->posY != UniteDetruite->posY))
    {
        ptr = ptr->suiv;
    }
    //4eme cas : l'unité se trouve à la fin
    if (ptr->suiv->suiv == NULL)
    {
        free(ptr->suiv->pdata);
        free(ptr->suiv);
        ptr->suiv = NULL;
        return;
    }
    TListePlayer suiv = ptr->suiv->suiv; // La cellule suivant celle qu'on veut détruire
    free(ptr->suiv->pdata);
    free(ptr->suiv);
    ptr->suiv = suiv;
}


void supprimeList(TListePlayer *player, Tunite *UniteSuprr)
{
    TListePlayer ptr = *player;
    // 1er cas : La liste est vide
    if(*player == NULL)
    {
        return;
    }

    //2eme cas : l'unité est la première
    if((ptr->pdata->posX == UniteSuprr->posX) && (ptr->pdata->posY == UniteSuprr->posY))
    {
        *player = ptr->suiv;
        free(ptr);
        ptr = NULL;
        return;
    }

    //3eme cas : l'unité se trouve au milieu de la liste
    while (ptr != NULL
        && ptr->suiv != NULL
        && (ptr->suiv->pdata->posX != UniteSuprr->posX
         || ptr->suiv->pdata->posY != UniteSuprr->posY))
    {
        ptr = ptr->suiv;
    }
    TListePlayer suprr = ptr->suiv;
    ptr->suiv = suprr->suiv;
    free(suprr);
    suprr = NULL;
}

void enleveEquipe(TListePlayer *aPortee ,Tunite *UniteAttaquante)
{
    TListePlayer ptr = *aPortee;
    if (UniteAttaquante ->nom == archer || UniteAttaquante ->nom == chevalier || UniteAttaquante ->nom == dragon || UniteAttaquante ->nom == gargouille)
    {
        while(ptr != NULL)
        {
            TListePlayer prochain = ptr->suiv; // car on free dans suprimelist donc il ne trouve plus son suiv
            if(ptr->pdata->nom != tourRoi)
            {
                supprimeList(aPortee,ptr->pdata);
            }
            ptr = prochain;
        }
    }
    else
    {
        while(ptr != NULL)
        {
            TListePlayer prochain = ptr->suiv;
            if((ptr->pdata)->nom == tourRoi || (ptr->pdata)->nom == tourAir || (ptr->pdata)->nom == tourSol)
            {
                supprimeList(aPortee,ptr->pdata);
            }
            ptr = prochain;
        }
    }
}

void enlevePos(TListePlayer *aPortee ,Tunite *UniteAttaquante)
{
    TListePlayer ptr = *aPortee;
    Tcible attaque = UniteAttaquante ->cibleAttaquable;
    if (attaque == solEtAir)
    {
        return;
    }
    while(ptr != NULL)
    {
        TListePlayer prochain = ptr->suiv;
        if((ptr->pdata)->maposition != attaque)
        {
            supprimeList(aPortee,ptr->pdata);
        }
        ptr = prochain;
    }
}

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    TListePlayer lstPortee;
    initTListePlayer(&lstPortee);
    int porte = UniteAttaquante ->portee;
    int posx = UniteAttaquante->posX;
    int posy = UniteAttaquante->posY;
    for(int i = posy-porte ; i<= posy+porte ; i++)
    {
        for(int j = posx-porte; j<= posx+porte; j++)
        {
            if((i >=0 && i < HAUTEURJEU) && (j >= 0 && j < LARGEURJEU))
            {
                if(jeu[j][i] != NULL)
                {
                    AjouterUnite(&lstPortee, jeu[j][i]);
                }
            }
        }
    }
    enleveEquipe(&lstPortee,UniteAttaquante);
    enlevePos(&lstPortee,UniteAttaquante);

    return lstPortee;
}

void triTListPlayer(TListePlayer l)
{
    TListePlayer en_cours, plus_petit, j;  //pointeurs sur cellule donc T_liste
    int a = 0;
    //nous ne faisons ici que des branchements et déplacements de pointeurs
    for (en_cours = l; en_cours != NULL; en_cours = en_cours->suiv) //avec "en_cours != fin", nous nous arrêtons bien sur l'avant dernière cellule
    {
        a = a+1;
        plus_petit = en_cours;
        for (j = en_cours ; j != NULL; j=j->suiv)
        {
            if (j->pdata->pointsDeVie < plus_petit->pdata->pointsDeVie )  //comparaison des contenus pointées (pas des pointeurs directement!)
                plus_petit = j;  //branchement de "plus_petit" sur la cellule ayant une valeur pointée par data plus petite que celle pointée par plus_petit
        }
        //comparaison avec la cellule fin non comparée dans le FOR ci-dessus
        Tunite* tmp = en_cours->pdata;
        en_cours->pdata = plus_petit->pdata;
        plus_petit->pdata = tmp;
    }
}

void combat (TListePlayer *player, TListePlayer *horde , TplateauJeu *jeu , SDL_Surface* surface)
{
    TListePlayer ordreAttaquants = ordreAttaque(player, horde);
    while(ordreAttaquants != NULL)
    {
        if(ordreAttaquants->pdata == NULL)
        {
            ordreAttaquants = ordreAttaquants->suiv;
        }
        else
        {
            // Récupère l'ennemi avec le moins d'HP autour de soit : La première de la liste
            TListePlayer aPortee = quiEstAPortee(*jeu, ordreAttaquants->pdata);
            // Commence l'affrontement
            if(aPortee != NULL)
            {
                triTListPlayer(aPortee);

                dessineAttaque(surface, ordreAttaquants->pdata, aPortee->pdata);
                SDL_Delay(50);

                // Inflige des dégats à la cible
                if(aPortee->pdata->pointsDeVie - ordreAttaquants->pdata->degats <= 0)
                {
                    aPortee->pdata->pointsDeVie = 0;
                }
                else
                {
                    aPortee->pdata->pointsDeVie =  aPortee->pdata->pointsDeVie - ordreAttaquants->pdata->degats;
                }

                // Tue l'unité cible si elle se retrouve à 0 HP
                if(aPortee->pdata->pointsDeVie == 0)
                {
                    Tunite* morte = aPortee->pdata;
                    // Supprime l'unité des attaquants
                    (*jeu)[morte->posX][morte->posY] = NULL;
                    // Cherche à quel camp il appartient pour le supprimer de la liste
                    if (morte->nom == archer || morte->nom == chevalier || morte->nom ==dragon || morte->nom ==gargouille)
                    {
                        supprimerUnite(horde, morte);
                    }
                    else
                    {
                        supprimerUnite(player, morte);
                    }
                    morte = NULL;
                }
            }
            ordreAttaquants = ordreAttaquants->suiv;
        }

        SDL_Delay(50);
    }
}



TListePlayer ordreAttaque(TListePlayer *player, TListePlayer *horde)
{
    TListePlayer ordre;
    initTListePlayer(&ordre);

    TListePlayer ptrHorde = *horde;
    TListePlayer ptrPlayer = *player;

    while(ptrHorde != NULL)
    {
        AjouterUnite(&ordre, ptrHorde->pdata);
        ptrHorde = ptrHorde->suiv;
    }

    while(ptrPlayer != NULL)
    {
        AjouterUnite(&ordre, ptrPlayer->pdata);
        ptrPlayer = ptrPlayer->suiv;
    }

    TListePlayer en_cours, plus_petit, j;  //pointeurs sur cellule donc T_liste
    int a = 0;
    //nous ne faisons ici que des branchements et déplacements de pointeurs
    for (en_cours = ordre; en_cours != NULL; en_cours = en_cours->suiv) //avec "en_cours != fin", nous nous arrêtons bien sur l'avant dernière cellule
    {
        a = a+1;
        plus_petit = en_cours;
        for (j = en_cours ; j != NULL; j=j->suiv)
        {
            if (j->pdata->vitesseAttaque < plus_petit->pdata->vitesseAttaque )  //comparaison des contenus pointées (pas des pointeurs directement!)
                plus_petit = j;  //branchement de "plus_petit" sur la cellule ayant une valeur pointée par data plus petite que celle pointée par plus_petit
        }
        //comparaison avec la cellule fin non comparée dans le FOR ci-dessus
        Tunite* tmp = en_cours->pdata;
        en_cours->pdata = plus_petit->pdata;
        plus_petit->pdata = tmp;
    }

    return ordre;
}

Position bestPlacement(TplateauJeu jeu, int portee, int** chemin)
{
    int bestX = -1, bestY = -1;
    int bestScore = -1;

    for (int i=0; i<LARGEURJEU; i++)
    {
        for (int j=0; j<HAUTEURJEU; j++)
        {
            int score = 0;

            for (int k=0; k<NBCOORDPARCOURS; k++)
            {
                int x = chemin[k][0];
                int y = chemin[k][1];

                int distance = abs(i - x) + abs(j - y);

                // Vérifie si la case qu'on évalue est une case du chemin
                if(distance == 0)
                {
                    score = 0;
                    break;
                }

                if ((distance <= portee))
                {
                    score++;
                }
            }

            if ((score > bestScore)&&(jeu[i][j] == NULL))
            {
                bestScore = score;
                bestX = i;
                bestY = j;
            }
        }
    }
    Position pos;
    pos.posX = bestX;
    pos.posY = bestY;
    return pos;
}

void creeTourPlayer(TplateauJeu jeu, TListePlayer *player, int** chemin)
{
    int whichTour = rand()%100;
    Position pos;
    Tunite* unite;

    // TourSol = 65% -> 0-64
    if(whichTour >=0 && whichTour<=49)
    {
        pos = bestPlacement(jeu, 5, chemin);
        unite = creeTourSol(pos.posX, pos.posY);
    }
    // TourAir = 35% -> 65-99
    if(whichTour >=50 && whichTour<=99)
    {
        pos = bestPlacement(jeu, 3, chemin);
        unite = creeTourAir(pos.posX, pos.posY);
    }

    // Ajout au plateau de jeu puis liste du Player
    jeu[pos.posX][pos.posY] = unite;
    AjouterUnite(player, unite);
}

//sert a crée avec une unité de la horde(avec un certain % de chance pour chaqu'une)
void creeUniteHorde(TplateauJeu jeu, TListePlayer *horde, Position spawn)
{
    int whichUnite = rand()%100;
    Tunite* unite;

    // Chevalier = 15% -> 0-14
    if(whichUnite >=0 && whichUnite <=14)
    {
        unite = creeChevalier(spawn.posX, spawn.posY);
    }
    // Dragon = 20% -> 15-34
    if(whichUnite >=15 && whichUnite <=34)
    {
        unite = creeDragon(spawn.posX, spawn.posY);
    }
    // Archer = 35% -> 35-69
    if(whichUnite >=35 && whichUnite <=69)
    {
        unite = creeArcher(spawn.posX, spawn.posY);
    }
    // Gargouille = 30% ->70-99
    if(whichUnite >=70 && whichUnite <=99)
    {
        unite = creeGargouille(spawn.posX, spawn.posY);
    }

    // Ajout au plateau de jeu puis a la liste de la Horde
    jeu[spawn.posX][spawn.posY] = unite;
    AjouterUnite(horde, unite);
}

//ne sert que avant la boucle while du jeux pour placer les tours initiale
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu)
{
    TListePlayer en_cours = player;
    afficheTListePlayer(&player);
    while(en_cours != NULL)
    {
        printf("pos x : %d    pos y : %d\n",en_cours->pdata->posX,en_cours->pdata->posY);
        jeu[en_cours->pdata->posX][en_cours->pdata->posY] = en_cours->pdata;
        en_cours = en_cours->suiv;
    }
}
//on a besoin de garder le cas ou distance est egale a 0 avec le random pour eviter de tourner dans le wile en boucle si par exemple on est deja contre un mur
int** initCheminRandom()
{

    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = HAUTEURJEU-1;  //Dernière ligne
    int xdepart = rand()%LARGEURJEU;  //Une colonne aléatoire du plateau
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;
    int distance;
    int direction = rand()%3; // une direction aléatoire meme si elle va etre remise aléatoire dans le while
    int directionAv = direction;

    while(distanceMaxRestante > 0)
    {
        //ces conditions permette de ne pas retourner sur ces pas (faire gauche et pas droite sur une meme ligne juste apres) ou de ne pas aller en haut 2 fois d'affiler
        if(directionAv == 0)
        {
            do
            {
                direction = rand()%3;
            }
            while(direction == 0);
        }
        else if(directionAv == 1)
        {
            do
            {
                direction = rand()%3;
            }
            while(direction == 2);
        }
        else
        {
            do
            {
                direction = rand()%3;
            }
            while(direction == 1);
        }

        // Vers le haut
        if(direction == 0)
        {
            do
            {
                distance = rand()%4;
            }
            while((distance > distanceMaxRestante) || (ydepart - distance < 1));

            ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, distance, &distanceMaxRestante);
        }
        // Vers la gauche
        else if(direction == 1)
        {
            do
            {
                distance = rand()%4;
            }
            while((distance > distanceMaxRestante) || (xdepart - distance < 0));

            ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, distance, &distanceMaxRestante);
        }
        // Vers la droite
        else
        {
            do
            {
                distance = rand()%4;
            }
            while((distance > distanceMaxRestante) || (xdepart + distance > LARGEURJEU-1));

            ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, distance, &distanceMaxRestante);
        }
        //si on a avancer de 0 alors on reprend la valeurs precedente pour eviter de changer direction et retourner sur nos pas
        if(distance == 0)
        {
            direction = directionAv;
        }
        //sinon la direction d'avant deviens celle acctuel
        else
        {
            directionAv = direction;
        }

        distanceMaxRestante -= distance;
    }
    return chemin;  //tab2D contenant des pointeurs
}

bool tourRoiDetruite(TListePlayer player)
{
    if(player->pdata->nom != tourRoi)
    {
        return false;
    }
    return true;
}

int countUnite(TListePlayer player)
{
    TListePlayer ptr = player;
    int nb = 0;

    if(ptr == NULL)
    {
        return 0;
    }

    while(ptr != NULL)
    {
        nb++;
        ptr = ptr->suiv;
    }
    return nb;
}
