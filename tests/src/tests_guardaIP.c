#include <stdio.h>
#include "guardaIP.h"


int test_guardaIP_init_guardaIP(){
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    //Aquest rang és de 262 IPs
    if (!init_guardaIP(ip_test_ini,ip_test_final)) return 0;
    if (r_length != 263) return 0;
    return 1;
}

int test_guardaIP_r_clear(){
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    init_guardaIP(ip_test_ini, ip_test_final);
    int i;
    for (i = 0; i < r_length; i++){
	if (registry[i] != 0) return 0;
    }
    return 1;
}

int test_guardaIP_r_add(){ 
    char* ip_test_ini = IP_INI;
    char* ip_test_final = "192.168.3.10";
    init_guardaIP(ip_test_ini, ip_test_final);
    char a[]= "192.168.2.4";
    return r_add(a) && registry[160];
}

int test_guardaIP_r_findValue(){ 
    char* ip_test_ini = "192.168.2.3";
    char* ip_test_final = "192.168.3.10";
    init_guardaIP(ip_test_ini, ip_test_final);
    char a[]= "192.168.2.4";
    r_add(a);
    return r_findValue(a);
}


int test_guardaIP(){
    int ret = 1;

    //TEST DE LA FUNCIÓ init_guuardaIP
    if (test_guardaIP_init_guardaIP()){
	printf("    Test funció	init_guardaIP: OK\n");
    } else {
	printf("    Test funció init_guardaIP: FAIL\n");
	ret = 0; //no passa el test
    }

    //TEST DE LA FUNCIÓ r_clear 
    if (test_guardaIP_r_clear()){
	printf("    Test funció	r_clear: OK\n");
    } else {
	printf("    Test funció	r_clear: FAIL\n");
	ret = 0; //no passa el test
    }
    
    //TEST DE LA FUNCIÓ r_add 
    if (test_guardaIP_r_add()){
	printf("    Test funció	add: OK\n");
    } else {
	printf("    Test funció	add: FAIL\n");
	ret = 0; //no passa el test
    }
    

    //TEST DE LA FUNCIÓ r_findValue 
    if (test_guardaIP_r_findValue){
	printf("    Test funció	r_findValue: OK\n");
    } else {
	printf("    Test funció	r_findValue: FAIL\n");
	ret = 0; //no passa el test
    }
    
    return ret;
}



