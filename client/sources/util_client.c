#include "util_client.h"



/****************************************************  DEBUT DES OPERATIONS **********************************************************/
/***********************************************************************************************************************************/

///Enlever les retour à la lgne
void enlever_RetourLigne(char * mot)
{
    if(mot[strlen(mot) - 1] == '\n')
        mot[strlen(mot) - 1] = '\0';
}



/**** Opération 1 ****
**********************/
char* operation1(char *ville_D, char *ville_A)
{

    char heure[5] = "";
    char minutes[3] = "";
    do
    {
        printf("Veuillez entrer l'heure  de départ (HH:MM):");
        do
        {
            printf("\nHH:");
            scanf("%s", heure);
        }
        while ((strcasecmp(heure,"0")>=0) && (strcasecmp(heure,"24")>=0) );

        do
        {
            printf("MIN:");
            scanf("%s", minutes);
        }
        while ((strcasecmp(minutes,"0")>=0) && (strcasecmp(minutes,"59")>0) );
        strcat(heure,":");
        strcat(heure, minutes);
    }
    while(strlen(heure)!=5.0);

    char *result = malloc(strlen(ville_D)+strlen(ville_A)+strlen(heure));
    strcpy(result, ville_D);
    strcat(result,">");
    strcat(result, ville_A);
    strcat(result,">");
    strcat(result,"1");
    strcat(result,">");
    strcat(result,heure);
    return result;
}


/**** Opération 2 ****
**********************/
char* operation2(char *ville_D, char *ville_A)
{
    int erreur = 0, erreur_2 = 0;

    char heure_D[5] = "";// Heure depart min
    char minutes_D[3] = "";
    char heure_A[5] = "";// Heure depart max
    char minutes_A[3] = "";
    printf("Veuillez entrer la 1ere heure de départ (HH : MM) :");

    //-----------------------HEURE DE DEPART MAX-----------------------
    do
    {
        erreur += 1 ;
        if (erreur > 1 )
        {
            printf("Erreur! Veuillez revoir votre saisie.");
        }

        do
        {
            printf("\nHH:");
            scanf("%s", heure_D);
        }
        while ((strcasecmp(heure_D,"0")>=0) && (strcasecmp(heure_D,"24")>=0) );

        do
        {
            printf("MIN:");
            scanf("%s", minutes_D);
        }
        while ((strcasecmp(minutes_D,"0")>=0) && (strcasecmp(minutes_D,"59")>0) );
        strcat(heure_D,":");
        strcat(heure_D, minutes_D);

    }
    while(strlen(heure_D)!=5.0);


    //----------------------HEURE DE DEPART MAX---------------------------------
    printf("Veuillez entrer la 2ieme heure de depart (HH : MM):");
    do
    {
        do
        {
            erreur_2 += 1 ;
            if (erreur_2 > 1 )
            {
                printf("Erreur! Veuillez saisir une heure ultérieure.");
            }

            printf("\nHH:");
            scanf("%s", heure_A);
        }
        while ((strcasecmp(heure_A,"0")>=0) && (strcasecmp(heure_A,"24")>=0) );
        do
        {

            printf("MIN:");
            scanf("%s", minutes_A);
        }
        while ((strcasecmp(minutes_A,"0")>=0) && (strcasecmp(minutes_A,"59")>0) );
        strcat(heure_A,":");
        strcat(heure_A, minutes_A);

    }
    while((strlen(heure_A)!=5.0) || (strcasecmp(heure_A,heure_D)<0) );


    char *result = malloc(strlen(ville_D)+strlen(ville_A)+strlen(heure_A)+strlen(heure_A));
    strcpy(result, ville_D);
    strcat(result,">");
    strcat(result, ville_A);
    strcat(result,">");
    strcat(result,"2");
    strcat(result,">");
    strcat(result, heure_D);
    strcat(result,">");
    strcat(result, heure_A);
    return result;
}

/**** Opération 3 ****
**********************/
char* operation3(char *ville_D, char *ville_A)
{
    char *result = malloc(strlen(ville_D)+strlen(ville_A));
    strcpy(result, ville_D);
    strcat(result,">");
    strcat(result, ville_A);
    strcat(result,">");
    strcat(result,"3");
    return result;
}

/**** Opération Filtre Recherche ****
*************************************/
char* preferenceUser(char *ville_D, char *ville_A)
{
    char *result = malloc(sizeof(char)*TAILLE_REQUETE);
    int preference = 0;

    strcpy(result, ville_D);
    strcat(result,">");
    strcat(result, ville_A);
    strcat(result,">");

    //---------------preference----------------------------------------------------

    do
    {
        printf("Avez-vous des preferences de recherche (1- Prix / 2- Duree  / 3- Non)?\nVotre choix : ");
        scanf ("%d", &preference);
    }
    while ((preference != 1) && (preference != 2) && (preference != 3));

    switch (preference)
    {
    case 1:
        strcat(result,"4");
        break;
    case 2:
        strcat(result,"5");
        break;
    default:
        strcat(result,"6");
        break;
    }

    strcat(result,">");
    return result;
}

/****************************************************  FIN DES OPERATIONS **********************************************************/
/***********************************************************************************************************************************/




/****  FONCTION DE DECOUPAGE D'UNE REQUETE ****
***********************************************/

int decomposer(char chaine[SIZE_LIGNE], char smart[SIZE_MORCEAU][TAILLE_BUFFER], char coupeur[])
{
    char* pch;
    int i=0;
    pch = strtok(chaine, coupeur);

    while(pch !=NULL)
    {
        strcpy(smart[i] , pch);
        pch = strtok(NULL, coupeur);
        i++;
    }
    return i;
}


/****  FONCTION D'AFFICHAGE DE LA REPONSE DU SERVEUR ****
*********************************************************/

int affichage_Resultat(char *req)
{
    char  reponseServeur[TAILLE_RESULTAT]; ///  1/ CONTIENDRA LA REPONSE DU SERVEUR

    char reponse_D[SIZE_MORCEAU][TAILLE_BUFFER];///     2/ CONTIENDRA LA REPONSE DU SERVEUR: APRES DECOUPAGE PAR TRAIN

    char trainReponse_D[SIZE_MORCEAU][TAILLE_BUFFER];///     2/ CONTIENDRA LA REPONSE DU SERVEUR: APRES DECOUPAGE PAR TRAIN

    strcpy(reponseServeur,req);///      3/ COPIE DE LA REPONSE DU SERVEUR DANS LE TABLEAU DANS 1/

    decomposer(reponseServeur,reponse_D, "/");

    if (strcmp(reponse_D[0], "ERROR_VILLE") == 0){
      printf("\t\t ERREUR: Aucune ville ne correspond à votre recherche.\n" );
      return 1;
    }
    else if (strcmp(reponse_D[0], "PASDETRAIN") == 0) {
      printf("\t\t Aucun train ne correspond à votre recherche.\n" );
      return 1;
    }
    else if (strcmp(reponse_D[0], "ERROR_PREF") == 0){
      printf("\t\tMIAGE-SNCF vous souhaite un bon voyage!\n" );
      return 0;
    }
    else if (strcmp(reponse_D[0], "ERROR_SWITCH_CASE") == 0){
      printf("\t\tNotre serveur rencontre des problèmes.\nVeuillez relancer une nouvelle recherche\n" );
      return 1;
    }
    else{
      int i=0, nombreDeTrains=atoi(reponse_D[0]); //nombreInfostrain = 0;

      for (i=1; i<=nombreDeTrains; i++)
      {
          int nombre_Champs=(decomposer(reponse_D[i],trainReponse_D, ">"));;

          printf("\t\t\t===============================");
          printf("\n\t\t\t       TRAIN N: %s           ", trainReponse_D[0]);
          printf("\n\t\t\t=============================");

          //LE SERVEUR S'OCCUPERA DE NOUS PASSER LE NOMBRE DE TRAINS ==> LA TAILLE DU TABLEAU DE TRAINS

          printf("\n\t- GARE DE DEPART: %s", trainReponse_D[1]);
          printf("\n\t- GARE D'ARRIVEE: %s", trainReponse_D[2]);
          printf("\n\t- HEURE DE DEPART: %s", trainReponse_D[3]);
          printf("\n\t- HEURE D'ARRIVEE: %s", trainReponse_D[4]);
          printf("\n\t- PRIX: %s euros", trainReponse_D[5]);
          printf("\n\t- SUPPL/REDUC: %s",trainReponse_D[6]);
          printf("\n\t- NOUVEAU PRIX: %s euros\n\n\n",trainReponse_D[7]);
      }
      if (nombreDeTrains==1) return 5;
      else {
      return 0; }
    }
}

char* interaction(char *A, char *B)
{
    int menu = 1;
    char *req;


   // system("clear");
    while(menu < 5 && menu > 0)
    {
        do
        {
            system("clear");
            printf("\t\t********************************************");
            printf("\n\t\t*\t*===========================*      *");
            printf("\n\t\t*\t*   BIENVENUE MIAGE-NCF  !  *      *");
            printf("\n\t\t*\t*===========================*      *");
            printf("\n\t\t********************************************");
            printf("\n\nVeuillez choisir l'operation :\n");
            printf("1- Cherher un train à partir d'un horaire de départ\n");
            printf("2- Cherher un train à partir d'un horaire de départ et d'un horaire d'arrivee\n");
            printf("3- Lister tous les trains\n");
            printf("4- Quitter\n\n");
            printf("Votre choix ici: ");
            scanf("%d", &menu);
        }
        while (menu < 0 || menu > 4);

        switch(menu)
        {
        case 1 :

            printf("\n\t\t=====================================");
            printf("\n\t\t   TRAINS SELON L'HEURE DE DEPART     ");
            printf("\n\t\t=====================================\n");

            /// CONNEXION AVEC LE SERVEUR
            /// ON ENVOIE LA REQUETE
            req=operation1(A,B);
			      return req;
            ///TRAITEMENT ET AFFICHAGE DE LA REPONSE DU SERVEUR

           // APRES AFFICHAGE_RESULTAT(reponseDuServeur);

            ///DEMANDE DE PREFERENCES RECHERCHES
            //req=operation4(A,B);
            break;
        case 2 :

            printf("\n\t======================================");
            printf("\n\t   TRAINS SELON UNE PLAGE HORAIRE    ");
            printf("\n\t======================================\n");

            /// CONNEXION AVEC LE SERVEUR
            /// ON ENVOIE LA REQUETE
            req=operation2(A,B);
            return req;

            // APRES AFFICHAGE_RESULTAT(reponseDuServeur);

            ///DEMANDE DE PREFERENCES RECHERCHES
            //req=operation4(A,B);

            ///TRAITEMENT ET AFFICHAGE DE LA REPONSE DU SERVEUR
            break;
        case 3 :

            printf("\n\t\t================================");
            printf("\n\t\t   LISTE DE TOUS LES TRAINS     ");
            printf("\n\t\t================================\n");

            /// CONNEXION AVEC LE SERVEUR
            /// ON ENVOIE req GRACE A SEND()
            req=operation3(A,B);
            return req;

            // APRES AFFICHAGE_RESULTAT(reponseDuServeur);
            break;
        default :
            printf("Au revoir\n");
            break;
        }
        return NULL;
       	break;
    }
}
