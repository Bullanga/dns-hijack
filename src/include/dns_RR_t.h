
#ifndef _DNS_RR_H_
#define _DNS_RR_H_
#include <stdlib.h> 
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>


void generate_success_response(DNS_RR Request, const char *ip, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len);



void generate_failure_response(DNS_RR Request, int master_socket, const struct sockaddr client_addr, int client_len);

void parse_requested_domain(char *target, char *data);

void parse_client_ip(char *target, struct sockaddr *client);




#endif
