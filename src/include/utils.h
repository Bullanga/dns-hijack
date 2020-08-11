#ifndef __UTILS_H
#define __UTILS_H

int registered(char ip[16]);
void process(Message *request, int master_socket, const struct sockaddr client_addr, socklen_t client_len);
int resolve_query(char ip[16], char* req_domain, unsigned int type);

#endif
