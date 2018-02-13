#include "util_server.h"

/************************************FONCTION FLOAT*****************************************/








/************************************************FIN FONCTION FLOAT************************/






/*_________ ___________ fonction de decomposition _____________*/
void decomposer(char chaine[SIZE_LIGNE], char smart[SIZE_MORCEAU][TAILLE_BUFFER], char coupeur[]){
    char* pch;
    int i=0;
    pch = strtok(chaine, coupeur);

    while(pch !=NULL) {
          strcpy(smart[i] , pch);
          pch = strtok(NULL, coupeur);
          i++;
    }
}
/*_______________________________________________________________________________________________________________________________________________*/



/* _________________________________________LOAD DATA ________________________________________________________________________________________*/

struct_train load_data(char path_name[]){
	struct_train liste;
	char buffer[TAILLE_BUFFER];
	char ligne[SIZE_MORCEAU][TAILLE_BUFFER];
	FILE* reading = fopen(path_name, "r");
	int i = 0;
	int j;

	if (reading == NULL){
		printf("Error! opening file");
		exit(1);
  	}

  	/************* Chargement dans la structure *********/
	while(fgets(buffer, sizeof(buffer), reading) != NULL){
		decomposer(buffer, ligne , ";");

		strcpy(liste.train[i].numero , ligne[0]);
		strcpy(liste.train[i].ville_Depart , ligne[1]);
		strcpy(liste.train[i].ville_Arrivee , ligne[2]);
		strcpy(liste.train[i].heure_Depart , ligne[3]);
		strcpy(liste.train[i].heure_Arrivee , ligne[4]);
		strcpy(liste.train[i].prix , ligne[5] );
		strcpy(liste.train[i].option , ligne[6]);



    i++;
		liste.taille = i;
	}
	  fclose(reading);


	char buffer_price[20];

	/*********** Application du code modificateur de prix ************/

	float new_price;
	for(j = 0; j < liste.taille ; j++){

		if(strcmp(liste.train[j].option, "SUPPL\n") == 0 )		new_price = atof(liste.train[j].prix)*1.1;
		else if( strcmp(liste.train[j].option, "REDUC\n") == 0 )	new_price = atof(liste.train[j].prix)*0.8;
		else 													new_price = atof(liste.train[j].prix);

		sprintf(buffer_price, "%g", new_price);
		strcpy(liste.train[j].prixOption, buffer_price);
	}



	return liste;

}
/* ___________________________________________________________CITY OPERATION_____________________________________________________________________*/
struct_train city_operation(char requete_d[NM_REQUETE][TAILLE_BUFFER], struct_train liste){
	int i;
	struct_train new_liste;
	new_liste.taille = 0;
	for(i = 0; i < liste.taille ; i++){
		if( strcasecmp(liste.train[i].ville_Depart, requete_d[0]) == 0 && strcasecmp(liste.train[i].ville_Arrivee , requete_d[1]) == 0 ){
			new_liste.taille++;
			new_liste.train[ new_liste.taille - 1 ] = liste.train[i];
		}
	}
	return new_liste;
}
/* ___________________________________________________________________________________TRI HORAIRE_____________________________________________*/
struct_train tri_horaire(struct_train liste_recu){
   struct_train liste = liste_recu;
   int i, j;
   struct unTrain key;
   char horaire_train[liste.taille][2][TAILLE_BUFFER];
   char key_horaire[2][TAILLE_BUFFER];
   char res_heure[TAILLE_BUFFER];

	for(i = 0; i < liste.taille; i++){// on recupere tous  les horaires dans un tableau 2D ( plus facile a traiter)
		strcpy(res_heure , liste.train[i].heure_Depart);
		decomposer(res_heure, horaire_train[i], ":" );
	}

   for (i = 1; i < liste.taille; i++)
   {
   	   key = liste.train[i];
       strcpy(key_horaire[0], horaire_train[i][0]);
       strcpy(key_horaire[1], horaire_train[i][1]);

       j = i-1;

       while (j >= 0 && (atof(horaire_train[j][0]) + atof(horaire_train[j][1])/60 )  > (atof(key_horaire[0]) + atof(key_horaire[1])/60)	){

           liste.train[j+1] = liste.train[j];
           strcpy(horaire_train[j+1][0] , horaire_train[j][0]);
           strcpy(horaire_train[j+1][1] , horaire_train[j][1]);
           j = j-1;
       }

       liste.train[j+1] = key;
       strcpy(horaire_train[j+1][0] , key_horaire[0]);
       strcpy(horaire_train[j+1][1] , key_horaire[1]);
   }
   return liste;
}

//Enlever les retour à la lgne
void enlever_RetourLigne(char * mot)
{
    if(mot[strlen(mot) - 1] == '\n')
        mot[strlen(mot) - 1] = '\0';
}

/* _______________________________________________________________________________________OPERATION REQ 1____________________________*/
char* command_concat_liste(struct_train con_liste){
	char *commande = (char*)malloc(sizeof(char)*TAILLE_RESULTAT);
	int i;

	char buff[TAILLE_BUFFER];
	snprintf(buff, sizeof(buff), "%d", con_liste.taille);
	strcat(commande, buff );
	strcat(commande, "/" );
	for(i = 0; i < con_liste.taille; i++){

		strcat(commande, ">");
		strcat(commande, con_liste.train[i].numero );
		strcat(commande, ">");
		strcat(commande, con_liste.train[i].ville_Depart );
		strcat(commande, ">");
		strcat(commande, con_liste.train[i].ville_Arrivee );
		strcat(commande, ">");
		strcat(commande, con_liste.train[i].heure_Depart );
		strcat(commande, ">");
		strcat(commande, con_liste.train[i].heure_Arrivee );
		strcat(commande, ">");
		strcat(commande, con_liste.train[i].prix );
		strcat(commande, ">");

    if(strlen(con_liste.train[i].option) < 5){
       strcat(commande, "AUCUNE");
    }
		else {
      enlever_RetourLigne(con_liste.train[i].option);
      strcat(commande, con_liste.train[i].option);
    }
		strcat(commande, ">");
		strcat(commande, con_liste.train[i].prixOption);
		strcat(commande, "/");
	}

	return commande;
}
/* _______________________________________________________________________________________CONCAT UN TRAIN____________________________*/
char* command_concat(struct unTrain le_train){
	char *commande = (char*)malloc(sizeof(char)*TAILLE_RESULTAT);

	strcat(commande, "1");
	strcat(commande, "/" );
	strcat(commande, ">");
	strcat(commande, le_train.numero );
	strcat(commande, ">");
	strcat(commande, le_train.ville_Depart );
	strcat(commande, ">");
	strcat(commande, le_train.ville_Arrivee );
	strcat(commande, ">");
	strcat(commande, le_train.heure_Depart );
	strcat(commande, ">");
	strcat(commande, le_train.heure_Arrivee );
	strcat(commande, ">");
	strcat(commande, le_train.prix );
	strcat(commande, ">");
  if(strlen(le_train.option) < 5){
     strcat(commande, "AUCUNE");
  }
  else {
    enlever_RetourLigne(le_train.option);
    strcat(commande, le_train.option);
  }
	strcat(commande, ">");
	strcat(commande, le_train.prixOption);
	strcat(commande, "/");

	return commande;
}


/* _______________________________________________________________________________________OPERATION REQ 1____________________________*/

train* operation_req_1(char requete_d[NM_REQUETE][TAILLE_BUFFER], struct_train liste_r_sorted){
	struct_train liste_res;
	char horaire_req[2][TAILLE_BUFFER];
	char horaire_train[liste_r_sorted.taille][2][TAILLE_BUFFER];
	char tmpchar[TAILLE_BUFFER];
	char *resultat;
	int i;
	decomposer(requete_d[3], horaire_req, ":");
	train *train_se = malloc(sizeof(train));
	resultat = (char*)malloc(sizeof(char)*TAILLE_RESULTAT);

	for(i = 0; i < liste_r_sorted.taille; i++){
		strcpy(tmpchar , liste_r_sorted.train[i].heure_Depart);
		decomposer(tmpchar, horaire_train[i], ":" );
	}


	for(i = 0; i < liste_r_sorted.taille; i++){
		if((atof(horaire_train[i][0]) + atof(horaire_train[i][1])/60 )  >= (atof(horaire_req[0]) + atof(horaire_req[1])/60)){
			// operations non necessaires mais pour des soucis de standardisation la fonction prend une liste.
			*train_se = liste_r_sorted.train[i];
			return  train_se;
		}
	}
	return NULL;
}



/* _______________________________________________________________________________________OPERATION REQ 2____________________________*/

struct_train operation_req_2(char requete[NM_REQUETE][TAILLE_BUFFER], struct_train liste_r_sorted){
	struct_train liste_res;

	char horaire_req_min[2][TAILLE_BUFFER];
	char horaire_req_sup[2][TAILLE_BUFFER];

	char horaire_train_d[liste_r_sorted.taille][2][TAILLE_BUFFER];

	char tmpchar_d[TAILLE_BUFFER];


	int i;
	decomposer(requete[3], horaire_req_min, ":");
	decomposer(requete[4], horaire_req_sup, ":");


	for(i = 0; i < liste_r_sorted.taille; i++){
		strcpy(tmpchar_d , liste_r_sorted.train[i].heure_Depart);
		decomposer(tmpchar_d, horaire_train_d[i], ":" );
	}


	liste_res.taille =0;
	for(i = 0; i < liste_r_sorted.taille; i++){
		if((atof(horaire_train_d[i][0]) + atof(horaire_train_d[i][1])/60 )  >= (atof(horaire_req_min[0]) + atof(horaire_req_min[1])/60)){
			//printf("%s : %s\n", horaire_req_d[0], horaire_req_d[1]);
			if((atof(horaire_train_d[i][0]) + atof(horaire_train_d[i][1])/60 )  <= (atof(horaire_req_sup[0]) + atof(horaire_req_sup[1])/60)){
				liste_res.taille++;
				liste_res.train[ liste_res.taille - 1] = liste_r_sorted.train[i];
			}
		}
	}
	return liste_res;
}
/* ______________________________________________________________________________________OPERATION REQ 3____________________________*/
struct_train operation_req_3(char requete[NM_REQUETE][TAILLE_BUFFER], struct_train liste_r_sorted){
	return liste_r_sorted;
}

/* _______________________________________________________________________________________OPERATION PRIX___________________________*/


struct unTrain operation_prix(struct_train liste){
	int i;
	int min = 0 ;
	for(i = 0; i< liste.taille ; i++){
		if(atof(liste.train[i].prixOption) < atof(liste.train[min].prixOption))min = i;
	}
	return liste.train[min];
}

/* _______________________________________________________________________________________OPERATION DUREE____________________________*/

struct unTrain operation_duree(struct_train liste){
	int i;
	float actual_shift;

	/******** chargement des horaires *****/
	char horaire_train_d[liste.taille][2][TAILLE_BUFFER];
	char tmpchar_d[TAILLE_BUFFER];

	char horaire_train_a[liste.taille][2][TAILLE_BUFFER];
	char tmpchar_a[TAILLE_BUFFER];

	for(i = 0; i < liste.taille; i++){
		strcpy(tmpchar_d , liste.train[i].heure_Depart);
		decomposer(tmpchar_d, horaire_train_d[i], ":" );

		strcpy(tmpchar_a , liste.train[i].heure_Arrivee);
		decomposer(tmpchar_a, horaire_train_a[i], ":" );
	}
	/****** recherche du minimum ***/

	float shift_min = (atof(horaire_train_a[0][0]) + atof(horaire_train_a[0][1])/60 )  - (atof(horaire_train_d[0][0]) + atof(horaire_train_d[0][1])/60);
	int indice_min = 0;

	for(i = 0; i< liste.taille ; i++){

		actual_shift = (atof(horaire_train_a[i][0]) + atof(horaire_train_a[i][1])/60 )  - (atof(horaire_train_d[i][0]) + atof(horaire_train_d[i][1])/60);

		if( actual_shift < shift_min ){
			shift_min = actual_shift;
			indice_min = i;
		}
	}
	return liste.train[indice_min];
}

/* _______________________________________________________________________________________TRAITEMENT REQUETE____________________________*/

char* traiter_requete(char requete[NM_REQUETE][TAILLE_BUFFER] , struct_train liste){

		char* erreur = malloc(sizeof(char)*TAILLE_RESULTAT);
		strcpy(erreur, "PASDETRAIN");

		struct_train liste_filtre;
		train *train_select = malloc(sizeof(train_select));

		switch(atoi(requete[2]) ){
			case 1:
				train_select = operation_req_1(requete, liste);
				if(train_select == NULL)return erreur;
				else return command_concat(*train_select);

				break;
			case 2:
				liste_filtre =  operation_req_2(requete, liste);
				liste_stored = liste_filtre;

				if(liste_filtre.taille == 0)return erreur;
				else return command_concat_liste(liste_filtre);

				break;
			case 3:
				liste_filtre =  operation_req_3(requete, liste);
				liste_stored = liste_filtre;

				if(liste_filtre.taille == 0)return erreur;
				else return command_concat_liste(liste_filtre);

				break;
			case 4:
				return command_concat(operation_prix(liste_stored));
				break;
			case 5:
				return command_concat(operation_duree(liste_stored));
				break;
			default:
				strcpy(erreur, "ERROR_PREF");
				return erreur; // pas de requete
				break;
		}
		strcpy(erreur, "ERROR_SWITCH_CASE");
		return erreur; // cas requete ne marche pas
}
