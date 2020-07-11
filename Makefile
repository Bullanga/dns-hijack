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

## Fitxers als que aplicar test
FILES_TO_TEST= guardaIP

SRC= dns.c dns_RR_t.c process.c	guardaIP.c checks.c	 ## Codis en C que tenim i que es volen compilar. CAL POSAR AQUI TOTS ELS .C QUE ES VULGUI COMPILAR. MOLT IMPORTANT !!!

OBJ=${SRC:.c=.o}



CFLAGS= -Wall -ggdb -I $(INCLUDE)
CC=gcc

#### FITXERS DEL CODI ####
all: dns

dns: ${OBJ}	
	$(CC) $(CFLAGS) -o dns $^

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $<

#### TESTS ####
## CAL TENIR EN COMPTE QUE CAL HAVER AFEGIT EL QUE ES VOL TESTEJAR A LA VAR FILES_TO_TEST ###

tests: all_tests
	$(CC) -o test $(TESTS_DIR)/test.c *.o -I $(INCLUDE) -I $(T_INCLUDE) -I $(T_SRC_DIR) -I $(SRC_DIR)   
	./test

files_to_test:
	for i in $(FILES_TO_TEST); do \
	    $(CC) -g -I $(INCLUDE) -c  $(SRC_DIR)/$$i.c; \
	    done

all_tests: 
	$(CC)  -g -c $(T_SRC_DIR)/*.c -I $(INCLUDE) -I $(SRC_DIR) -I $(T_INCLUDE)



clean:     # AIXO ENS HO HEM DE CURRAR
	rm -rf *.o test dns $(INCLUDE)/*.gch
