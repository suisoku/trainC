#include "util_server.h"


void fin_fils(){
    wait(NULL);
}



/**********Fonction qui gere le comportement server*************/
void comportement_server(int socket_client){
    int taille_lect;
    char client_message[TAILLE_REQUETE];
    char* server_message = (char*)malloc(sizeof(char)*TAILLE_RESULTAT);
    char path_name[TAILLE_BUFFER] = "Trains.txt";

    /****** Lecture message ******/

    while( (taille_lect = recv(socket_client , client_message , TAILLE_REQUETE , 0)) > 0 )
    {
        puts("Reception:");
        puts(client_message);


        char requete_d[NM_REQUETE][TAILLE_BUFFER];


        decomposer(client_message, requete_d, ">"); //Desabillage de de la chaine en un tableau de chaine
        struct_train liste = load_data(path_name);  //Chargement des donnes dans la structure liste
        struct_train right_liste = city_operation(requete_d, liste); //filtrage des villes choisis
        struct_train liste_sorted = tri_horaire(right_liste); // Tri preliminaire par horaire de depart

       if(liste_sorted.taille == 0){
          strcpy(server_message, "ERROR_VILLE");
        }
        else {
          strcpy(server_message,traiter_requete(requete_d, liste_sorted));
        }
        puts(server_message);
      //  strcpy(server_message,traiter_requete(requete_d, liste_sorted));
        /*** Envoi de la requete par la fonction write **/
        write(socket_client , server_message  , TAILLE_RESULTAT); //On traite la suite de la requete/ Cf rapport sur le format de donnees
    }

    if(taille_lect == 0){
        puts("\nClient deconnecté");
        fflush(stdout);

    }
    else if(taille_lect == -1){
        perror("Erreur reception");
    }
}





int main(int argc , char *argv[])
{
    int socket_ecoute , socket_client , size_adr;
    struct sockaddr_in server , client;



    /*** structure pour handle le signal du fils ****/
    struct sigaction a;
    a.sa_handler = fin_fils;
    a.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &a, NULL);



    /*** Verification des arguments **/
    if(argc != 2){
        fprintf(stderr , "usage : PORT \n");
        exit(1);
    }

    /*** Creation de la socket d'ecoute ***/
    socket_ecoute = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_ecoute == -1){
        fprintf(stderr , "Erreur creation socket");
        exit(1);
    }
    puts("Creation de socket reussi");

    /****** Preparation structure ******/
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(  atoi(argv[1])  );

    /****** Bind ******/
    if( bind(socket_ecoute,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("Erreur bin");
        exit(1);
    }
    puts("Bind reussi");


    listen(socket_ecoute , NB_CONNEXIONS);


    puts("En attente de connexion...");
    size_adr = sizeof(struct sockaddr_in);


    while(1){
        /****** primitive blockante en attente de connexion ******/
        socket_client = accept(socket_ecoute, (struct sockaddr *)&client, (socklen_t*)&size_adr);
        if (socket_client < 0){
            perror("Erreur connexion");
            exit(1);
        }
        puts("Connexion reussie");

        switch( fork() )
        {
            case 0:
                comportement_server(socket_client);
                //close(socket_client);
                break;
            default:
                break;
        }
    }
    close(socket_ecoute);
    return 0;
}
