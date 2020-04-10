/* guardaIP.h*/
#ifndef guardaIP_H

#include "config.h"
#define guardaIP_H 

int r_length;
int* registry;

//Es detallen les funcions de l'interficie a continuació

//Inicialitza l'estructura ip_ini ha de ser la constant IP_INI.
int init_guardaIP(char* ip_ini, char* ip_final);

//Borra l'estructura (tots els valors a 0)
int r_clear();

//Ip està a registry ? 1 : 0
int r_findValue();

//retorna la llargada
int r_get_length();

//afegeix una ip a les ips registrades. Retorna l'index al vector.
int r_add(char* ip);

#endif


