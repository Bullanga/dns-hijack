#include <stdio.h>
#include "config.h"
#include "guardaIP.h"


int test_guardaIP_calcRange_i_calcIndex(){
    char* ip_test = "192.168.2.3";
    printf("la ip inicial és %s", IP_INI);
    printf("la ip final és 192.168.2.3"); 
    printf("El rang entre elles és %d", calcRange(IP_INI,"192.168.2.3"));
    printf("L'índex de %s és %d", ip_test, calcIndex(ip_test));

    //cal acabar el test despres del debug.
    return 1;
}


int test_guardaIP(){
    int ret = 1;
    if (test_guardaIP_calcRange_i_calcIndex()){
	printf("Test funcions calcRange i calcIndex de guardaIP: OK");
    } else {
	printf("Test funcions calcRange i calcIndex de guardaIP: FAIL");
	ret = 0; //no passa el test
    }
    return ret;
}



