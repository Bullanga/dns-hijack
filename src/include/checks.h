#ifndef __CHECKS_H
#define __CHECKS_H
#include <string.h>


//check si la ip està registrada
int check_if_registered(char* ip);

int check_if_public_domain(char* req_domain);

int check_if_private_domain(char* req_domain);

int check_if_use_inite();

int resolve_query(char* req_domain, char ip[16]);

#endif
