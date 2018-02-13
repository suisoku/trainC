#include "util_client.h"

int main(int argc , char *argv[])
{
    int sock, resultatAffichage=0,flagPreference=0;
    struct sockaddr_in server;
    char A[TAILLE_BUFFER];
    char B[TAILLE_BUFFER];

    system("clear");

    //Création de la socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Erreur lors de la creation de la socket");
    }
    printf("Creation de la scoket reussie. ");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(argv[1]) );

    //Connexion avec le serveur
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Erreur de connexion");
        return 1;
    }

    puts("Connexion etablie\n\n");
    printf("\t\t********************************************");
    printf("\n\t\t*\t*===========================*      *");
    printf("\n\t\t*\t*   BIENVENUE MIAGE-NCF  !  *      *");
    printf("\n\t\t*\t*===========================*      *");
    printf("\n\t\t********************************************");
    //Communication avec le serveur
    while(1)
    {
      char message[TAILLE_REQUETE]="" , server_reply[TAILLE_RESULTAT]="";
      printf("\n\n\t- VEUILLEZ SAISIR UNE GARE DE DEPART: ");
      fgets(A, TAILLE_BUFFER,stdin); 
//while(getchar()!='\n');
      enlever_RetourLigne(A);

      printf("\n\n\t- VEUILLEZ SAISIR UNE GARE D'ARRIVEE: ");
      fgets(B, TAILLE_BUFFER, stdin);
      enlever_RetourLigne(B);

        flagPreference =0;
        //Saisie des informations (préparation de la requête du Client)
        char* res_interaction = (char*)malloc(sizeof(char)*TAILLE_RESULTAT);
        char* res_preference = (char*)malloc(sizeof(char)*TAILLE_RESULTAT);
        res_interaction = interaction(A,B);
        if (res_interaction==NULL) {
          exit(0);
        }
        else{
            strcpy(message, res_interaction);
        }

        //Envoi de la requête
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Echec lors de l'envoi");
            return 1;
        }

        //Reception de la réponse du serveur
        if( recv(sock , server_reply , TAILLE_RESULTAT , 0) < 0)
        {
            puts("Echec lors de la reception");
            break;
        }

        puts("Reponse du serveur:");
        resultatAffichage=affichage_Resultat(server_reply);
        if (resultatAffichage !=0){
          puts("MERCI DE VOTRE VISITE");
          return 0;
        }

        //A la sortie de l'affichage des résultats! On signale qu'il y a des possibilité de filtrer la recherche avec flagPreference;
        flagPreference=1;
        if (flagPreference == 1 && resultatAffichage == 0){
          res_preference=preferenceUser(A,B);
          strcpy(message, res_preference);
          puts(message);
          if( send(sock , message , strlen(message) , 0) < 0)
          {
              puts("Echec lors de l'envoi");
              return 1;
          }


          while (recv(sock , server_reply , TAILLE_RESULTAT , 0) < 0);
          //Reception de la réponse du serveur

            puts("Reponse du serveur:");
            puts(server_reply);
            affichage_Resultat(server_reply);
            puts("MERCI DE VOTRE VISITE");
            return 0;
        }
    }

    close(sock);
    return 0;
}
