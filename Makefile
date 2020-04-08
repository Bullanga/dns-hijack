################################
########### Makefile ###########
################################

HEADERS=include
SOURCE=src
TESTS_DIR=tests
T_HEADERS=$(TESTS_DIR)/include
T_SOURCE=$(TESTS_DIR)/src
FILES_TO_TEST=guardaIP 

all: dns_RR_t.o tests
	gcc -o dns -I $(HEADERS) $(SOURCE)/dns.c dns_RR_t.o 

clean:     # AIXO ENS HO HEM DE CURRAR
	rm -rf *.o test dns 


#### FITXERS DEL CODI ####

dns_RR_t.o: $(SOURCE)/dns_RR_t.c
	gcc -c $(SOURCE)/dns_RR_t.c -I $(HEADERS)

guardaIP.o: $(SOURCE)/guardaIP.c
	gcc -c $(SOURCE)/guardaIP.c -I $(HEADERS)



#### TESTS ####
## CAL TENIR EN COMPTE QUE CAL HAVER AFEGIT EL QUE ES VOL TESTEJAR A LA VAR FILES_TO_TEST ###

tests: all_tests files_to_test
	gcc -o test $(TESTS_DIR)/test.c *.o -I $(HEADERS) -I $(T_HEADERS) -I $(T_SOURCE) -I $(SOURCE)   

files_to_test:
	for i in $(FILES_TO_TEST); do \
	    gcc -g -I $(HEADERS) -c  $(SOURCE)/$$i.c; \
	    done

all_tests: 
	gcc  -g -c $(T_SOURCE)/*.c -I $(HEADERS) -I $(SOURCE) -I $(T_HEADERS)



