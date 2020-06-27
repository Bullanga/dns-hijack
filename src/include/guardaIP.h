/* guardaIP.h*/
#ifndef guardaIP_H
#define guardaIP_H 

int r_length;
int* registry;

//Es detallen les funcions de l'interficie a continuació

//Inicialitza l'estructura ip_ini ha de ser la constant IP_INI.
int init_guardaIP(const char* ip_ini, const char* ip_final);

//Borra l'estructura (tots els valors a 0)
int r_clear();

//Ip està a registry ? 1 : 0
int r_findValue (char* ip);

//retorna la llargada
int r_get_length();

//afegeix una ip a les ips registrades. Retorna l'index al vector.
int r_add(const char* ip);

#endif


