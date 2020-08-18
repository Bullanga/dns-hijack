#ifndef __DNSLIB_H_
#define __DNSLIN_H_
#include <sys/types.h>
#include <sys/socket.h>
#include "dns_types.h"



void generate_failure_response(Message *request, int master_socket, struct sockaddr client_addr, int client_len);
void generate_success_response(Message *request, const char *ip, const char *comment, int master_socket, struct sockaddr client_addr, int client_len);
void parse_client_ip(char *target, const struct sockaddr *client);
void parse_message_raw_body(Message *message);
void resolve_query(Message *message, const RR *records, int records_size);
void RR_populate_missing(RR *rr);
void RR_build_raw_big_endian(RR *rr);



#endif
