#include <stdio.h>
#include "config.h"
#include "guardaIP.h"


int test_guardaIP_calcRange_i_calcIndex(){
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    //printf("la ip inicial és %s\n", IP_INI);
    //printf("la ip final és 192.168.2.3\n"); 
    //printf("El rang entre elles és %d\n", calcRange(IP_INI,"192.168.2.3"));
    //printf("L'índex de %s és %d\n", ip_test, calcIndex(ip_test));

    //cal acabar el test despres del debug.

    return calcRange(ip_test_ini,ip_test_final) == 262;
}


int test_guardaIP(){
    int ret = 1;
    if (test_guardaIP_calcRange_i_calcIndex()){
	printf("    Test funcions calcRange i calcIndex de guardaIP: OK\n");
    } else {
	printf("    Test funcions calcRange i calcIndex de guardaIP: FAIL\n");
	ret = 0; //no passa el test
    }
    return ret;
}



