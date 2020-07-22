#include "checks.h"
#include "wrapp.h"

//check si la ip està registrada
int check_if_registered(char* ip){
    return r_findValue(ip);
}

int check_if_public_domain(char* req_domain){
    int i; 
    for (i=0; i < public_domains_num; i++){
	if (0 == strcmp(req_domain, public_domains[i])){
	    return 1;
	}
    }
    return 0;
}


int check_if_private_domain(char* req_domain){
    int i; 
    for (i=0; i < private_domains_num; i++){
	if (0 == strcmp(req_domain,private_domains[i])){
	    return 1;
	}
    }
    return 0;
}

int check_if_use_inite(){
    return use_inite;
}
