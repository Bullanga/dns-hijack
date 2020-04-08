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
	rm -rf *.o $(TESTS_DIR)/*.o $(TESTS_DIR)/test dns 


#### FITXERS DEL CODI ####

dns_RR_t.o: dns_RR_t.c
	gcc -c dns_RR_t.c -I $(HEADERS)

guardaIP.o: guardaIP.c
	gcc -c guardaIP.c -I $(HEADERS)



#### TESTS ####
## CAL TENIR EN COMPTE QUE CAL HAVER AFEGIT EL QUE ES VOL TESTEJAR A LA VAR FILES_TO_TEST ###

tests: all_tests files_to_test
	gcc -o test $(TESTS_DIR)/test.c $(TESTS_DIR)/*.o $(SOURCE)/*.o -I $(HEADERS) 

files_to_test:
	for i in $(FILES_TO_TEST); do \
	    gcc -I $(HEADERS) -c  $(SOURCE)/$$i.c; \
	    done

all_tests: 
	gcc  -c $(T_SOURCE)/*.c -I $(HEADERS) -I $(SOURCE) -I $(T_HEADERS)



