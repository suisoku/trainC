#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>

#define TAILLE_REQUETE 512
#define NM_REQUETE 10
#define TM_REQUETE 64
#define TAILLE_RESULTAT 2048



#define TAILLE_BUFFER 128
#define SIZE_LIGNE 1024
#define SIZE_MORCEAU 32
#define TAILLE_LISTE 32


char* interaction(char *A, char *B); //Cette fonction gère le menu principal et l'ensemble de la préparation d'une requête.
int affichage_Resultat(char *req); //Cette fonction gère l'affichage des résultats de la réponse du serveur
char* preferenceUser(char *ville_D, char *ville_A); //Cette fonction gère les options du résultat de la recherche
void enlever_RetourLigne(char * mot); //Cette fonction permet de retirer les retours à la ligne de fgets.
