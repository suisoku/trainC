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
#include <errno.h>

#define TAILLE_REQUETE 512
#define NM_REQUETE 10
#define TAILLE_RESULTAT 2048
#define TAILLE_BUFFER 128
#define SIZE_LIGNE 1024
#define SIZE_MORCEAU 32
#define TAILLE_LISTE 32


#define NB_CONNEXIONS 5

/* _________________________________________STRUCTURES _______________________________________________________________________________*/

typedef struct unTrain
{
	char numero[TAILLE_REQUETE];
	char ville_Depart[TAILLE_BUFFER];
	char ville_Arrivee[TAILLE_BUFFER];
	char heure_Depart[TAILLE_BUFFER];
	char heure_Arrivee[TAILLE_BUFFER];
	char prix[TAILLE_BUFFER];
	char option[TAILLE_BUFFER];
	char prixOption[TAILLE_BUFFER];
} train;

typedef struct trains
{
	struct unTrain train[TAILLE_LISTE];
	int taille;

} struct_train;

struct_train liste_stored;

void decomposer(char chaine[SIZE_LIGNE], char smart[SIZE_MORCEAU][TAILLE_BUFFER], char coupeur[]);
struct_train load_data(char path_name[]);
struct_train city_operation(char requete_d[NM_REQUETE][TAILLE_BUFFER], struct_train liste);
struct_train tri_horaire(struct_train liste_recu);
train* operation_req_1(char requete_d[NM_REQUETE][TAILLE_BUFFER], struct_train liste_r_sorted);
struct_train operation_req_2(char requete_d[NM_REQUETE][TAILLE_BUFFER], struct_train liste_r_sorted);
struct_train operation_req_3(char requete[NM_REQUETE][TAILLE_BUFFER], struct_train liste_r_sorted);
char* traiter_requete(char requete[NM_REQUETE][TAILLE_BUFFER], struct_train liste);
