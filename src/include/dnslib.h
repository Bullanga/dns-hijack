#ifndef __DNSLIB_H_
#define __DNSLIN_H_



void generate_failure_response(Packet request, int master_socket, const struct sockaddr client_addr, int client_len);
void generate_success_response(Packet request, const char *ip, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len);
void parse_client_ip(char *target, const struct sockaddr *client);
unsigned int parse_requested_domain(char *target, char *data);



#endif
