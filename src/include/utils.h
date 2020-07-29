#ifndef __UTILS_H
#define __UTILS_H

int check_if_private_domain(char* req_domain);
int check_if_public_domain(char* req_domain);
int check_if_registered(char* ip);
int check_if_use_inite();
void process(DNS_RR Request, int master_socket, const struct sockaddr client_addr, socklen_t client_len);
int resolve_query(char ip[16], char* req_domain);

#endif
