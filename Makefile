################################
########### Makefile ###########
################################

#SI ES CANVIEN ELS PATHS NOMÉS FA FALTA CANVIAR AQUESTES 
#VARIABLES D'AQUÍ A BAIX I FUNCIONARA EL MAKEFILE. 

## Directori source amb els codis	
SRC_DIR=src

## Directori on estan els includes 	
INCLUDE=$(SRC_DIR)/include

## Directori dels tests
TESTS_DIR=tests

## Include dels tests
T_INCLUDE=$(TESTS_DIR)/include

## Directori del codi font dels tests
T_SRC_DIR=$(TESTS_DIR)/src

SRC= dns.c dnslib.c utils.c guardaIP.c  ## Codis en C que tenim i que es volen compilar. CAL POSAR AQUI TOTS ELS .C QUE ES VULGUI COMPILAR. MOLT IMPORTANT. AIXÒ PERMETRÀ TAMBÉ AFEGIR-LOS AL TEST !!!

SRC_TEST= guardaIP.c   #fitxers que es testejen 

OBJ=${SRC:.c=.o}
OBJ_TEST=${SRC_TEST:.c=.o}

CFLAGS= -Wall -std=c99 -ggdb -I $(INCLUDE) -I /usr/include/postgresql
CFLAGS_TEST= -Wall -std=c99 -ggdb -I $(INCLUDE) -I $(T_INCLUDE) -I /usr/include/postgresql
CC=gcc

#### FITXERS DEL CODI ####
all: dns 

dns: ${OBJ}	
	$(CC) $(CFLAGS) -o dns $^ -lpq

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $<

#### TESTS ####
## CAL TENIR EN COMPTE QUE CAL HAVER AFEGIT EL QUE ES VOL TESTEJAR A LA VAR FILES_TO_TEST ###

tests: ${OBJ_TEST} test.o 
	$(CC) -o test $^ -lpq 
	./test

test.o: 
	$(CC) $(CFLAGS_TEST) -c $(TESTS_DIR)/test.c



clean:     # AIXO ENS HO HEM DE CURRAR
	rm -rf *.o test dns $(INCLUDE)/*.gch
