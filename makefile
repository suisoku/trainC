#Macro dependances/paths 

SRC_CLIENT = client/sources/client.c client/sources/util_client.c
SRC_SERVER = serveur/sources/server.c serveur/sources/util_server.c

OBJ_CLIENT = client/sources/client.o client/sources/util_client.o
OBJ_SERVER = serveur/sources/server.o serveur/sources/util_server.o
 
INCL_SERVER = -Iserveur/headers/
INCL_CLIENT = -Iclient/headers/

# Macros Execution
CC = gcc
EXE_CLIENT = client/bin/client
EXE_SERVER = serveur/bin/server


compile: $(EXE_CLIENT) $(EXE_SERVER)

$(EXE_CLIENT): 	$(OBJ_CLIENT)	
	gcc $(OBJ_CLIENT) -o $(EXE_CLIENT)

$(EXE_SERVER): 	$(OBJ_SERVER)	
	gcc $(OBJ_SERVER) -o $(EXE_SERVER)


client/sources/client.o: client/sources/client.c client/headers/util_client.h
	gcc $(INCL_CLIENT) -c client/sources/client.c -o client/sources/client.o

client/sources/util_client.o: client/sources/util_client.c client/headers/util_client.h
	gcc $(INCL_CLIENT) -c client/sources/util_client.c -o client/sources/util_client.o


serveur/sources/server.o: serveur/sources/server.c serveur/headers/util_server.h
	gcc $(INCL_SERVER) -c serveur/sources/server.c -o serveur/sources/server.o

serveur/sources/util_server.o: serveur/sources/util_server.c serveur/headers/util_server.h
	gcc $(INCL_SERVER) -c serveur/sources/util_server.c -o serveur/sources/util_server.o

clean:
	@echo "Supression des objets"
	rm $(OBJ_CLIENT) $(OBJ_SERVER)



# On gere les arguments suivant le make runS ( execution de server)
ifeq (run_server,$(firstword $(MAKECMDGOALS)))
  # utilise les reste de la commandes comme arguments
  ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS)) 
  $(eval $(ARGS):;@:)
endif
# meme chose pour execution client runC
ifeq (run_client,$(firstword $(MAKECMDGOALS)))
  # utilise les reste de la commandes comme arguments
  ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS)) 
  $(eval $(ARGS):;@:)
endif





run_server: $(EXE_SERVER)
	./$(EXE_SERVER) $(ARGS)

run_client: $(EXE_SERVER)
	./$(EXE_CLIENT) $(ARGS)
