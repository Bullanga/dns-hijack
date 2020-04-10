/* guardaIP.h*/

#ifndef guardaIP_H
#define guardaIP_H 

int lenght;
int* registry;

//Es detallen les funcions de l'interficie a continuació

//Inicialitza l'estructura
int init_guardaIP(char* ip_ini, char* ip_final);

//Borra l'estructura (tots els valors a 0)
int clear();

//Ip està a registry ? 1 : 0
int findValue();

//retorna la llargada
int get_lenght();

#endif


