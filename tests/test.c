#include <stdio.h>
#include <string.h>
#include "wrapp.h"
#include "tests_guardaIP.h"

//la gracia d'aquest fitxer es anar posant els tests de cada paquet i veure si els compleix

int main(){
    //test de guardaIP
    int total_tests=0;
    int success_tests=0;

    //test de guardaIP
    printf("TESTS DE L'ESTRUCTURA DE DADES GUARDAIP:\n");
    total_tests++; //incremento els tests
    success_tests += test_guardaIP(); //si es bo em donarà 1.

//AQUI VAN ELS TESTS DE CADA FITXER QUE HI HA. Cal organitzar-los en carpetes


    //Summary
    printf("\n\nTested functionalities: %d\n", total_tests);
    printf("Successfull tests: %d\n", success_tests);
}

