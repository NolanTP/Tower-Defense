#include "SDL.h"
#include "maSDL.h"    //bibliotheque avec des fonctions d'affichage utilisant la SDL
#include "towerdefend.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TListePlayer.h"
#include "Save.h"

/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
    // Initialiser le RANDOM
    srand(time(NULL));

    int test=0;
    if(test==1)
    {
        // Test du type TListePlayer
        Tunite* dragon = creeDragon(0,0);
        Tunite* roi = creeTourRoi(1,0);
        Tunite* tourAir = creeTourAir(2,0);
        Tunite* tourSol = creeTourSol(3,0);
        Tunite* tourAir2 = creeTourAir(4,0);

        TListePlayer player;

        initTListePlayer(&player);
        AjouterUnite(&player, dragon);
        AjouterUnite(&player, roi);
        AjouterUnite(&player, tourAir);
        AjouterUnite(&player, tourSol);
        AjouterUnite(&player, tourAir2);
        printf("ajout termine\n");
        afficheTListePlayer(&player);
        printf("affichage de la liste termine\n");


        supprimerUnite(&player, tourSol);
        printf("Suppression du tourSol (0) terminee\n");
        afficheTListePlayer(&player);


        supprimerUnite(&player, dragon);
        printf("Suppression du dragon (5) terminee\n");
        afficheTListePlayer(&player);

        supprimerUnite(&player, tourAir2);
        printf("Suppression du tourAir2 (1) terminee\n");
        afficheTListePlayer(&player);


        return 0;
    }
    else
    {
        SDL_Window *pWindow;
        SDL_Init(SDL_INIT_VIDEO);

        pWindow = SDL_CreateWindow(
            "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            LARGEURJEU*TAILLE,
            HAUTEURJEU*TAILLE,
            SDL_WINDOW_SHOWN
        );

        //SDL_Renderer * renderer = SDL_CreateRenderer(pWindow, -1, 0);  //non utilise, pour memoire

        SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow);  //le sprite qui couvre tout l'ecran
        SDL_Surface* pSpriteTourSol = SDL_LoadBMP("./data/NewTourSol_80x80.bmp");  //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
        SDL_Surface* pSpriteTourAir = SDL_LoadBMP("./data/NewTourAir_80x80.bmp");  //indice 1 dans tabSprite (via l'enum TuniteDuJeu)
        SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data/NewRoi_80x80.bmp"); //indice 2
        SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data/NewArcher_80x80.bmp"); //indice 3
        SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data/NewChevalier_80x80.bmp"); //indice 4
        SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data/NewDragon_80x80.bmp"); //indice 5
        SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data/NewGargouille_80x80.bmp"); //indice 6
        SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Eau.bmp"); //indice 7  Ne figure pas dans l'enum TuniteDuJeu
        SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Herbe_80x80.bmp"); //indice 8 idem
        SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Pont_80x80.bmp"); //indice 9 idem
        SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp"); //indice 10 idem

        // ASTUCE : on stocke le sprite d'une unite a l'indice de son nom dans le type enum TuniteDuJeu, dans le tableau TabSprite
        // SAUF pour l'Eau, l''herbe et le pont qui apparaitront en l absence d'unite (NULL dans le plateau) et en foction de certains indices x,y definissant le chemin central
        SDL_Surface* TabSprite[11]={pSpriteTourSol,pSpriteTourAir,pSpriteTourRoi,pSpriteArcher,pSpriteChevalier,pSpriteDragon,pSpriteGargouille,pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre};

        int** tabParcours = initCheminRandom();  //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau � 2 cases (indice 0 pour X, indice 1 pour Y)

        if ( pSpriteTourSol )  //si le permier sprite a bien ete charge, on suppose que les autres aussi
        {
            TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
            initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
            affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);



            prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
            maj_fenetre(pWindow);


            /**********************************************************************/
            /*              DEFINISSEZ/INITIALISER ICI VOS VARIABLES              */
            /*                                                                    */
            /* player : 1 Roi et 2 tours de chaques au debut du jeu               */
            /**********************************************************************/
            TListePlayer player, horde;
            initTListePlayer(&player);
            initTListePlayer(&horde);

            // Un entier de 0-99, correspondant au pourcentage de chance de creer une unite
            int createHorde, createPlayer;

            // Spwan des unites de la Horde : tout en bas du chemin
            Position spawnHorde = {tabParcours[0][X], tabParcours[0][Y]};

            // Creation d'une tour de chaque pour le Player
            Position posRoi = {tabParcours[NBCOORDPARCOURS-1][X], tabParcours[NBCOORDPARCOURS-1][Y]};
            Tunite* roi = creeTourRoi(posRoi.posX, posRoi.posY - 1);

            AjouterUnite(&player, roi);

            Position posAir = bestPlacement(jeu, 3, tabParcours);
            Position posSol = bestPlacement(jeu, 5, tabParcours);
            Tunite* air = creeTourAir(posAir.posX, posAir.posY);
            Tunite* sol = creeTourSol(posSol.posX, posSol.posY);

            AjouterUnite(&player, air);
            AjouterUnite(&player, sol);


            PositionnePlayerOnPlateau(player, jeu);


            // boucle principale du jeu
            int cont = 1;
            while ( tourRoiDetruite(player) && (cont != 0) ){

                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            cont = 0;
                        }

                        if (event.type == SDL_KEYDOWN) {
                            switch (event.key.keysym.scancode) {

                                case SDL_SCANCODE_ESCAPE:
                                    printf("on finit\n");
                                    cont = 0;
                                    break;

                                case SDL_SCANCODE_V:
                                    message("charge partie txt","V pressed");
                                    jeu = loadGameTxt(&player, &horde, &tabParcours);
                                    spawnHorde.posX = tabParcours[0][X];
                                    spawnHorde.posY = tabParcours[0][Y];
                                    break;

                                case SDL_SCANCODE_C:
                                    message("Charge partie binaire","C pressed");
                                    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
                                    jeu = loadGameBin(&player, &horde, &tabParcours);
                                    spawnHorde.posX = tabParcours[0][X];
                                    spawnHorde.posY = tabParcours[0][Y];
                                    break;

                                case SDL_SCANCODE_D:
                                    message("Sauvegarde txt","D pressed");
                                    saveGameTxt(player,horde,tabParcours);
                                    break;

                                case SDL_SCANCODE_S:
                                    message("Sauvegarde binaire ","S pressed");
                                    saveGameBin(player,horde,tabParcours);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }


                    printf("unite = %d, pv = %d\n", player->pdata->nom, player->pdata->pointsDeVie);
                    SDL_PumpEvents(); //do events
                    efface_fenetre(pWinSurf);
                    prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                    affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);
                    /**********************************************************************/
                    /* Explication du deroulement du jeu                                  */
                    /* Etape 1 : Les unites des deux camps attaquent (la priorite depend  */
                    /*           de celui qui a la plus grande vitesse d'attaque          */
                    /* Etape 2 : Toutes les unites ennemis avancent                       */
                    /* Etape 3 : Creation de nouvelle unite en fonction d'un taux de spawn*/
                    /* Attente entre chaque Etape = 0.1s                                  */
                    /**********************************************************************/

                    /// Step 1 ///

                    combat (&player,&horde , &jeu , pWinSurf);

                    /// Step 2 ///

                    avancerUnites(jeu, tabParcours);
                    SDL_Delay(20);; // 0.1s

                    /// Step 3 ///

                    // Creer une unite de la Horde : 50% de chance
                    createHorde = rand()%100; // 0-99
                    if(createHorde >= 50)
                    {
                        creeUniteHorde(jeu, &horde, spawnHorde);
                    }

                    // Creer une unite du Player : 50%
                    createPlayer = rand()%100;
                    if(createPlayer >= 80)
                    {
                        creeTourPlayer(jeu, &player, tabParcours);
                    }

                    SDL_Delay(10);; // 0.1s

                    /**********************************************************************/
                    /*                              FIN                                   */
                    /**********************************************************************/

                    maj_fenetre(pWindow);
                    SDL_Delay(150);  //valeur du delai a modifier eventuellement




            }
            printf("Fin du jeu\n");
            //fin boucle du jeu

            SDL_FreeSurface(pSpriteTourSol); // Liberation de la ressource occupee par le sprite
            SDL_FreeSurface(pSpriteTourAir);
            SDL_FreeSurface(pSpriteTourRoi);
            SDL_FreeSurface(pSpriteArcher);
            SDL_FreeSurface(pSpriteChevalier);
            SDL_FreeSurface(pSpriteDragon);
            SDL_FreeSurface(pSpriteGargouille);
            SDL_FreeSurface(pSpriteEau);
            SDL_FreeSurface(pSpriteHerbe);
            SDL_FreeSurface(pSpritePont);
            SDL_FreeSurface(pWinSurf);
        }
        else
        {
            fprintf(stdout,"Echec de chargement du sprite (%s)\n",SDL_GetError());
        }

        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        freeChemin(tabParcours);
        return 0;
    }
}
